char hexCharToByte(char aHexChar);

char* hexStrToBytes(char* aHexStr, int* aOutLen);

char * fixedXOR(char * aHexStr, char * bHexStr,  int len);

char * singleByteXOR (char * inStr, char key); 

char * repeatingKeyXOR(char * inStr, char * key);

void printByteBuf(char * byteBuf, int len);

char singleByteDecrypt(char * inStr);

double bufferScorer(char * inBuf, int len);

void findNeedle(FILE * haystack);