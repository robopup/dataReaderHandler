// dataHandler.cpp : Defines the entry point for the console application.
// Developers: RW/MT/RR
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
	FILE *geosoftio;
	DWORD data[108] = { 0 };
	int wrapSize = 4;
	long filesize = 0;

	// Create Geosoft import template *.i*
	// Check with Geoff to see if this is still required.
	if ((geosoftio = fopen("C:\\Users\\ronny.wong\\OneDrive\\Geotech\\_dataFiles\\geomag.i0", "w+")) == NULL) {
		puts("Can not generate Geosoft XYZ import template\r\n");
		puts("Press a key to exit()\r\n");
		return EXIT_FAILURE;
	}
	else {
		puts("The file was created with the following parameters:\r\n");
		fprintf(geosoftio, "IMPORT 6, NORMAL, in, LONG, FLOAT, 10, 2");
	}

	// Open file for reading from SD Card/Physical Drive -----------------------------------------------------------------
	if ((readPtr = fopen("C:\\Users\\ronny.wong\\OneDrive\\Geotech\\_dataFiles\\WriteBinaryData.bin", "rb")) == NULL) {
		puts("The file was not opened\n\r");
		puts("Press a key to exit()\n\r");
		return EXIT_FAILURE;
	}
	else {
		puts("The file was opened.\n\r");
	}

	// Open file for writing to (XYZ) -----------------------------------------------------------------------------------
	if ((csvPtr = fopen("C:\\Users\\ronny.wong\\OneDrive\\Geotech\\_dataFiles\\gt_out_all_2.xyz", "w+")) == NULL) {
		puts("Was not able to create XYZ file for exporting\n\r");
		puts("Press a key to exit()\n\r");
		return EXIT_FAILURE;
	}
	else {
		puts("The xyz file was opened.\n\r");
	}
	// Set-up Header information and file details
	fseek(readPtr, 0L, SEEK_END);		// Check end of file position and calculate size
	filesize = ftell(readPtr);
	fprintf(csvPtr, "GT Ltd. - MAG DATA COLLECTOR\r");
	fprintf(csvPtr, "R&D\r\n");
	fprintf(csvPtr, "Imported File Size: %ld [Bytes]\n\r", filesize);
	fseek(readPtr, 0L, SEEK_SET);		// Reset pointer to beginning of file
	fprintf(csvPtr, "DP1 DP2 DP3 DP4 Time Count\n");
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
	
	// Close FILE I/O handles
	// fclose returns 0 or NULL is successful
	puts("Closing FILE handles...\n\r");
	if ((fclose(geosoftio) == NULL)) {			
		puts("Successfully closed Geosoft XYZ Import Template file.\r\n");
	}
	else {
		puts("Cannot close IO handle for Geosoft XYZ Import Template file.\n\r");
	}
	if ((fclose(csvPtr) == NULL)) {
		puts("Successfully closed CSV file handle.\n\r");
	}
	else {
		puts("Cannot close FILE handle for CSV file.\n\r");
	}
	if ((fclose(readPtr) == NULL)) {
		puts("Successfully closed data file handle.\r\n");
	}
	else {
		puts("Cannot close FILE handle for data file.\n\r");
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