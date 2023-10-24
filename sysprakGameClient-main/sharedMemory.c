#include "sharedMemory.h"

/**
 * Shared Memory Bereich erstellen.
 * @param size Speichergröße des zu erstellenden Shared Memory
 * @return ID des Shared Memory Segments
 */
int createSHM(size_t size){
  int shmid = -1;                                                               //Variable für die shm ID
  if((shmid = shmget(KEY, size, PERMISSION)) == -1){                            //Shared Memory erstellen
      perror("Error while creating shared memory segment");                     //Fehlerbehandlung falls Fehler bei Erstellung
      exit(EXIT_FAILURE);
  }
  return shmid;                                                                 //shmid zurückgeben
}

/**
 * Bindung zum Shared Memory herstellen
 * @param shmid ID des Shared Memory Segments
 * @return Pointer zum Speicherbereich
 */
void *attachSHM(int shmid){
  void *shm = shmat(shmid, 0, 0);                                               //Shared Memory anbinden
  if(shm == NULL){                                                              //Fehlerbehandlung falls shmat fehlschlägt
    perror("Error while attching shared memory segment");
    exit(EXIT_FAILURE);
  }
  return shm;                                                                   //Pointer auf das shm Segment zurückgeben
}

/**
 * Bindung zum einem Shared Memory Segment lösen
 * @param shm Pointer auf das zu lösende Shared Memory Segement
 */
void detachSHM(void *shm){
  if((shmdt(shm)) == -1){                                                       //Shared Memory detatchen
      perror("Error while detatching shared memory segment");                   //Fehlerbehandlung falls Fehler bei Erstellung
      exit(EXIT_FAILURE);
  }
}

/**
 * Shared Memory Bereich löschen.
 * @param shmid ID des Shared Memory Segments
 */
int deleteSHM(int shmid){
  int r;
  if((r = shmctl(shmid, IPC_RMID, 0))!=0){                                            //Shared Memory löschen
    perror("Error while deleting shared memory segment");
    exit(EXIT_FAILURE);
  }
  return r;
}