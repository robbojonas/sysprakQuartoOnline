#include "config.h"

/**
 *Diese Funktion liest aus der Konfigurations Datei die nötigen Parameterwerte aus und gibt sie als String zurück.
 *@param configFile Die Konfigurations Datei als Pfad
 *@param paramName als char*, der Name des Parameters
*/
char* readConfiguration(char* paramName){
   char *configFile = confile;
   FILE *file = NULL;                                                           //Die gewählte Konfig Datei vom Typ FILE
   char string[BUFFERLENGTH] = {0};                                             //Zeilenweise Strings, die aus der Konfig Datei gelesen werden
   char *delimiter = "= \n";                                                    //Tokens, die die Strings in verschiedene Abschnitte teilen
   char *substring = NULL;                                                      //Substrings, die wir rauslesen wollen

   file = fopen(configFile, "r");                                               //Öffnen der Konfigdatei
   if(file != NULL){                                                            //Abfrage, ob sich die Datei öffnen lässt
     while(fgets(string,BUFFERLENGTH,file)) {                                   //Einzelne zeilenweise Abfrage der Datei
       if(strstr(string, paramName) != NULL){                                   //Filtern nach der Zeile, die den "Hostname" enthält
          substring = findParamValue(string, delimiter);             //Funktionsaufruf, der den zugehörigen ParameterWert zurückgibt
          break;
        }

     }
   }else{                                                                       //Configfile konnte nicht geöffnet werden
     perror("Could not open the specified config file. Please make sure the file exists in the specified directory.");
     exit(EXIT_FAILURE);
   }

   fclose (file);                                                               //Schliessen der Konfig Datei
   char *res = calloc(BUFFERLENGTH, sizeof(char));
   strcpy(res, substring);
   return res;
}

/**
 *Der Pointer zerteilt nach dem Token den String in Abschnitte und gibt den letzten Abschnitt (den Parameterwert) zurück.
 *@Param string als char* ist der zu lesende String
 *@Param delimiter als char* ist das Token, nachdem der String in Abschnitte geteilt wird
 *@Param substring als **char ist der Substring, der aus dem String gesucht wird
*/
char* findParamValue(char *string, char *delimiter){
    char *ptr = NULL;                                                                  //Pointer
    char *res = NULL;
    ptr = strtok(string, delimiter);                                            //Zeigt auf das erste Token
    while(ptr != NULL){                                                         //Während der Pointer noch nicht am Ende ist
      res = ptr;
      ptr = strtok(NULL, delimiter);                                            //Der Pointer geht eins weiter
    }
    return res;
}

/**
 *Erstellt eine Default Config wenn noch keine existiert.
 */
void createDefaultConfig(void){
  FILE *file = NULL;
  file = fopen(confile, "r");                                                   //Config versuchen zu öffnen NULL falls nicht existent
  if(file == NULL) {
    printf("Creating default config...\n");                                     //Falls nicht existent, erstelle default config
    file = fopen(confile, "w");                                                 //Erstele file und öffne im Schreibmodus
    if(file == NULL) {                                                          //Überprüfen ob auf die Datei zugegriffen werden kann
  	   perror("No permission to create default config");
    }else {
      fprintf(file, "hostname=%s\nportnumber=%i\ngamekindname=%s",              //Inhalt der Default Config reinschreiben
      HOSTNAME,PORTNUMBER,GAMEKINDNAME);
      fclose(file);                                                             //File schließen
    }
  }else{
    fclose(file);                                                               //File schließen
  }
}