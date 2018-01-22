// dataHandler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <strsafe.h>

using namespace std;

#define BUFLEN		512

// Function Protoypes
void swap(char *firstElem, char *secondElem);

int main()
{
	unsigned char buffer[BUFLEN];
	FILE *ptr;
	DWORD data[108] = { 0 };
	int wrapSize = 4;

	if ((ptr = fopen("I:\\WriteBinaryData.bin", "rb")) == NULL) {
		printf("The file was not opened\n\r");
		printf("Press a key to exit()\n\r");
		return 1;
	}
	else{
		printf("The file was opened.\n\r");
	}

	fread(buffer, sizeof(buffer), 1, ptr);

	// Zero-crossing conversion : nanoteslas to frequency
	
	// Swap and rearrange the buffer to big endian
	for (int i = 0; i < 432; i += 4) {
		swap(buffer[i + 0], buffer[i + 3]);
		swap(buffer[i + 1], buffer[i + 2]);
	}

	int track = 0;
	int skipline = 0;
	for (int k = 0; k < BUFLEN; k += 4) {
		for (int m = 0; m < wrapSize; m++) {
			data[track] = (data[track] << 8) | (unsigned char)buffer[m + k];
		}
		printf("%10d", data[track]);
		skipline++;
		track++;
		if (skipline == 6) {
			skipline = 0;
			printf("\r\n");
		}
	}

	int count = 0;
	for (int i = 0; i < BUFLEN; i++) {
		printf("%02x ", buffer[i]);
		count++;
		if (count == 4) {
			printf("\n\r");
			count = 0;
		}
	}

	printf("\r\n");
	printf("Press any key to continue()\n\r");
	getchar();

    return 0;
}

// Functions
void swap(char *firstElem, char *secondElem)
{
	char temp = *firstElem;
	*firstElem = *secondElem;
	*secondElem = temp;
}