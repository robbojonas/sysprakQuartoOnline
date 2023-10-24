#include "performConnection.h"

void performConnection(int sock)
{
  end = 1;
  rdy = 0;
  do
  {

    FD_ZERO(&readfds);       // Macht das Set frei
    FD_SET(sock, &readfds);  // Fügt dem Set den Socket hinzu (die Gameserververbindung)
    FD_SET(fd[0], &readfds); // Fügt dem Set die Pipe hinzu (Leseseite!)

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    // Selectmethode, die aus dem Filedeskriptorset im festgelegten Zeitintervall überprüft, ob Daten anstehen
    retval = select(sizeof(readfds) * 2, &readfds, NULL, NULL, &tv);
    if (retval == -1)
    {
      perror("select()");
      exit(EXIT_FAILURE);
    }
    else if (retval)
    {
      pipeData = FD_ISSET(fd[0], &readfds); // ISSET testet, ob an DIESER PIPE etwas ansteht
      socketData = FD_ISSET(sock, &readfds);

      if (socketData != 0)
      {

        // char *buffer = calloc(BUFFERLENGTH, sizeof(char)); // Speicher für Puffervariable allokalisieren-- malloc besser
        char *buffer = malloc(BUFFERLENGTH * sizeof(char));
        int i = 0;
        do
        {
          if (recv(sock, &buffer[i], 1, 0) < 0)
          {
            perror("Host is not responding");
          }

          i++;
        } while (buffer[i - 1] != '\n');
        // buffer[i] = '\0'; -- ist dabei
        /*
        if ((read(sock, buffer, BUFFERLENGTH)) < 0)
        {
          perror("Host is not responding");
        }
    */
        processAndSendResponse(buffer, sock);
      }

      if (pipeData != 0 && rdy)
      { // Wenn etwas ansteht, dann..Aus der Pipe lesen
        char *move = calloc(BUFFERLENGTH_MOVE, sizeof(char));
        if ((read(fd[0], move, BUFFERLENGTH_MOVE)) < 0)
        {
          perror("Couldn't read from pipe");
        }
        else
        {
          sendMove(move, sock);
          rdy = 0;
        }
      }
    }
  } while (end); // Nehme solange Antworten vom Server entgegen bis Server "+ ENDPLAYERS" antwortet
}

void sendMove(char *move, int sock)
{
  char *response = calloc(BUFFERLENGTH_MOVE + strlen(move), sizeof(char));

  strcpy(response, "PLAY ");
  strcat(response, move);
  strcat(response, "\n");
  sendResponse(response, sock);
  free(move);
}

void processAndSendResponse(char *buffer, int sock)
{
  // printf("%s\n", buffer);
  //  ggf einfacher pointer, nicht double
  //  char *requests = calloc(BUFFERLENGTH, sizeof(char *));

  // strtoken(buffer, "\n", requests);

  int x = 0; // Variable für mehrere Anfragen
             /* do
              {*/
  end = !match(buffer + 2, "QUIT");
  if (buffer[0] == '+')
  {
    if (strlen(buffer) > 2)
    {
      char *response = handle(buffer + 2);
      sendResponse(response, sock);
    }
  }
  else if (buffer[0] == '-')
  {
    printf("server: Error! %s\nDisconnecting server...\n",
           buffer + 2);
    exit(EXIT_FAILURE);
  }
  x++;
  /*
    } while (requests[x] != NULL && end);*/

  if (buffer != NULL)
  {
    free(buffer);
  }
}

void sendResponse(char *response, int sock)
{
  if (response != NULL)
  {
    send(sock, response, strlen(response), 0);
    printf("client: %s", response);
  }
  if (response != NULL)
  {
    free(response);
  }
}
