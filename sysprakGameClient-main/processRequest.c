#include "processRequest.h"

/**
 *Vergleicht einen String mit einem Regular Expression Pattern.
 *
 *@param string Der String der vergleichen werden soll
 *@param pattern Das Pattern das mit dem String verglichen werden soll
 */
int match(const char *string, char *pattern){
    int    status;                                                              //Variable in dem das Ergebnis des Vergleichs gespeichert wird
    regex_t    re;                                                              //Die Regular Expression

    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {                   //Regular Expression mit dem Pattern wird compiliert
        return(0);                                                              //Falls falsches Pattern angegeben wurde wird 0 zurückgegeben
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);                         //Führt die Regular Expression auf den String aus
    regfree(&re);                                                               //Regular Expression wieder freigeben
    if (status != 0) {                                                          //Falls pattern nicht passt
        return(0);                                                              //gebe 0 zurück
    }
    return(1);                                                                  //Falls das Pattern passt gebe 1 zurück
}

/**
 *Die Funktion substring extrahiert einen Teilstring von from bis to aus einem
 *gegebenen String und gibt ihn als char Pointer zurück.
 *@param string Der String aus dem der Teilsting extrahiert werden soll
 *@param from Anfangsposition des Teilstrings als int
 *@param to Endposition des Teilstrings als int
 *@return Teilstring von from bis to als char* oder NULL falls ein Fehler auftritt.
 */

char *substring(char *string, unsigned int from, unsigned int to){
  if(from>=to || to>strlen(string)){                                            //Unsinnige Eingaben abfangen
    return NULL;                                                                //NULL zurückgeben falls
  }else{                                                                        //Falls sinbvolle Eingabe
    char *substring = malloc((to-from)*sizeof(char)+1);                           //Speicher füresponse substring allokalisieren
    strncpy(substring,string+from,(to-from));                                   //Teilstring in substring speichern
    substring[(to-from)]= '\0';                                                 //Teilstringende festlegen
    return substring;                                                           //Substring zurückgeben
  }
}

/**
 *Die Funktion strtoken zerteilt einen String in mehrere durch den Seperator
 *getrennte Teilstrings und speichert diese ein einem array.
 *
 *@param str Der String der in Teilstrings zerlegt werden soll
 *@param seperator Der Seperator nach dem zerlegt werden soll
 *@param token Das String Array in dem die Teilstrings gespeichert werden
 *@return Anzahl der Teilstrings als int
 */
int strtoken(char *str, char *separator, char *token[]){
  int i = 0;                                                                    //Zählvariable/Laufvariable für die Anzahl
  token[0] = strtok(str, separator);                                            //strtok initialisieren und ersten Eintrag in das String Array
  while (token[i]){                                                             //Solange der vorherige Eintrag nicht NULL ist
    i++;                                                                        //Zählvariable hochzählen
    token[i] = strtok(NULL, separator);                                         //Da strtok schon initialisiert wurde, kann man mit NULL den restlichen Teilstring zerlegen, speichert ihn im Array
  }
  return i;                                                                     //Gibt Anzahl zurück
}