#include <iostream>
#include <fstream>
#include "utils.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]){

  printf("\nLETTURA DEI DATI DAL DATABASE\n");
  if (reader_db() != 1) {
      fprintf(stderr,"Errore nella lettura dal database");
      return EXIT_FAILURE;
    }
  printf(" \nAVVIO CONNESSIONE SULLA PORTA 9002\n");
  if(webServerCreate() == -1){
    fprintf(stderr, "Errore nella socket\n");
    return EXIT_FAILURE;
  };

  
  return EXIT_SUCCESS;
}