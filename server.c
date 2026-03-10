#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#include "header.h"

void Worker(int id_coda_richieste, int id_coda_risposte);

int main() {

    key_t chiave_coda_connessione = ftok(".", 'a');

    int id_coda_connessione = msgget(chiave_coda_connessione, IPC_CREAT | 0664));
    if(id_coda_connessione < 0){
        perror("errore msgget coda connessione");
        exit(1);
    }


    for(int i=0; i<TOTALE_CLIENT; i++) {

        /* TBD: Ricevere un messaggio dalla coda delle connessioni,
                e salvare i valori ricevuti in "id_coda_richieste" e
                in "id_coda_risposte".
         */

        messaggio_connessione msg_con;

        int err;

        err = msgrcv(id_coda_connessione, &msg_con, sizeof(messaggio_connessione) - sizeof(long), 1, 0);
        if(err < 0){
            perror("errore msgrcv coda connessione");
            exit(1);
        } 

        int id_coda_richieste = msg_con.id_richiesta;
        int id_coda_risposte = msg_con.id_risposta;

        printf("[Server %d] Ricezione connessione, coda richieste = %d, coda risposte = %d\n", getpid(), id_coda_richieste, id_coda_risposte);

        
        /* TBD: Creare un processo figlio,
                e fargli eseguire la funzione "Worker()",
                passandogli in ingresso gli id delle code
                di richiesta e di risposta.
         */

        pid_t pid = fork();
        if(pid == 0){
            Worker(id_coda_richieste, id_coda_risposte);
            exit(0);
        }else if(pid < 0){
            perror("errore fork worker");
        }

    }


    /* TBD: Attendere la terminazione dei processi figli "Worker"
            (in totale, il numero di figli è pari a TOTALE_CLIENT)
     */

    int status;

    for(int i=0; i<TOTALE_CLIENT; i++){
        wait(&status);
    }


    /* TBD: Rimuovere la coda connessioni */

    msgctl(id_coda_connessione, IPC_RMID, NULL);

    return 0;
}


void Worker(int id_coda_richieste, int id_coda_risposte) {

    printf("[Worker %d] In attesa di richieste...\n", getpid());

    for(int i=0; i<TOTALE_MESSAGGI; i++) {

        /* TBD: Ricevere un messaggio di richiesta */

        messaggio_richiesta msg_ric;

        int err;

        err = msgrcv(id_coda_richieste, &msg_ric, sizeof(messaggio_richiesta) - sizeof(long), 2, 0);
        if(err < 0){
            perror("errore msgrcv coda richiesta");
            exit(1);
        }

        int valore_ricevuto = msg_ric.valore;

        printf("[Worker %d] Ricevuto richiesta, valore = %d\n", getpid(), valore_ricevuto);


        int valore_inviato = valore_ricevuto;

        /* TBD: Inviare un messaggio di risposta,
                inserendo "valore_inviato" nel messaggio.
         */

        messaggio_risposta msg_ris;
        
        msg_ris.mtype = 2;
        msg_ris.valore = valore_inviato;

        err = msgsnd(id_coda_risposte, &msg_ris, sizeof(messaggio_risposta) - sizeof(long), 0);
        if(err < 0){
            perror("errore msgsnd coda richiesta");
            exit(1);
        } 

        printf("[Worker %d] Inviato risposta, valore = %d\n", getpid(), valore_inviato);
    }
}