#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <semaphore.h>
#include "longest_word_search.h"
#include "queue_ids.h"
#include "passageprocessor.h"
#include "msghandling.h"
#include "cda.h"


sem_t hold;
int current_rcv_message = 0;
int current_snd_message = 0;
int num_passages = -1;
CDA *pref = NULL;

void printResults(MESSAGE *msg, int index, FILE *where);
size_t initMESSAGEbuffer(prefix_buf **sbuf, int type, int id, const char *str, int length);
void initMESSAGErcv(response_buf **rbuf);
int remainingMessages(response_buf *rbuf, int current_msg);
void parsePrefixes(CDA *pref, char **argv, int argc);
int getPrefix(char **argv, int index, int num_prefix);
void freeResources(CDA *msg, CDA *pref);
void border(struct winsize terminal);
void SIGINTstatus(int siginit);

int main(int argc, char **argv){

    if(sem_init(&hold, 1,1) < 0){
        perror("Error in making semphore\n");
        exit(-1);
    }
    signal(SIGINT, SIGINTstatus);

    int msg_sent = 0;
    int num_prefix = argc;

    /*allow initialization of pref before SIGINT handler can access*/
    sem_wait(&hold);
        pref = newCDA(0);
    sem_post(&hold);

    /*CDA -> circular dynamic array, written by me*/
    CDA *msg = newCDA(0);

    struct winsize terminal; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal);
        
    parsePrefixes(pref, argv, argc);
    if(sizeCDA(pref) == 0){
        fprintf(stderr, "No prefixes were provided. Exiting...\n");
        exit(-1);
    }

    insertCDAback(msg, newMESSAGE(IPC_CREAT, 0666, CRIMSON_ID, QUEUE_NUMBER, 0));
    initSysQueueV(getCDA(msg, msg_sent));

    prefix_buf *sbuf = NULL;
    int buf_length;
    initMESSAGEbuffer(&sbuf, 1, msg_sent + 1, getCDA(pref, msg_sent), WORD_LENGTH);
    setMESSAGEsbuf(getCDA(msg, msg_sent), sbuf);
    buf_length =  strlen(sbuf->prefix) + sizeof(int) + 1; 

    #ifdef DEBUG
        border(terminal);
    #endif

    int current_msg = 0;
    response_buf *rbuf;

    while(sndMESSAGE(getCDA(msg, msg_sent), sbuf, buf_length)){

        /*update current_snd_message global var*/
        sem_wait(&hold);
            current_snd_message = msg_sent;
        sem_post(&hold);

        int err;
        initMESSAGErcv(&rbuf);

        printf("Message(%d): \"%s\" Sent (%ld bytes)\n\n", msg_sent + 1, (char*)getCDA(pref, msg_sent), sizeof(char*));

        /*sleep for variable amount of time between sending messages*/
        sleep(atoi(argv[1]));

        current_msg = 0;
        while((err = rcvMESSAGE(getCDA(msg, msg_sent), rbuf, 2, 0))){
            /*(rbuf->index > -1) -- prevents segfault due to premature entry into while loop
             *(before rcvMESSAGE returns successfully with first meessage)
             *before the if statement, sending SIGINT twice would cause segfault by sending execution into the while loop before 
             *an actual message was received. -- caused segfault*/
            if(rbuf->index > -1){

                /*protect shared variables num_passages and current_rcv_message from SIGINT handler during assignment*/
                sem_wait(&hold);

                    num_passages = rbuf->count; 
                    current_rcv_message = rbuf->index;

                sem_post(&hold);

                if(err < 0){

                    fprintf(stderr, "rcvMESSAGE returned an error. Exiting.\n");
                    exit(-1);

                }

                if(current_msg == 0)
                    updateMESSAGErbufsize(getCDA(msg, msg_sent), rbuf->count);

                insertMESSAGErbuf(getCDA(msg, msg_sent), rbuf, rbuf->index);
                if(remainingMessages(rbuf, current_msg) == 0) break;
                initMESSAGErcv(&rbuf);

                current_msg++;

            }

        }

        printf("Report \"%s\"\n", ((prefix_buf*)getMESSAGEsbuf(getCDA(msg, msg_sent)))->prefix);

        for(int i = 0; i < rbuf->count; i++)
            printResults(getCDA(msg, msg_sent), i, stdout);
        fprintf(stdout, "\n");

        msg_sent++;

        if(msg_sent == sizeCDA(pref)){

            //send 0 prefix_buf id to passage processor for termination
            initMESSAGEbuffer(&sbuf, 1, 0, "", 0);

            if (endMESSAGE(getCDA(msg, msg_sent - 1), sbuf, 1 + sizeof(int) + 1) < 0){

                fprintf(stderr, "endMESSAGE returned an error. Exiting...\n");
                exit(-1);

            }

            free(sbuf);
            break;

        }
        
        insertCDAback(msg, newMESSAGE(IPC_CREAT, 0666, CRIMSON_ID, QUEUE_NUMBER, 0));
        initMESSAGEbuffer(&sbuf, 1, msg_sent + 1, (char*)getCDA(pref, msg_sent), WORD_LENGTH);
        setMESSAGEsbuf(getCDA(msg, msg_sent), sbuf);
        setMESSAGEmsquid(getCDA(msg, msg_sent), getMESSAGEmsquid(getCDA(msg, msg_sent - 1))); 
        buf_length = strlen(sbuf->prefix) + sizeof(int) + 1; 

        #ifdef DEBUG
            fprintf(stderr, "message length: %ld\n", strlen(sbuf->prefix));
        #endif

        /*sleep for variable amount of time between sending messages*/
        sleep(atoi(argv[1]));

    } 

    freeResources(msg, pref);
    return 0;

}

void printResults(MESSAGE *msg, int index, FILE *where){

    response_buf *rbuf = (response_buf*)getMESSAGErbuf(msg, index);

    fprintf(where, "Passage: %d - ", rbuf->index);
    fprintf(where, "%s - ", rbuf->location_description);

    if(rbuf->present == 1)
        fprintf(where, "%s\n", rbuf->longest_word);
    else
        fprintf(where, "no word found\n");

}

/* initialize prefix_buf pointer */
size_t initMESSAGEbuffer(prefix_buf **sbuf, int type, int id, const char *str, int length){

    *sbuf = malloc(sizeof(prefix_buf));
    (*sbuf)->mtype = type;
    (*sbuf)->id = id;

    return strlcpy((*sbuf)->prefix, str, length);

}

/* initialize response_buf pointer */
void initMESSAGErcv(response_buf **rbuf){
    *rbuf = malloc(sizeof(response_buf));
    (*rbuf)->count = -1;
    (*rbuf)->index = -1;
}

/* Determine when passage processor has sent all messages for current prefix */
int remainingMessages(response_buf *rbuf, int current_msg){

    return rbuf->count - current_msg - 1;

}

/* while a prefix is non spec-conforming, move to the next prefix in argv 
 * returns the index to the next available spec conforming prefix
 */
int getPrefix(char **argv, int index, int num_prefix){
    char *prefix = argv[index];
    int length = strlen(prefix);

    while (index < num_prefix && (length < 3 || length > 20)){

        #ifdef DEBUG
            if(length > 20)  fprintf(stderr, "\"%s\" is greater that 20 characters. Tossed\n", prefix);
            else fprintf(stderr, "\"%s\" is less that 3 characters. Tossed\n", prefix);
        #endif

        if(++index < num_prefix){
            prefix = argv[index];
            length = strlen(prefix);
        }
    }

    return index;

}

void freeResources(CDA *msg, CDA *pref){

    for(int i = 0; i < sizeCDA(msg); i++){
        freeMESSAGE(getCDA(msg, i));
    }

    freeCDA(pref);
    freeCDA(msg);

}

/* go through argv starting at index 2 (first prefix), call getPrefix()
 * to determine the location of the next acceptable prefix in argv,
 * then push into CDA *pref
 */
void parsePrefixes(CDA *pref, char **argv, int argc){
    int i = 2;
    int index = 0;

    while (i < argc){

        i = getPrefix(argv, i, argc); //getPrefix() should never get an i that is out of bounds...
        #ifdef DEBUG
            fprintf(stderr, "parsePrefixes() i: %d\n", i);
        #endif
        if(i == argc) break;

        sem_wait(&hold);
            insertCDAback(pref, argv[i]);
        sem_post(&hold);

        #ifdef DEBUG
            fprintf(stderr, "pref: %s, index: %d\n", (char*)getCDA(pref, index), i);
        #endif 

        i++;
        index++;

    }

}

/* provides some readability to output */
void border(struct winsize terminal){

    for(int i = 0; i < terminal.ws_col; i++)
        printf("-");
    printf("\n\n");

}

void SIGINTstatus(int siginit){

    sem_wait(&hold);

    if(!pref){
        fprintf(stderr,"Waiting for initialization...\n"); 
        return;
    }

    for(int i = 0; i < sizeCDA(pref); i++){

       printf("%s - ", (char*)getCDA(pref, i));

       if(i < current_snd_message) printf("done\n");
       else if(i == current_snd_message && num_passages > -1 && num_passages > 0){

           if(current_rcv_message == num_passages) printf("done\n");
           else printf("%d of %d\n", current_rcv_message + 1, num_passages);

       }
       else printf("pending\n");

    }

    sem_post(&hold);

}

