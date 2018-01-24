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
#define n0			-2
#define n1			-1
#define n2			0
#define n3			1
#define ADCCONVTIME	20E-9			// 20 nanoseconds

// Function Protoypes
void swap(char *firstElem, char *secondElem);

int main()
{
	unsigned char buffer[BUFLEN];
	FILE *ptr;
	DWORD data[108] = { 0 };
	int wrapSize = 4;

	long long int temp_num;
	long long int temp_denom;
	long long int zcrossing
		
		;
	long long int time;

	long long int y0, y1, y2, y3;
	int zindex = 3;

	float product;

	// C:\Users\nocturnalhippo\Desktop
	// if ((ptr = fopen("E:\\WriteBinaryData.bin", "rb")) == NULL) {
	if ((ptr = fopen("C:\\Users\\nocturnalhippo\\Desktop\\WriteBinaryData.bin", "rb")) == NULL) {
		printf("The file was not opened\n\r");
		printf("Press a key to exit()\n\r");
		return 1;
	}
	else{
		printf("The file was opened.\n\r");
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

	// ZC calculation:
	// 32-bit signed values are stored in data[track]
	printf("The size of data is: %d\n\r", sizeof(data));
	getchar();
	temp_num = 0;
	temp_denom = 0;
	y0 = (data[0]);			// A: x1
	y1 = (data[1]);			// B: x2
	y2 = (data[2]);			// C: x3
	y3 = (data[3]);			// D: x4
	time = (data[4]);
	
	temp_num = (-2 * y0) + (5 * y1) + (-4 * y2) + (y3);
	temp_denom = (3 * y1) + (-y0) + (-3 * y2) + (y3);

	product = ADCCONVTIME * ((float)temp_num / (float)temp_denom);

	printf("ADCCONVTIME: %0.13f\n\r", ADCCONVTIME);
	getchar();
	printf("product_1: %0.15f\n\r", product);
	getchar();

	float zc1 = (time*ADCCONVTIME) + product;
	printf("zc1: %0.15f\n\r", zc1);
	getchar();

	y0 = data[102];
	y1 = data[103];
	y2 = data[104];
	y3 = data[105];
	time = data[106];

	printf("%lld %lld %lld %lld %lld\n\r", y0, y1, y2, y3, time);
	getchar();

	temp_num = (-2 * y0) + (5 * y1) + (-4 * y2) + (y3);
	temp_denom = (3 * y1) + (-y0) + (-3 * y2) + (y3);

	product = ADCCONVTIME * ((float)temp_num / (float)temp_denom);

	printf("ADCCONVTIME: %0.13f\n\r", ADCCONVTIME);
	getchar();
	printf("product_1: %0.15f\n\r", product);
	getchar();

	float zc2 = (time*ADCCONVTIME) + product;
	printf("zc2: %0.15f\n\r", zc2);
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