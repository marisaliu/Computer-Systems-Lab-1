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
int setnum;
int **tagArray;
int **lruArray;

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

/*If there is a hit, this outputs the cache way in which the accessed 
 * line can be found; it returns -1 if there is a cache miss 
 * */
int hitWay(int tagbits, int memAddress)
{
  setnum = whichSet(memAddress);
  int lineCount;
  for(lineCount = 0; lineCount < setAssociative; lineCount++)
  {
    if(tagbits == tagArray[setnum][lineCount])
    {
      return lineCount;
    }
  }
  return -1; 
}


//Updates the tagArray and lruArray upon a hit. This function is only called on a cache hit.
void updateOnHit(int memAddress, int lineIndex)
{
  int lineCount;
  for(lineCount=0; lineCount<setAssociative; lineCount++)
  { 
    if(lruArray[setnum][lineCount]!=-1){lruArray[setnum][lineCount]++;}
    if(lineCount==lineIndex){lruArray[setnum][lineCount]=0;}
  }
}

//Updates the tagArray and lruArray upon a miss. This function is only called on a cache miss.
void updateOnMiss(int memAddress)
{
  int setnum = whichSet(memAddress);
  int lineCount;
  int inTagArray=0;   //0 is false, 1 is true
  
  for(lineCount=0; lineCount<setAssociative; lineCount++)
  {
	if(tagArray[lineCount]==NULL)
	{
	  tagArray[setnum][lineCount]=tagBits(memAddress);
	  lruArray[setnum][lineCount]=0;
	  inTagArray=1;
	  break;
	}
	else
	{
	  lruArray[setnum][lineCount]=lruArray[setnum][lineCount]+1;
	}
  }
  
  if(inTagArray==0){
	//find index of address thats been there the longest
    	int max=0;    //holds max lru time
	int index=0;   //index of max lru time
	for(int i=0; i < setAssociative; i++){
	if(max < lruArray[setnum][i])
	  max=lruArray[setnum][i];
	 index=i;
	}
	//replace
	tagArray[setnum][index]=tagBits(memAddress);
	lruArray[setnum][index]=0;
  }

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

//    printf("Set Associative %i \n", setAssociative);
  //  printf("Line Size: %i \n", lineSize);
    //printf("Cache Size: %i \n", cacheSize);
    
    int row;
    tagArray = (int **)malloc(numberSets*sizeof(int *));
    lruArray = (int **)malloc(numberSets*sizeof(int *));
    for(row=0; row < numberSets; row++)
    {
      tagArray[row] = (int *)malloc(setAssociative*sizeof(int));
      lruArray[row] = (int *)malloc(setAssociative*sizeof(int));
    }
    
    //initialize lruArray
    int column;
    for(row=0; row < numberSets; row++){
      for(column=0; column < setAssociative; column++){lruArray[row][column]=-1;}}    

    FILE *traceFile;
    char *line;
    traceFile = fopen("traceFile.txt", "r");
    
    
    
    
    


    int count = 0;
    while((count++ < 50)&& fscanf(traceFile, "%[^\n]\n",line) == 1)
    {
    
    addressLength=strlen(line)*4;
    long int memAddress = strtol(line, NULL,16);  //converts hex string to int 
   //printf("Count %i \n", count);

    
    //printf("Memory Address: %i \n", memAddress);
    printf("%s %s\n","Line in Hex", line);
   // printf("%d",addressLength); 
    //printf("Set Number: %i \n", setNum);
    
  
    
    tagLength = addressLength-setIndexLength()-offsetLength();
    //printf(tagSize is %d", tagSize);
    
    
    int tagbits = tagBits(memAddress);
    int hitOrMiss = hitWay(tagbits, memAddress);
    //printf("Hit or Miss: %i \n", hitOrMiss);
    //newline = fscanf(traceFile, "%[^\n]", line);
    }
fclose(traceFile);

    
    }

}


