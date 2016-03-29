char hexCharToByte(char aHexChar);

char* hexStrToBytes(char* aHexStr, int* aOutLen);

char * fixedXOR(char * aByteBuff, char * bByteBuff,  int len);

char * singleByteXOR (char * inBuff, char key, int len); 

char * repeatingKeyXOR(char * inStr, char * key, int * len);

void printByteBuf(char * byteBuf, int len);

void printByteBufHex(char * byteBuf, int len);

char singleByteDecrypt(char * byteBuf, int len);

double bufferScorer(char * inBuf, int len);

void findNeedle(FILE * haystack, int * needleLine, char * key);