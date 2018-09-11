#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


int setAssociative;
int lineSize;
int cacheSize;
//outputs the cache set in which the address falls
int whichSet( int memAddress)
{    
  return (memAddress%((cacheSize*1024)/(lineSize*setAssociative)));
}

int main(int argc, char *argv[])
{
if(argc != 4)
{
  printf("ERROR! WRONG NUMBER OF ARGUEMENTS. PLEASE INDICATE THE SET ASSOCIATIVITY, LINE SIZE, AND CACHE SIZE");
}
else{
  setAssociative = atoi(argv[1]);
  lineSize = atoi(argv[2]);
  cacheSize = atoi(argv[3]);

   // printf("Set Associative %i \n", setAssociative);
   // printf("Line Size: %i \n", lineSize);
    //printf("Cache Size: %i \n", cacheSize);

  FILE *traceFile;
  char line[8];

  traceFile = fopen("traceFile.txt", "r");
  fscanf(traceFile,"%[^\n]", line);
 
  long int memAddress = strtol(line, NULL,16); 
    
  //  printf("Memory Address: %i \n", memAddress);
   // printf("Line in Hex %s \n", line);
    
    int setNum = whichSet(memAddress);
  //  printf("Set Number: %i \n", setNum);
    fclose(traceFile);
}
}

