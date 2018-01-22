// dataHandler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

#define BUFLEN	512

int main()
{
	unsigned char buffer[BUFLEN];
	FILE *ptr;

	if ((ptr = fopen("I:\\WriteBinaryData", "rb")) == NULL)
		printf("The file was not opened\n\r");
	else{
		printf("The file was opened.\n\r");
		printf("Press a key to exit()\n\r");
	}

	fread(buffer, sizeof(buffer), 1, ptr);

	for (int i = 0; i < BUFLEN; i++) {
		printf("%02x ", buffer[i]);
	}

	printf("\r\n");
	printf("Press any key to continue()\n\r");
	getchar();

    return 0;
}

