#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

int main() {

    key_t chiave_coda_connessione = ftok(".", 'a');

    int id_coda_connessione = msgget(chiave_coda_connessione, IPC_CREAT | 0664);
    if(id_coda_connessione < 0){
        perror("errore msgget coda connessione");
        exit(1);
    }
    
    key_t chiave_coda_richieste = IPC_PRIVATE;

    int id_coda_richieste = msgget(chiave_coda_richieste, IPC_CREAT | 0664);
    if(id_coda_richieste < 0){
        perror("errore msgget coda richieste");
        exit(1);
    }

    key_t chiave_coda_risposte = IPC_PRIVATE;

    int id_coda_risposte = msgget(chiave_coda_risposte, IPC_CREAT | 0664);
    if(id_coda_risposte < 0){
        perror("errore msgget coda risposte");
        exit(1);
    }

    /* TBD: Inviare un messaggio di connessione, 
            contenente gli id della coda richieste e
            della coda risposte.
     */

    messaggio_connessione msg_con;

    msg_con.mtype = 1;
    msg_con.id_richiesta = id_coda_richieste;
    msg_con.id_risposta = id_coda_risposte;

    int err;

    err = msgsnd(id_coda_connessione, &msg_con, sizeof(messaggio_connessione) - sizeof(long), 0);
    if(err < 0){
        perror("errore msgsnd connessione");
        exit(1);
    } 

    printf("[Client %d] Invio connessione, coda richieste = %d, coda risposte = %d\n", getpid(), id_coda_richieste, id_coda_risposte);


    srand(getpid());

    for(int i=0; i<TOTALE_MESSAGGI; i++) {

        int valore_inviato = rand() % 10;

        /* TBD: Inviare un messaggio sulla coda richieste,
                inserendovi "valore_inviato".
         */

        messaggio_richiesta msg_ric;
        
        msg_ric.mtype = 2;
        msg_ric.valore = valore_inviato;

        err = msgsnd(id_coda_richieste, &msg_ric, sizeof(messaggio_richiesta) - sizeof(long), 0);
        if(err < 0){
            perror("errore msgsnd coda richieste");
            exit(1);
        }


        printf("[Client %d] Valore inviato: %d\n", getpid(), valore_inviato);
    }


    for(int i=0; i<TOTALE_MESSAGGI; i++) {

        /* TBD: Ricevere un messaggio sulla coda risposte,
                e salvare il valore ricevuto in "valore_risposta".
         */

        messaggio_risposta msg_ris;

        err = msgrcv(id_coda_risposte, &msg_ris, sizeof(messaggio_risposta) - sizeof(long), 2, 0);
        if(err < 0){
            perror("errore msgrcv coda risposte");
            exit(1);
        }

        int valore_risposta = msg_ris.valore;

        printf("[Client %d] Valore ricevuto: %d\n", getpid(), valore_risposta);
    }


    /* TBD: Rimuovere la coda richieste */

    msgctl(id_coda_richieste, IPC_RMID, NULL);

    /* TBD: Rimuovere la coda risposte */

    msgctl(id_coda_risposte, IPC_RMID, NULL);

    return 0;
}