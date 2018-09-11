#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define ASSOCIATIVITY 2 //# of lines per set
#define LINE 16 //size of lines in bytes
#define CACHE 32 //size of cache in kbytes
#define ADDRESS 32 //size in # of bits

//Performs a logical logBASE2 on int argument and returns output
int logicalLog2(int x) 
{
  int count = 0;

  while(x > 1)
  {
    x = x/2;
    count++;
  }
  return count;
}

//Outputs the number of bits in the set index field of the address
int setIndexLength()
{
  return logicalLog2((CACHE*1024)/(LINE*ASSOCIATIVITY)); //convert cache to bits first
}

//Main
int main(int argc, char *argv[])
{
  FILE *traceFile;
  char firstLine[8];

  traceFile = fopen("traceFile.txt", "r");
  fscanf(traceFile,"%[^\n]", firstLine);
  printf(firstLine);
  fclose(traceFile);
}
