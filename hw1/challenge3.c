#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "crypto-utils.h"

// Compile with: gcc -std=c99 challenge3.c crypto-utils.c -g -o challenge3


static char * aHexStr = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

int main(int argc, char *argv[]){
	int len; 

	char * aByteBuff = hexStrToBytes(aHexStr, &len);
	char key = singleByteDecrypt(aByteBuff, len);
	printf("The key was: %c \nThe message is: ", key);
	char * result = singleByteXOR(aByteBuff, key, len);
	printByteBuf(result, len);

	free(result);
}