#ifndef responseHdl
#define responseHdl

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "processRequest.h"
#include "parameter.h"
#include "sharedMemory.h"
#include "field.h"

extern int fd[2];
extern struct serverinfo *serverinfo;
extern int *shmid_player;
extern char *gameid; // client.c
extern char *player; // client.c
extern int rdy;
int prolog;
int command; // Flag für den Befehl MOVE
int won0;    // Flag -hat player 1 gewonnen
int won1;
char tmp[BUFFERLENGTH]; // temporäres array für die ganzen field informationen
char *handle(char *request);

#endif
