#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"


static char * stringA = "1c0111001f010100061a024b53535009181c";
static char * stringB = "686974207468652062756c6c277320657965";

int main(int argc, char * arg[]){
	int * bufferLen = 18;
	char * answer = fixedXOR(stringA, stringB);

	for (int i = 0; i < * bufferLen; i++){
		printf(" %c ", answer[i]);
	}
	printf("/n");
	free(answer);
	return 0;
}