#include "client.h"

int main (int argc, char **argv){
  confile = NULL;
  gameid = NULL;
  player = NULL;
  gameid = NULL;
  player = NULL;
  fflag = 0;                                                                    //Flags für die optionalen Kommandozeilenargumente überprüfen, ob ein Argument für Player oder die KonfigDatei angegeben wurde
  pflag = 0;
  pid_t pid = 0;
  shmid_serverinfo = createSHM(sizeof(struct serverinfo));                      //Shared Memory erstellen, für das Serverinformationen struct
  shmid_shmid_player = createSHM(BUFFERLENGTH*sizeof(int));                     //Shared Memory erstellen, in diesem Segment werden die shmids der einzelnen Players
  signal(SIGUSR1, think_);
  signal(SIGUSR2, attachPlayers);
  if(argc<2){                                                                   //Test: Wird eine Game-ID übergeben
    perror("No game id");                                                       //Keine Game-ID vorhanden
    exit(EXIT_FAILURE);                                                         //Programm beenden
  }else if (strlen(argv[2])!=13){                                               //Ist die Game-ID 13-stellig
      perror("Invalid game id");                                                //Game-ID zu kurz oder zu lang
      exit(EXIT_FAILURE);                                                       //Programm beenden
  }else{                                                                        //13-stellige Game-ID übergeben
     gameid=argv[2];                                                            //kopieren des Strings nach gameid
     while((c = getopt(argc, argv, ":p:f:")) != -1){                            //(optind = Index des Arguments) iteriert durch alle Argumente
       switch(c){
         case 'p': if(strcmp(optarg,"")!=0){
                     player = optarg;                                           //p-flag: Wert des Players wird aus der Kommandozeile übernommen
                     pflag = 1;                                                 //pflag signalisiert, dass ein Player angegeben wurde
                   }
                   break;
         case 'f': if(strcmp(optarg,"")!=0){
                     confile = optarg;                                          //f-flag: Pfad der Konfigdatei wird aus der Kommandozeile übernommen
                     fflag = 1;                                                 //fflag signalisiert, dass eine Konfigdatei angegeben wurde
                     printf("The specified confile is %s.\n", confile);
                   }
                   break;
         /*case 'g': if(strcmp(optarg,"")!=0){
                     gameid = optarg;
                     gflag = 1;
                     printf("The gameid is %s. \n", gameid);
                   }*/          
         default:
                   break;
      }
    }

    if(pflag == 0){                                                             //Wenn kein Player angegeben wurde
      player = "";
      printf("No Player specified\n");
    }
    if(fflag == 0){                                                             //Wenn keine Konfigdatei angegeben wurde
      printf("No configuration file specified\n");
      confile = "client.conf";                                                  //Default Konfigdatei ist client.conf
      createDefaultConfig();
      printf("Using default configuration file \"%s\"\n",confile);
    }

    //Struct befüllen mit den Werten zur Verbindung mit dem Server
    char *bufHost = readConfiguration(paramNameHost);
    strcpy(cp.hostName,bufHost);
    free(bufHost);

    char *bufGameKind = readConfiguration(paramNameGame);
    strcpy(cp.gameKindName,bufGameKind);
    free(bufGameKind);

    char *bufPort = readConfiguration(paramNamePort);
    char bufPort2[BUFFERLENGTH_PORT];
    strcpy(bufPort2, bufPort);
    free(bufPort);
    cp.portNumber = atoi(bufPort2);
  }

  //Unnamed Pipe einrichten, die zwischen Connector und Thinker laufen soll
  if(pipe(fd) < 0){
      perror("Error establishing an unnamed pipe");
      exit(EXIT_FAILURE);
  }

  if((pid=fork())<0){                                                           //Aufsplitten des Prozesses
    perror("Error while splitting the process");                                //Fehler bei fork
    exit(EXIT_FAILURE);
  } else if(pid == 0){                                                          //Kindprozess: Prozess-ID == 0
      //CONNECTOR
      atexit(exit_handler);
      int sock;

      //Schreibeseite der Pipe schließen
      close(fd[1]);

      //Shared Memory Segmente anbinden
      serverinfo = attachSHM(shmid_serverinfo);
      shmid_player = attachSHM(shmid_shmid_player);

      sock = connectServer(cp.portNumber, cp.hostName);                         //Aufruf connectServer
      performConnection(sock);

      //Schliesst das Socket
      close(fd[0]);
      close(sock);
  } else {                                                                      //Elternprozess: Prozess-ID > 0
    //THINKER
    //Leseseite der Pipe schließen
    close(fd[0]);

    //Shared Memory Segmente anbinden
    serverinfo = attachSHM(shmid_serverinfo);
    shmid_player = attachSHM(shmid_shmid_player);

    serverinfo->pid_thinker = getpid();                                         //pid vom Parent im struct speichern
    serverinfo->pid_connector = pid;                                            //pid vom Child im struct speichern

    if(waitpid(pid,NULL,0) != pid){                                             //Warten bis der Kindprozess terminiert
      perror("Error while waiting for childprocess");
      exit(EXIT_FAILURE);
    }
    close(fd[1]);
  }
  exit(EXIT_SUCCESS);
}

static void exit_handler(void){
      //Löschen der Shared Memory Segmente
      if(shmid_player != 0){
        for(int i = 0; i<serverinfo->totalplayers-1; i++){
          if(shmid_player[i]!=0)
            shmid_player[i] = deleteSHM(shmid_player[i]);
        }
      }
      //Löschen der Shared Memory Segmente
      if(shmid_shmid_player!=0)
        shmid_shmid_player = deleteSHM(shmid_shmid_player);

      if(shmid_serverinfo!=0)
        shmid_serverinfo = deleteSHM(shmid_serverinfo);
}

void attachPlayers(int sig){
  (void)sig;
  for(int i = 0; i<serverinfo->totalplayers-1; i++){                            //Shared Memory Segment jedes Spielers attachen und im struct speichern
    serverinfo->otherplayers[i] = attachSHM(shmid_player[i]);
  }
}