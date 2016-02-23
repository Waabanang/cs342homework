#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"

// Compile with: gcc -std=c99 challenge2.c crypto-utils.c -g -o challenge2

static char * stringA = "1c0111001f010100061a024b53535009181c";
static char * stringB = "686974207468652062756c6c277320657965";

static char * stringC = "746865206b696420646f6e277420706c6179";

int main(int argc, char * arg[]){
	int lenA, lenB, lenC;

	char * bufferA = hexStrToBytes(stringA, &lenB);
	char * bufferB = hexStrToBytes(stringB, &lenA); 

	if (lenA != lenB){
		printf("%s\n", "lengths don't match");
		return -1;
	}

	char * test = fixedXOR(bufferA, bufferB, lenA);
	char * answer = hexStrToBytes(stringC, &lenC);

	if (memcmp(test, answer, lenA)){
		printf("success! \n");
	} else {
		printf("failure.\n");
	}
	free(answer);
	free(test);

	return 0;
}