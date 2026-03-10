#ifndef HEADER
#define HEADER

typedef struct {

    /* TBD: Definire la struttura del messaggio di connessione,
            in cui passare lo id della coda richieste, e 
            lo id della coda risposte.
     */

    long mtype;
    int id_richiesta;
    int id_risposta; 

} messaggio_connessione;

typedef struct {

    /* TBD: Definire la struttura del messaggio di richiesta,
            per inviare un valore intero.
     */

    long mtype;
    int valore;

} messaggio_richiesta;

typedef struct {

    /* TBD: Definire la struttura del messaggio di risposta,
            per ricevere un valore intero.
     */

    long mtype;
    int valore;

} messaggio_risposta;

/* TBD: Definire con delle macro i valori dei tipi di messaggio */


#define TOTALE_MESSAGGI 3
#define TOTALE_CLIENT 3


#endif