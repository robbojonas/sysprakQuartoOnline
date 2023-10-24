#ifndef processReq                                                              //Wenn das Makro noch nicht existiert
#define processReq                                                              //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

//Funktionen deklarieren
int match(const char *string, char *pattern);                                   //Vergleicht einen String mit einem Regex Pattern
char *substring(char *string, unsigned int from, unsigned int to);              //Extrahiert einen Substring aus einem String
int strtoken(char *str, char *separator, char *token[]);                        //Zerlegt einen String anhand eines Seperators

#endif