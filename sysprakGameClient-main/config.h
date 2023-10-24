#ifndef konfigDatei                                                             //Wenn das Makro noch nicht existiert
#define konfigDatei                                                             //Makro definieren

//Bibliotheken einbinden
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//Module einbinden
#include "parameter.h"

//Struct definieren
typedef struct{
    char hostName[BUFFERLENGTH];
    int portNumber;
    char gameKindName[BUFFERLENGTH];
}configparams;

//Externe Variablen deklarieren
extern char *confile;

//Funktionen definieren
char* readConfiguration(char *paramName);
char* findParamValue(char *string, char *delimiter);
void createDefaultConfig(void);

#endif