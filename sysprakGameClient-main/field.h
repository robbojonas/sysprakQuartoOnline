#ifndef fieldutil
#define fieldutil

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

//Module einbinden
#include "parameter.h"
#include "sharedMemory.h"
#include "responseHandler.h"
#include "processRequest.h"

//Funktionen deklarieren
int converter(int decimal);
void saveasbinary(char str[], int i);
void printfield();
char* decimaltobinary(char ptr);

//Variablen
int i;
int decimal;
int bin;
int base;
char binary[4];
char text[4];
char *ptr;

#endif