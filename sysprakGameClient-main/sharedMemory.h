#ifndef sharedMem
#define sharedMem

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "parameter.h"

// Konstanten
#define PERMISSION 0644 // Schreib/Leserechte für User, Leserechte für Gruppen
#define KEY IPC_PRIVATE

struct serverinfo
{
  char gamename[BUFFERLENGTH];
  int clientplayernr;
  char clientname[BUFFERLENGTH];
  int totalplayers;
  pid_t pid_connector;
  pid_t pid_thinker;
  struct player *otherplayers[BUFFERLENGTH];
  char field[ROWS][COLUMNS][BUFFERLENGTH]; // Feld gespeichert
  int startcalc;
  char next[BUFFERLENGTH_SMALL]; // Speicher nächsten Spielstein
  int heigth; // Spielfeld höhe
  int width;  // Spielfeld breite -- denn das ganze muss dynamisch änderbar sein
};

struct player
{
  int playernr;
  char playername[BUFFERLENGTH];
  int ready;
};

int createSHM(size_t size);
void *attachSHM(int shmid);
void detachSHM(void *shm);
int deleteSHM(int shmid);

#endif
