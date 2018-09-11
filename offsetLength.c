#include <stdio.h>

#define LINESIZE 32

int logtwo();

int main()
{

printf("%d", logtwo(LINESIZE));
}

int logtwo(int x){

int o=0;
while(x > 1){
x=x/2;
o=o+1;
}

return o;

}


