#include <stdio.h>
#include <mysql/mysql.h>
#include <list>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
using namespace std;
 
MYSQL *con;

void insertDB(MYSQL *con) {
    if (mysql_query(con, "INSERT INTO Stanze VALUES(A1,5.23,21.88,0.02)")) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
}

int connection_db(){
     con = mysql_init(NULL);

  if (con == NULL) {
      fprintf(stderr, "mysql_init() failed\n");
      return 0;
      exit(EXIT_FAILURE);
  }
  if (!(mysql_real_connect(con, "localhost", "root", "", "pick_delivery", 3306, NULL, 0))) {
      //finish_with_error(con);
      fprintf(stderr, "mysql_real_connect() failed\n");
      return 0;
      exit(EXIT_FAILURE);
  }
  return 1;
}

int reader_db() {
    if(connection_db()==0){
        exit(EXIT_FAILURE);
    }
  if (mysql_query(con, "SELECT * FROM stanze")) {
      //finish_with_error(con);
      fprintf(stderr, "mysql_query() failed\n");
      exit(EXIT_FAILURE);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  if (result == NULL) {
      //finish_with_error(con);
      fprintf(stderr, "mysql_store_result() failed\n");
      exit(EXIT_FAILURE);
  }

  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result))) {
      for(int i = 0; i < num_fields; i++) {
          printf("%s ", row[i] ? row[i] : "NULL");
      }
          printf("\n");
  }
  mysql_free_result(result);
  mysql_close(con);
  return 1;
}


//coordinate per dove deve andare a prendere il pacco
 float* get_partenza(char* src){
     if(connection_db()==0){
        exit(EXIT_FAILURE);
    }
    float *coord_partenza=(float*)malloc((sizeof(float)*3));

    string query = "select * from stanze where stanza=";
    string room = src;
    string destination = query +"'"+room+"'";
    const char *c = destination.c_str();

     printf("source read in reader\n");
     printf("%s\n", c);
    
    if (mysql_query(con,(const char*)c)) {
      //finish_with_error(con);
      fprintf(stderr, "mysql_query() failed\n");
      exit(EXIT_FAILURE);
    }

    MYSQL_RES *result = mysql_store_result(con);
  if (result == NULL) {
      //finish_with_error(con);
      fprintf(stderr, "mysql_store_result() failed\n");
      exit(EXIT_FAILURE);
  }

  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result))) {
      for(int i = 0; i < num_fields; i++) {
          coord_partenza[0]=atof(row[1]);
          coord_partenza[1]=atof(row[2]);
          coord_partenza[2]=atof(row[3]);

          printf("%s ", row[i] ? row[i] : "NULL");
      }
          printf("\n");
  }
  mysql_free_result(result);

    printf("coordinata src x is %f\n",coord_partenza[0]);
    printf("coordinata src y is %f\n",coord_partenza[1]);
    printf("coordinata src theta is %f\n",coord_partenza[2]);
    
    
    mysql_close(con);
   
    return coord_partenza;
    
 }
// //coordinate dove riportare il pacco
 float* get_arrivo(char* dst){
      if(connection_db()==0){
        exit(EXIT_FAILURE);
    }

    float *coord_arrivo=(float*)malloc((sizeof(float)*3));

    string query = "select * from stanze where stanza=";
    string room = dst;
    string destination = query +"'"+room+"'";
    const char *c = destination.c_str();

     printf("destination read in reader\n");
     printf("%s\n", c);
    
    if (mysql_query(con,(const char*)c)) {
      //finish_with_error(con);
      fprintf(stderr, "mysql_query() failed\n");
      exit(EXIT_FAILURE);
  }

    MYSQL_RES *result = mysql_store_result(con);
  if (result == NULL) {
      //finish_with_error(con);
      fprintf(stderr, "mysql_store_result() failed\n");
      exit(EXIT_FAILURE);
  }

  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result))) {
      for(int i = 0; i < num_fields; i++) {
          coord_arrivo[0]=atof(row[1]);
          coord_arrivo[1]=atof(row[2]);
          coord_arrivo[2]=atof(row[3]);

          printf("%s ", row[i] ? row[i] : "NULL");
      }
          printf("\n");
  }
  mysql_free_result(result);

    printf("coordinata dst x is %f\n",coord_arrivo[0]);
    printf("coordinata dst y is %f\n",coord_arrivo[1]);
    printf("coordinata dst theta is %f\n",coord_arrivo[2]);

mysql_close(con);
return coord_arrivo;
    
    
 }