#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#include "header.h"


int main() {


    /* TBD: Creare un processo figlio, e
            fargli eseguire il programma "./server"
     */

    pid_t pid;
    
    pid = fork();
    if(pid == 0){
        execl("./server", "server", NULL);
        perror("errore execl server");
        exit(1);
    }else if(pid < 0){
        perror("errore fork server");
        exit(1);
    }

    for(int i=0; i<TOTALE_CLIENT; i++) {

        /* TBD: Creare un processo figlio, e
                fargli eseguire il programma "./client"
         */

        pid = fork();
        if(pid == 0){
            execl("./client", "client", NULL);
            perror("errore execl client");
            exit(1);
        }else if(pid < 0){
            perror("errore fork client");
            exit(1);
        }

    }



    int status;

    for(int i=0; i<4; i++) {
        
        /* TBD: Attendere la terminazione*/
        wait(&status);
    }
    
}