#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int setAssociative;
int lineSize;
int cacheSize;
int addressLength;
int tagLength;
int numberSets;

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
  return logicalLog2(numberSets); //convert cache to bits first
}

//outputs the cache set in which the address falls
int whichSet( int memAddress)
{    
  return (memAddress%numberSets);
}

//Outputs the number of bits in the line offset field of the address.
int offsetLength()
{
  return logicalLog2(lineSize);
}

//Outputs tag bits associated with the address
int tagBits(int memAddress)
{
  int count = (setIndexLength()+offsetLength());
  while(count>0)
  {
    memAddress = memAddress/2;
    count--;
  }
  return memAddress;
}


//////////////////////////////////////////////////////////
/////////////////////Main Code////////////////////////////
//////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  if(argc != 4)
  {
    printf("ERROR! WRONG NUMBER OF ARGUEMENTS. PLEASE INDICATE THE SET ASSOCIATIVITY, LINE SIZE, AND CACHE SIZE");
  }
  else
  {
    setAssociative = atoi(argv[1]);
    lineSize = atoi(argv[2]);
    cacheSize = atoi(argv[3]);
    numberSets = (cacheSize*1024)/(setAssociative*lineSize);
/*
    printf("Set Associative %i \n", setAssociative);
    printf("Line Size: %i \n", lineSize);
    printf("Cache Size: %i \n", cacheSize);
*/
    FILE *traceFile;
    char *line;
    traceFile = fopen("traceFile.txt", "r");
    fscanf(traceFile,"%[^\n]", line);
    addressLength=strlen(line)*4;
    long int memAddress = strtol(line, NULL,16);  //converts hex string to int 
   

    int setNum = whichSet(memAddress);
    /*
    printf("Memory Address: %i \n", memAddress);
    printf("Line in Hex %s \n", line);
    printf("%d",addressLength); 
    printf("Set Number: %i \n", setNum);
    */

    fclose(traceFile);

    tagLength = addressLength-setIndexLength()-offsetLength();
    //printf("tagSize is %d", tagSize);
    unsigned int **tagArray = malloc(sizeof(tagLength));
    unsigned int **lruArray = malloc(sizeof(lineSize*numberSets));
  }
}


