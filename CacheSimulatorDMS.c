#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int logicalLog2(int x) //
{
int count = 0;

while(x > 1)
{
x = x/2;
count++;
}
return count;
}


int main(int argc, char *argv[])
{
FILE *traceFile;
char firstLine[8];

traceFile = fopen("traceFile.txt", "r");
fscanf(traceFile,"%[^\n]", firstLine);
printf(firstLine);
fclose(traceFile);

}


