/////////////////////////////////////////////////////////////////////////
////////////////////LAB 1 - CACHE SIMULATOR//////////////////////////////
/////////////DYLAN BANH, MARISA LIU, SOLOMON WANG///////////////////////
////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


///////////////////////////////////////////////////////////////////////
///////////////////////GLOBAL VARIABLES////////////////////////////////
///////////////////////////////////////////////////////////////////////

int setAssociative;
int lineSize;
int cacheSize;
int addressLength;
int tagLength;
char *fileName;
int numberSets;
int setnum;
int **tagArray;
int **lruArray;


///////////////////////////////////////////////////////////////////////
///////////////////////////FUNCTIONS///////////////////////////////////
//////////////////////////////////////////////////////////////////////

//Performs a logical logBASE2 on int argument and returns output
int logicalLog2(int x) 
{
  assert(x>0);
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
  int length = (logicalLog2((cacheSize*1024)/lineSize))/setAssociative; //convert cache to bits first
  assert(length<numberSets);
  return length; 
}

//outputs the cache set in which the address falls
int whichSet( int memAddress)
{    
  int set = memAddress%numberSets;
  assert(set>=0 && set<=numberSets);
  return set;
}

//Outputs the number of bits in the line offset field of the address.
int offsetLength()
{
  int length = logicalLog2(lineSize);
  assert(length<lineSize);
  return length;
}

//Outputs tag bits associated with the address
int tagBits(int memAddress)
{
  int initAddress = memAddress;
  int count = (setIndexLength()+offsetLength());
  while(count>0)
  {
    memAddress = memAddress/2;
    count--;
  }
  assert(memAddress<=initAddress/2);
  return memAddress;
}

/*If there is a hit, this outputs the cache way in which the accessed 
 * line can be found; it returns -1 if there is a cache miss 
 * */
int hitWay(int tagbits, int memAddress)
{
  assert(tagbits>0);
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
  assert(lineIndex<setAssociative);
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
  int prevVal;

  for(lineCount=0; lineCount<setAssociative; lineCount++)
  {
	if(lruArray[setnum][lineCount]==-1)
	{
	  prevVal=tagArray[setnum][lineCount];
	  tagArray[setnum][lineCount]=tagBits(memAddress);
	  assert(prevVal!=tagArray[setnum][lineCount]);
	  lruArray[setnum][lineCount]=0;
	  inTagArray=1;
	  break;
	}
	else
	{
	  lruArray[setnum][lineCount]=lruArray[setnum][lineCount]+1;
	}
  }
  //If set in tagArray is already full
  if(inTagArray==0){
	//find index of address thats been there the longest
    	int max=0;    //holds max lru time
	int index=0;   //index of max lru time
	int i;
	for(i=0; i < setAssociative; i++){
	if(max < lruArray[setnum][i])
	  max=lruArray[setnum][i];
	 index=i;
	}
	assert(max>0);
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
  assert(argc == 5);
    
    setAssociative = atoi(argv[1]);
    lineSize = atoi(argv[2]);
    cacheSize = atoi(argv[3]);
    fileName = argv[4];
    numberSets = (cacheSize*1024)/(setAssociative*lineSize);
    
    //Initialize the tagArray and lruArray
    int row;
    tagArray = (int **)malloc(numberSets*sizeof(int *));
    lruArray = (int **)malloc(numberSets*sizeof(int *));
    for(row=0; row < numberSets; row++)
    {
      tagArray[row] = (int *)malloc(setAssociative*sizeof(int));
      lruArray[row] = (int *)malloc(setAssociative*sizeof(int));
    }
    
    int column;
    for(row=0; row < numberSets; row++){
      for(column=0; column < setAssociative; column++){lruArray[row][column]=-1;}
    }    
    //Open trace file
    FILE *traceFile;
    unsigned int line;
    traceFile = fopen(fileName, "r");    
    //Loop to go through file to the last line
    int numberHits = 0;
    int numberMisses = 0;
    while(fscanf(traceFile, "%x",&line)!=EOF)
    {        
      tagLength = addressLength-setIndexLength()-offsetLength();
      int tagbits = tagBits(line);
      int hitOrMiss = hitWay(tagbits, line);
      if(hitOrMiss == -1)
      {
        updateOnMiss(line);
        numberMisses++;
      }
      else
      {
        updateOnHit(line, hitOrMiss);
        numberHits++;
      }
    }

    //Print out the MissRate for the file
    float missRate = (float) numberMisses/(numberMisses+numberHits);
    printf("%s %d %d %d %f \n",fileName, cacheSize, setAssociative, lineSize, missRate);
    fclose(traceFile);
    

}


