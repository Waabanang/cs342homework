#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "crypto-utils.h"

#define MAX_LINE 61

// Compile with: gcc -std=c99 challenge4.c crypto-utils.c -g -o challenge4

int main(int argc, char *argv[]){
	static const char filename[] = "haystack.txt";
	FILE *haystack = fopen ( filename, "r" );
	findNeedle(haystack);
	}