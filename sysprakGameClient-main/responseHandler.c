#include "responseHandler.h"

int prolog = 1;  //! Variable für den Fortschritt der Prologphase.
int command = 0; // Flag, das nach den verschiedenen Befehlen gesetzt wird
int playercount = 0;
int gameover = 0;
int fieldcount = 0;

char *handle(char *request)
{
  char *response;
  char *out;

  if ((response = malloc(BUFFERLENGTH * sizeof(char))) == NULL)
  {
    perror("Not enough Memory for variables");
  }
  if ((out = malloc(BUFFERLENGTH * sizeof(char))) == NULL)
  {
    perror("Not enough Memory for variables");
  }
  if (fieldcount!=0)
  {
    int i=atoi(substring(request, 0, 1)); // request[0]
    strcpy(tmp,substring(request,2,strlen(request)));
    saveasbinary(tmp,i);
    fieldcount--;
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
    if (out != NULL)
    {
      free(out);
    }
    out = NULL;
  }
  else if (prolog == 1 && match(request,"MNM Gameserver .+accepting connections"))
  {
    strcpy(response, "VERSION ");
    strcat(response, VERSION);
    // Format
    strcpy(out, "The MNM Gameserver version "); // Ausgabe The MNM Gameserver <Versionsnummer> accepted the connection
    char *version = substring(request, 16, strlen(request) - 22);
    strcat(out, version); // Versionsnummer
    strcat(out, " accepted the connection.");
    if (version != NULL)
    {
      free(version);
    }
    prolog++;
  }
  else if (prolog == 2 && match(request,
                                "Client version accepted - please send Game-ID to join"))
  {
    strcpy(response, "ID ");  // Setze response auf die passende Antwort
    strcat(response, gameid); // Game-ID von client.c übernehmen
    // Format
    strcpy(out, "The clients version was accepted by the Gameserver. Please send a valid Game-ID to join the game.");
    prolog++;
  }
  else if (prolog == 3 && match(request,
                                "PLAYING .+"))
  {
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
    // Format
    strcpy(out, "Preparing to play the game ");
    char *gamekind = substring(request, 8, strlen(request) - 1);
    if (strcmp(gamekind, GAMEKINDNAME) != 0)
    {
      perror("Our Client can only play Quarto");
      exit(EXIT_FAILURE);
    }
    strcat(out, gamekind);
    if (gamekind != NULL)
    {
      free(gamekind);
    }
    prolog++;
  }
  else if (prolog == 4 && match(request,
                                ".+"))
  {
    strcpy(response, "PLAYER ");
    strcat(response, player);
    // Format
    strcpy(out, "The games name is ");
    strcpy(serverinfo->gamename, request);
    strcat(out, request);
    prolog++;
  }
  else if (prolog == 5 && match(request, "YOU .+ .+"))
  {
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
    // Format
    char *player = substring(request, 6, 14);
    strcpy(serverinfo->clientname, player);
    strcpy(out, player);
    strcat(out, " you are player number ");
    char *splayernumber = substring(request, 4, 5);
    serverinfo->clientplayernr = atoi(splayernumber);
    strcat(out, splayernumber);
    free(player);
    free(splayernumber);
    prolog++;
  }
  else if (prolog == 6 && match(request, "TOTAL .+"))
  {
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
    // Format
    char *stotalplayers = substring(request, 6, 7);
    // printf("%c\n", *stotalplayers); --->2 passt
    serverinfo->totalplayers = atoi(stotalplayers);
    // printf("%i\n", serverinfo->totalplayers); ---->2 passt
    strcpy(out, stotalplayers);
    if (atoi(stotalplayers) < 2)
    {
      strcat(out, " player will take part in this game.");
    }
    else
    {
      strcat(out, " players will take part in this game.");
    }

    for (int i = 0; i < serverinfo->totalplayers - 1; i++)
    {
      shmid_player[i] = createSHM(sizeof(struct player));       // Shared Memory Segment für jeden Spieler erstellen
      serverinfo->otherplayers[i] = attachSHM(shmid_player[i]); // Shared Memory Segment anbinden
    }

    free(stotalplayers);
    prolog++;
  }
  else if (match(request, "ENDPLAYERS"))
  {
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
    kill(serverinfo->pid_thinker, SIGUSR2);

    // Format
    strcpy(out, "Starting the game..."); // Ausgabe ENDPLAYERS - The prolog is finished!
  }
  else if (prolog >= 7 && match(request, ".+ .+ .+"))
  {
    if (response != NULL)
    {
      free(response);
    }
    response = NULL; // Setze response auf die passende Antwort
    // Format
    char *rplayer = substring(request, 2, strlen(request) - 2); // Ausgabe <Player> (<Playernumber>) is ready/ not ready
    strcpy(serverinfo->otherplayers[playercount]->playername, rplayer);
    strcpy(out, rplayer);
    strcat(out, " (");
    char *rplayernumb = substring(request, 0, 1);
    serverinfo->otherplayers[playercount]->playernr = atoi(rplayernumb);
    strcat(out, rplayernumb);
    strcat(out, ") is ");
    char *playerstatus = substring(request, strlen(request) - 1, strlen(request));
    serverinfo->otherplayers[playercount]->ready = atoi(playerstatus);
    if (atoi(playerstatus) == 1)
    { // Spieler bereit -> letzte Zahl = 1
      strcat(out, "ready");
    }
    else if (atoi(playerstatus) == 0)
    { // Spieler nicht bereit -> letzte Zahl = 0
      strcat(out, "not ready");
    }

    free(rplayer);
    free(rplayernumb);
    free(playerstatus);
    prolog++;
    playercount++;
  }
  else if (match(request, "WAIT"))
  { // Wenn Anfrage des Servers übereinstimmt
    strcpy(response, "OKWAIT");
    strcpy(out, "Wait");
  }
  // NEXT korrekt implementiert
  else if (match(request, "NEXT .+"))
  {
    // strcpy(out, substring(request, 0, strlen(request)));
    strcpy(serverinfo->next, substring(request, 5, 7)); // Nächster Spielstein in der shm Bereich
    strcpy(out, "The next piece to be placed is: ");
    decimaltobinary(*serverinfo->next);
    strcpy(serverinfo->next,binary);
    strcat(out, serverinfo->next);
    strcpy(response, "OK");
  }
  // FIELD
  else if (match(request, "FIELD .+")) // FELD MUSS DYNAMISCH ÄNDERBAR SEIN
  {
    fieldcount = 4;
    serverinfo->width = atoi(substring(request, 6, 7));
    serverinfo->heigth = atoi(substring(request, 6, 7)); // Da feld quadratisch
    strcpy(out, "Our field has the width and heigth of: ");
    strcat(out, substring(request, 6, 7));
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
  }
  else if (match(request, "MOVE .+")) // HIER GIBT ES KEINE AUSGABE OBWOHL OUT GEFÜLLT WIRD - CHECK PRINTF
  {
    strcpy(out, "You have");
    strcat(out, substring(request, 4, 9));
    strcat(out, " seconds to make your move");
    command = 1;
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
  }
  else if (match(request, "MOVEOK"))
  { //Übereinstimmung mit MOVE
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
    strcpy(out, "Valid move");
  }
  else if (match(request, "OKTHINK"))
  {
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
    rdy = 1;
    strcpy(out, "Make a move");
  }
  else if (match(request, "ENDFIELD") && command == 1)
  { // Wenn Anfrage des Servers übereinstimmt
    fieldcount=0;
    if (!gameover)
    {
      strcpy(response, "THINKING");
      serverinfo->startcalc = 1;
      kill(serverinfo->pid_thinker, SIGUSR1);
    }
    else
    {
      printfield();
      if (response != NULL)
      {
        free(response);
      }
      response = NULL;
    }

    if (out != NULL)
    {
      free(out);
    }
    out = NULL;
  }
  else if (match(request, "PLAYER0WON .+"))
  { // Hat Spieler 0 gewonnen
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
    if (out != NULL)
    {
      free(out);
    }
    out = NULL;
    char *wonzero = substring(request, 11, strlen(request));
    if (!strcmp(wonzero, "Yes"))
    {
      won0 = 1;
    }
    else
    {
      won0 = 0;
    }
    free(wonzero);
  }
  else if (match(request, "PLAYER1WON .+"))
  { // Hat Spieler 1 gewonnen
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
    if (out != NULL)
    {
      free(out);
    }
    out = NULL;

    char *wonone = substring(request, 11, strlen(request));

    if (!strcmp(wonone, "Yes"))
    {
      won1 = 1;
    }
    else
    {
      won1 = 0;
    }

    free(wonone);
  }
  else if (match(request, "GAMEOVER"))
  {
    gameover = 1;
    printf("\n///////////////////////////////////////////////\n");
    printf("///////////////////////////////////////////////\n");
    printf("///                GAME OVER                ///\n");
    printf("///////////////////////////////////////////////\n");
    if (out != NULL)
    {
      free(out);
    }
    out = NULL;
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
  }
  else if (match(request, "QUIT"))
  { // Ausgabe Gewinner
    if (response != NULL)
    {
      free(response);
    }
    response = NULL;
    if (out != NULL)
    {
      free(out);
    }
    out = NULL;

    printf("///////////////////////////////////////////////\n");
    if (won0 && !won1)
    {
      printf("///              Player 0 won!              ///\n");
      printf("///////////////////////////////////////////////\n");
    }
    else if (!won0 && won1)
    {
      printf("///              Player 1 won!              ///\n");
      printf("///////////////////////////////////////////////\n");
    }
    else
    {
      printf("///         The Game ended in a draw        ///\n");
      printf("///////////////////////////////////////////////\n");
    }
  }
  else
  { // Ansonsten unbekannte Anfrage des Servers
    if (response != NULL)
    {
      free(response);
    }
    response = NULL; // Setze Antwort auf "NULL"
    if (out != NULL)
    {
      free(out);
    }
    out = NULL;
  }

  if (response != NULL)
  {
    strcat(response, "\n");
  }
  if (out != NULL)
  {
    printf("server: %s\n", out);
    free(out);
  }

  return response;
}
