#include "field.h"
int converter(int decimal){
  bin=0;
  int rest;
  int factor=1;
  while(decimal!=0){
    rest=decimal%2;
    decimal/=2;
    bin+=rest*factor;
    factor*=10;
  }
  return bin;
}
char* decimaltobinary(char ptr){
  int decimal=atoi(&ptr);
  strcpy(binary,"");
    converter(decimal);
    sprintf(text,"%d",bin);
    for(int n=1;n<=4-(int)strlen(text);n++){
      strcat(binary,"0");
    }
    strcat(binary,text);
    return binary;
}
void saveasbinary(char tmp[],int i){
	char delim[] = " ";

	char *ptr = strtok(tmp, delim);

	while(ptr != NULL)
	{
    for(int j=0;j<COLUMNS;j++){
      if('*'==ptr[0]){
        memcpy(serverinfo->field[4-i][j],"****",5);
      }else{
        decimaltobinary(*ptr);
	      memcpy(serverinfo->field[4-i][j],binary,4);
      }
	    ptr = strtok(NULL, delim);  
    }
	}
}
/**
 *Gibt das aktuelle Spielfeld mit Informationen aus dem Shared Memory aus.
 *Ein kompletter String um Unterbrechungen zu vermeiden.
 */
void printfield(){
  char *square = calloc(BUFFERLENGTH_FIELD,sizeof(char));
  char *buf = calloc(BUFFERLENGTH_SMALL,sizeof(char));
  strcat(square, "\n");
  strcat(square,"     A    B    C    D\n");                                                                //Spielfeld Beschriftung oben
  strcat(square,"  +---------------------+\n");
  for(int i=0; i<ROWS; i++){
    sprintf(buf," %i|", 4-i); 
    strcat(square,buf);   
    strcat(square," ");                                                                      //Spielfeld-Nummerierung links
    for(int j = 0; j<COLUMNS; j++){
      strcat(square,serverinfo->field[i][j]);
      strcat(square," ");
    }
    sprintf(buf,"|%i\n", 4-i); 
    strcat(square,buf);                                                                       //Spielfeld-Nummerierung rechts
  }
	strcat(square,"  +---------------------+\n");
  strcat(square,"     A    B    C    D\n");                                                                //Spielfeld Beschriftung unten
  strcat(square,"\n");
  printf("%s",square);
  free(square);
}
