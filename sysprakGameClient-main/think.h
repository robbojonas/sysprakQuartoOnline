#ifndef think
#define think

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "parameter.h"
#include "sharedMemory.h"
#include "responseHandler.h"
#include "field.h"

extern int fd[2];
extern struct serverinfo *serverinfo;

typedef struct
{
    char move[BUFFERLENGTH_MOVE];
    int weitererMember;
} move;

typedef struct
{
    move maves[BUFFERLENGTH_MOVE];
    int count;
} movearray;

void think_(int sig);

// weitere Methoden 2do

#endif
