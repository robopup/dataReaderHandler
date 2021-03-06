// dataHandler.cpp : Defines the entry point for the console application.
// Developers: Ron/Marc/Ryan
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
	FILE *readPtr;
	FILE *csvPtr;
	DWORD data[108] = { 0 };
	int wrapSize = 4;

	// Open file for writing to (CSV) -----------------------------------------------------------------------------------
	if ((csvPtr = fopen("C:\\Users\\ronny.wong\\OneDrive\\Geotech\\_dataFiles\\geotech_out_all.csv", "w+")) == NULL) {
		puts("Was not able to create CSV file for exporting\n\r");
		puts("Press a key to exit()\n\r");
		return EXIT_FAILURE;
	}
	else {
		puts("The CSV file wsa opened.\n\r");
	}
	fprintf(csvPtr, "Geotech Ltd. - R33 MAG DATA COLLECTOR\r");
	fprintf(csvPtr, "R&D - Ryan Raz/Marc-Andre Theberge/Dave West/Ronnie Wong\r\n");
	fprintf(csvPtr, "Data Point 1, Data Point 2, Data Point 3, Data Point 4, Time, Count\n");
	// -------------------------------------------------------------------------------------------------------------------

	// Open file for reading from SD Card/Physical Drive -----------------------------------------------------------------
	if ((readPtr = fopen("C:\\Users\\ronny.wong\\OneDrive\\Geotech\\_dataFiles\\WriteBinaryData.bin", "rb")) == NULL) {
		puts("The file was not opened\n\r");
		puts("Press a key to exit()\n\r");
		return EXIT_FAILURE;
	}
	else {
		puts("The file was opened.\n\r");
	}
	// --------------------------------------------------------------------------------------------------------------------

	// Begin reading in data...
	while (fread(buffer, sizeof(buffer), 1, readPtr) != NULL) {
		
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
			fprintf(csvPtr, "%13d, ", data[track]);
			skipline++;
			track++;
			if (skipline == 6) {
				skipline = 0;
				printf("\r\n");
				fprintf(csvPtr, "\r");
			}
		}

	}
	
	puts("Closing FILE handles...\n\r");
	if ((fclose(csvPtr) == NULL)) {
		puts("Cannot close FILE handle for CSV file.\n\r");
	}
	else {
		puts("Successfully closed CSV file handle.\n\r");
	}
	if ((fclose(readPtr) == NULL)) {
		puts("Cannot close FILE handle for data file.\n\r");
	}
	else {
		puts("Successfully closed data file handle.\r\n");
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