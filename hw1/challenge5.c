#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "crypto-utils.h"

// Compile with: c

static char * inStr = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
static char * key = "ICE";

static char * outStr ="0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

int main(int argc, char *argv[]){
	int len, answerLen, keyLen;
	char * encoded = repeatingKeyXOR(inStr, key, &len);
	char * answer = hexStrToBytes(outStr, &answerLen);

	printf("%s", "Test  : ");
	printByteBufHex(encoded, len);
	printf("%s", "Answer: ");
	printByteBufHex(answer, answerLen);

	printf("The diff is %d\n", memcmp(encoded, answer, len));


	free(encoded);
	free(answer);
	return 0;
}