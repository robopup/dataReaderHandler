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

#define BUFLEN		432

// Function Protoypes
void swap(char *firstElem, char *secondElem);

int main()
{
	unsigned char buffer[BUFLEN];
	FILE *ptr;
	DWORD data[108] = { 0 };
	int wrapSize = 4;

	// Open file for writing to (CSV)
	FILE *

	// C:\Users\nocturnalhippo\Desktop
	// if ((ptr = fopen("E:\\WriteBinaryData.bin", "rb")) == NULL) {
	if ((ptr = fopen("C:\\Users\\ronny.wong\\OneDrive\\Geotech\\_dataFiles\\WriteBinaryData.bin", "rb")) == NULL) {
		puts("The file was not opened\n\r");
		puts("Press a key to exit()\n\r");
		return 1;
	}
	else{
		puts("The file was opened.\n\r");
	}

	fread(buffer, sizeof(buffer), 1, ptr);

	printf("Size of buffer is: %d\r\n", sizeof(buffer));
	getchar();
	
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
		printf("%13d", data[track]);
		skipline++;
		track++;
		if (skipline == 6) {
			skipline = 0;
			printf("\r\n");
		}
	}
	getchar();

	/*
	int count = 0;
	for (int i = 0; i < BUFLEN; i++) {
		printf("%02x ", buffer[i]);
		count++;
		if (count == 4) {
			printf("\n\r");
			count = 0;
		}
	}
	*/

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