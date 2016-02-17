#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "crypto-utils.h"

// Compile with: gcc -std=c99 challenge3.c crypto-utils.c -g -o challenge3


static char * aHexStr = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

int main(int argc, char *argv[]){
	int printLen = 34;

	for (char i = 33; i < (CHAR_MAX); i++){
		printf("With key %c: ", i);
		char * candidate = singleByteXOR(aHexStr, i);
		printByteBuf(candidate, &printLen);
		free(candidate);
	}
}