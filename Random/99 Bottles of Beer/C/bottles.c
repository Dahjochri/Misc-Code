#include <stdio.h>

int main()
{
	int n=99;
	do
	{
		printf("%d bottles of beer on the wall\n",n);
		printf("%d bottles of beer\n",n);
		printf("Take one down, pass it around\n");
		printf("%d bottles of beer on the wall\n\n",--n);
	}while(n>0);
}
