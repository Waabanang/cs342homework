#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <ctype.h>
#include "crypto-utils.h"

#define MAX_LINE 128

char hexCharToByte(char aHexChar) {
  if (aHexChar >= '0' && aHexChar <= '9') {
    return aHexChar - '0';
  }
  if (aHexChar >= 'a' && aHexChar <= 'f') {
    return (aHexChar - ('a' - 10));
  }
  printf("Failure! Invalid hex character.\n");
  exit(1);
}

// returns a malloc'd byte buffer, caller frees
char* hexStrToBytes(char* aHexStr, int* aOutLen) {
  // need valid input
  if (!aHexStr || !aOutLen) {
    return NULL;
  }

  // need an even number of hex digits to convert to bytes
  if (strlen(aHexStr) % 2 != 0) {
    return NULL;
  }

  // calculate the length of the output buffer
  *aOutLen = strlen(aHexStr) / 2;
  if (*aOutLen == 0) {
    return NULL;
  }

  // create output buffer
  char *outBuf = malloc(*aOutLen);
  if (!outBuf) {
    return NULL;
  }

  // fill the output buffer
  int inputLen = strlen(aHexStr);
  for (int i = 0; i < inputLen; i = i + 2) {
    char char1 = aHexStr[i];
    char char2 = aHexStr[i + 1];

    char byte = 0;

    byte = hexCharToByte(char1) << 4;
    byte = byte | hexCharToByte(char2);

    outBuf[i / 2] = byte;
  }

  return outBuf;
}
// Takes in bytebuffers
// Outputs a malloc'd byte buffer that's xors together
char * fixedXOR(char * aBuf, char * bBuf, int len){
  char * outBuf = malloc(len);

  for (int i = 0; i < len; i++){
    outBuf[i] = aBuf[i] ^ bBuf[i];
  }

  return outBuf;
}

//Takes in a aHexStr, the length of that buffer, and a key
//Returns a malloc'd byte buffer that is each character of inBuf
//XOR'd against key
char * singleByteXOR (char * inBuf, char key, int len){
  char * outBuf = malloc(len);
  for (int i = 0; i < len; i++){
    outBuf[i] = inBuf[i] ^ key;
  }

  return outBuf;
}
// expects strings in!
//
char * repeatingKeyXOR(char * inStr, char * key, int * len){
  *len = strlen(inStr);
  char * outBuf = malloc(*len);
  
  for (int i = 0; i < *len; i++){
    outBuf[i] = inStr[i] ^ key[i % strlen(key)];
  }

  return outBuf;
}

//prints method for our bytebuffers, which have no null terminating byte
void printByteBuf(char * byteBuf, int len){
  for (int i = 0; i < len; i++){
    printf("%c", byteBuf[i]);
  }
  printf("\n");
}

void printByteBufHex(char * byteBuf, int len){
  for (int i = 0; i < len; i++){
    printf("%x", byteBuf[i]);
  }
  printf("\n");
}

//Takes in a hex string, generates a list of candidate structs
char singleByteDecrypt(char * inBuff, int len){
  double * scores = malloc(sizeof(double) * CHAR_MAX); //is this okay???

  //Scoring step
  for (char c = 0; c < CHAR_MAX; c++){
    char * phrase = singleByteXOR(inBuff, c, len);
    scores[c] = bufferScorer(phrase, len);
    free(phrase);//I free phrase here, is that fine?
  }

  //Get the lowest scoring
  char key = -1;
  double lowestScore = INT_MAX;
  int n = 0;
  int bestI = 0;

  for (char c = 0; c < CHAR_MAX; c++){
    if (scores[c] < lowestScore){
      lowestScore = scores[c];
      key = c;
      bestI = n;
    }
    n++;
  }
  //Clean up
  free(scores);
  return key;
}

double bufferScorer(char * inBuf, int len){
  double expectedFreq[27] = {19.18, 8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015,
    6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095,
    5.987, 6.327, 9.056, 2.758, 0.978, 2.391, 0.150, 1.974, 0.074};
    int * count = malloc(27 * sizeof(int));
    for (int i = 0; i < 27; i++) {
        count[i] = 0;
    }

  int ignored = 0; 
  for (int c = 0; c < len; c++){
    char current_character = inBuf[c];
    if (current_character >= 'A' && current_character <= 'Z'){
      count[current_character - ('A') +1]++; 
    } else if(current_character >= 'a' && current_character <= 'z'){
      count[current_character - ('a') +1]++;
    } else if(current_character == ' '){
      count[0]++;
    }
      else if(isascii(current_character) != 0){
      ignored++;
    } else{
      return INT_MAX;
    }
  }

  double score = 0; 

  for (int n = 0; n < 26; n++){
    score += (((double)count[n]/(len-ignored)) * 100.0 - expectedFreq[n]) * (((double)count[n]/(len-ignored)) * 100.0 - expectedFreq[n]); 
  }

  free(count);
  return score;
}

// Takes in a file, and pointer to a key and returns the line 
// which is encrypted on that file, and saves the key used to
// encrypt that line to the pointer it was passed.
void findNeedle(FILE * haystack, int * needleLine, char * key){
  char line[MAX_LINE];
  memset(line, 0, MAX_LINE);
  int lineNum = 1;
  double lowestScore = INT_MAX;
  char * encodedBuffer;

 

  //Find the bestScoring phrase
  while(fgets(line, MAX_LINE, haystack ) != NULL ){
    line[strcspn(line, "\n")] = 0;
    int len;
    char * buffer = hexStrToBytes(line, &len);
    char possibleKey = singleByteDecrypt(buffer, len);
    if (possibleKey > -1){
      double score = bufferScorer(buffer, len);
      if (score < lowestScore){
        lowestScore = score;
        * needleLine = lineNum;
        * key = possibleKey;
        encodedBuffer = buffer;
        printf("Found message on line: %d, key was: %x, scored: %f\n", lineNum, possibleKey, score);
        printf("Message might be:");
        printByteBuf(singleByteXOR(encodedBuffer, possibleKey, len), len);
      }
    } lineNum++;
    free(buffer);
  }
 
}
























