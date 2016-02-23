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

	char * bufferA = hexStrToBytes(stringA, &lenA);
	char * bufferB = hexStrToBytes(stringB, &lenB);

	if (lenA != lenB){
		printf("%s\n", "lengths don't match");
		return -1;
	}

	char * test = fixedXOR(bufferA, bufferB, lenA);
	char * answer = hexStrToBytes(stringC, &lenC);

	printByteBuf(answer, lenC);
	printByteBuf(test, lenA);

	if (memcmp(test, answer, lenC) == 0){
		printf("success! \n");
	} else {
		printf("failure.\n");
	}
	free(answer);
	free(test);

	return 0;
}