#include <unistd.h>
#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <signal.h>
#include <sstream>
#include <libwebsockets.h>
#include <set>
#include "utils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define fifo "../fifo"

typedef struct WSContent{
  int port;
  const char *interface;

}WSContent;

static int callback_http(
    struct lws *wsi,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len
) {
  switch (reason) {

    case LWS_CALLBACK_ESTABLISHED:
      printf("connection established\n");
      break;

    case LWS_CALLBACK_RECEIVE: {

      break;
    }
    default:
      break;
  }

  return 0;
}

static int callback_dumb_increment( //struct libwebsocket_context * this_context,
                                  struct lws *wsi,
                                  enum lws_callback_reasons reason,
                                  void *user, void *in, size_t len) {


  switch (reason) {
  case LWS_CALLBACK_ESTABLISHED:
    printf("connection established\n");

    break;



  case LWS_CALLBACK_RECEIVE: {
      int res;
    
      printf("lws callback received\n");
      char *buf = (char*) malloc((sizeof(char)*len)+1);
      strncpy(buf,(char*)in,len);
      buf[len] = '\0';

//controllo con if len
      if(len < 7){ 
      
      char * src = strtok(buf, ",");//src = a1
      char * dst =strtok(NULL,",");
      
      float* coord_arrivo;
      float* coord_partenza;
      coord_partenza=get_partenza(src);
      
      coord_arrivo=get_arrivo(dst);
      
      int fd=open(fifo,O_WRONLY);
      if(fd==-1){
        printf("errore apertura fifo\n");
        return EXIT_FAILURE;
      }
      int i;
      for(i=0;i<3;i++){
        if(write(fd,&coord_partenza[i],sizeof(float))==-1){
          printf("errore in scrittura fifo fifo\n");
          return EXIT_FAILURE;
        }
      }
     

    //////////////prova
    sleep(5);
    for(i=0;i<3;i++){
        if(write(fd,&coord_arrivo[i],sizeof(float))==-1){
          printf("errore in scrittura fifo fifo\n");
          return EXIT_FAILURE;
        }
    }

    //////////////prova
      close(fd);
     
      printf("coordinata in server main of x is %f\n",coord_partenza[0]);
      
     // printf("%s\n",buf);
      int size = strlen(buf);
   //   lws_write(wsi, (unsigned char*) buf, size, LWS_WRITE_TEXT); 
      //free(buf);
      }
      else{
        char* nome = strtok(buf, ",");//src = a1
        char* password =strtok(NULL,",");
        int loggato;
        loggato=get_login(nome,password);
        if( (loggato)==1){
          //login ok
         std::string s= "1\0";
         char* log=(char*)malloc(sizeof(char)*2);
         strcpy(log,s.c_str());

          lws_write(wsi, (unsigned char*) log, 2, LWS_WRITE_TEXT);
        }
        else{
          //utente non presente
           std::string s= "0\0";
              char* log=(char*)malloc(sizeof(char)*2);
             strcpy(log,s.c_str());

            lws_write(wsi, (unsigned char*) log, 2, LWS_WRITE_TEXT);
        }
      }



    }
    break;


  default:
    break;
  }

  return 0;
}



int webServerCreate(){
  int port = 9002;
  int opts = 0;
  const char *interface = NULL;
  struct lws_context *context;
  struct lws_context_creation_info info;


  //creazione fifo 

   int res;
  //    //creazione fifo  in writeonly

   unlink(fifo);
  
   res=mkfifo(fifo,0666);
    
     if(res==-1){
       printf("errore creazione fifo\n");
       return EXIT_FAILURE;
     }

  memset(&info, 0, sizeof info);

  struct lws_protocols protocols[] = {
    // first protocol must always be HTTP handler 
    {
      .name="http-only",   // name
      .callback=callback_http, // callback
      .per_session_data_size=0,              // per_session_data_size
      .rx_buffer_size=0
    },
    {
      .name = "pick-protocol", // protocol name - very important!
      .callback = callback_dumb_increment,   // callback
      .per_session_data_size=0,                          // we don't use any per session data
      .rx_buffer_size=0
    },
    {
      .name=NULL,
        .callback=NULL,
        .per_session_data_size=0,   // End of list 
        .rx_buffer_size=0
    }
  };

  info.port = port;
  info.iface = interface;
  info.protocols = protocols;
  //info.extensions = lws_get_internal_extensions();
  //if (!use_ssl) {
  info.ssl_cert_filepath = NULL;
  info.ssl_private_key_filepath = NULL;
  //} else {
  //  info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
  //  info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
  //}
  info.gid = -1;
  info.uid = -1;
  info.options = opts;



  // create libwebsocket context representing this server
  context = lws_create_context(&info);

  if (context == NULL) {
    fprintf(stderr, "libwebsocket init failed\n");
    return -1;
  }

  printf("starting server...\n");

  // infinite loop, to end this server send SIGTERM. (CTRL+C)
  while (1) {
    lws_service(context, 50);
    // libwebsocket_service will process all waiting events with
    // their callback functions and then wait 50 ms.
    // (this is a single threaded web server and this will keep our
    // server from generating load while there are not
    // requests to process)
  }

  lws_context_destroy(context);

  // if(unlink(fifo)==-1){
  //       printf("unlink");
  //       return EXIT_FAILURE;
  // }

  return 0;

}

