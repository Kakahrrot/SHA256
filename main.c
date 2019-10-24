#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SHA.h" 
int main(void)
{
	char s[MSG_SIZE];
	while(fgets(s, MSG_SIZE, stdin))
	{
		s[strlen(s) - 1] = 0;
		unsigned int *digest = sha256(s);
		for(int i = 0; i < 8; i++)
			printf("%08X", digest[i]);
		printf("\n");
		free(digest);
	}
}
