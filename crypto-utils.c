#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include "crypto-utils.h"

#define MAX_LINE 61

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
// Takes in HEX STRINGS
// Outputs a malloc'd byte buffer that's xors together
char * fixedXOR(char * aHexStr, char * bHexStr){
  if (strlen(aHexStr) != strlen(bHexStr)){ 
    printf("Input not of equal length /n");
    exit(1);
  }

  int len = (int) strlen(aHexStr);

  char * aBuf = hexStrToBytes(aHexStr, &len);
  char * bBuf = hexStrToBytes(bHexStr, &len);
  char * outBuf = malloc(sizeof(aBuf));

  for (int i = 0; i < len/2; i++){
    outBuf[i] = aBuf[i] ^ bBuf[i];
  }

  free(aBuf);
  free(bBuf);

  return outBuf;
}

//Takes in a aHexStr, the length of that buffer, and a key
//Returns a malloc'd byte buffer that is each character of inBuf
//XOR'd against key
char * singleByteXOR (char * inStr, char key){
  int lenStr = (int) strlen(inStr);
  int lenBuf = lenStr/2;

  char * inBuf = hexStrToBytes(inStr, &lenStr); 
  char * outBuf = malloc(sizeof(inBuf));

  for (int i = 0; i < lenBuf; i++){
    outBuf[i] = inBuf[i] ^ key;
  }

  return outBuf;
}
// Expects that both key and inStr are proper strings
//
char * repeatingKeyXOR(char * inStr, char * key){
  char * outBuf = malloc(sizeof(strlen(inStr)));

  for (int i = 0; i < strlen(inStr); i++){
    outBuf[i] = inStr[i] ^ key[i % strlen(key)];
  }

  return outBuf;
}

//prints method for our bytebuffers, which have no null terminating byte
void printByteBuf(char * byteBuf, int len){
  for (int i = 0; i < len; i++){
    printf("%x", byteBuf[i]);
  }
  printf("\n");
}

//Takes in a hex string, generates a list of candidate structs
char singleByteDecrypt(char * inStr){
  int lenStr = (int) strlen(inStr);
  int lenBuf = lenStr/2;
  bool * candidateKeys = malloc(sizeof(bool) * CHAR_MAX);
  double * scores = malloc(sizeof(double) * CHAR_MAX); 

  //Exclusion step
  for (char c = 0; c < CHAR_MAX; c++){
    char * candidate = singleByteXOR(inStr, c);
    candidateKeys[c] = true;
    for (int i = 0; i < lenBuf; i++){
      if (candidate[i] < 32 || candidate[i] > 122){
        candidateKeys[c] = false;
      }
    }
    free(candidate);
  }

  //Scoring step
  for (char c = 0; c < CHAR_MAX; c++){
    if (candidateKeys[c]){
      char * phrase = singleByteXOR(inStr, c);
      scores[c] = bufferScorer(phrase, lenBuf);
      free(phrase);
    } else {
      scores[c] = INT_MAX;
    }
  }
  //Get the lowest scoring
  char key;
  double lowestScore = INT_MAX;

  for (char c = 0; c < CHAR_MAX; c++){
    if (candidateKeys[c]){
      if (scores[c] < lowestScore){
        lowestScore = scores[c];
        key = c;
      }
    }
  }

  //Clean up
  free(candidateKeys);
  free(scores);
  return key;
}

double bufferScorer(char * inBuf, int len){
  double expectedFreq[27] = {19.18, 8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015,
    6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095,
    5.987, 6.327, 9.056, 2.758, 0.978, 2.391, 0.150, 1.974, 0.074};
  int * count = malloc(sizeof(expectedFreq));
  memset(count, 0, sizeof(expectedFreq));

  int ignored = 0; 
  for (int c = 0; c < len; c++){
    char current_character = inBuf[c];
    if (current_character >= 'A' && current_character <= 'Z'){
      count[current_character - ('A' + 1)]++; 
    } else if(current_character >= 'a' && current_character <= 'z'){
      count[current_character - ('a' + 1)]++;
    } else if(current_character == ' '){
      count[0]++;
    }
      else{
      ignored++;
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
void findNeedle(FILE * haystack){
  char line[MAX_LINE];
  int lineNum = 0;

  //Find the bestScoring phrase
  while(fgets(line, sizeof line, haystack ) != NULL ){
    char possibleKey = singleByteDecrypt(line);
    int len = strlen(line)/2;
    if (possibleKey){
      char * possibleMessage = singleByteXOR(line, possibleKey);
      printf("Line number %d, with key %c, mesage: ", lineNum, possibleKey);
      printByteBuf(possibleMessage, len);
      free(possibleMessage);
      }
    lineNum++;
  }
}























