#ifndef client                                                                  //Wenn das Makro noch nicht existiert
#define client                                                                  //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

//Module einbinden
#include "performConnection.h"
#include "connectServer.h"
#include "config.h"
#include "parameter.h"
#include "sharedMemory.h"
#include "think.h"
#include "responseHandler.h"

//Globale Variablen deklarieren
char *gameid;                                                                   //Globale Variable für die Game-ID deklarieren
char *player;                                                                   //Gloabel Variable für die PlayerNr deklarieren
char *paramNameHost = "hostname";                                               //Parametername des Hostnamens, nachdem wir filtern
char *paramNamePort = "portnumber";                                             //Parametername der Portnummer, nach der wir filtern
char *paramNameGame = "gamekindname";                                           //Parametername des Spiels, nach dem wir filtern
char *value = "";
configparams cp;                                                                //Eine Instanz vom Struct configparams namens "cp"
char *confile;                                                                  //Speichert den Pfad der Konfigurationsfile, die geöffnet werden soll
int pflag;                                                                      //Setzt ein Flag, wenn Player angegeben wurde
int fflag;                                                                      //Setzt ein Flag, wenn Konfigdatei angegeben wurde
int c;                                                                          //RetValue von getopt. Entweder Argument oder -1, wenn kein Argument vorhanden
int *shmid_player;                                                              //Gobales Int Array für die ShmIDs der Player Segmente
int shmid_serverinfo;
int shmid_shmid_player;
struct serverinfo *serverinfo;                                                  //Gobales struct für die Serverinfos
int fd[2];

//Funktionen deklarieren
static void exit_handler(void);
void attachPlayers(int sig);

#endif