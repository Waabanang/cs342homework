#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"


static char * stringA = "1c0111001f010100061a024b53535009181c";
static char * stringB = "686974207468652062756c6c277320657965";

static char * stringC = "746865206b696420646f6e277420706c6179";

int main(int argc, char * arg[]){
	int len = (int)strlen(stringC);
	char * test = fixedXOR(stringA, stringB);
	char * answer = hexStrToBytes(stringC, &len);

	if (strcmp(test, answer)){
		printf("success! \n");
	} else {
		printf("failure.\n");
	}
	free(answer);
	free(test);

	return 0;
}