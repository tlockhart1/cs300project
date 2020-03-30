/* Wrapper for systemV functions */

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "longest_word_search.h"
#include "queue_ids.h"
#include "msghandling.h"
#include "cda.h"

struct message{
    int msqid;
    int msgflg;
    key_t key;
    void *sbuf;
    CDA *rbuf;
    int err; //why did i think i need this field?
    int ret; // or this?
};

extern size_t strlcpy(char *dst, const char *src, size_t size){
    size_t    srclen;         /* Length of source string */

    /*
     * Figure out how much room is needed...
     */

    size --;

    srclen = strlen(src);

    /*
     * Copy the appropriate amount...
     */

    if (srclen > size)
        srclen = size;

    memcpy(dst, src, srclen);
    dst[srclen] = '\0';

    return (srclen);
}

extern MESSAGE *newMESSAGE(int mode, int perm, const char *path, int id, int size){
    MESSAGE *msg = (MESSAGE*)malloc(sizeof(MESSAGE));
    msg->rbuf = newCDA(size);
    msg->sbuf = NULL;
    msg->msgflg = mode | perm;
    msg->key = ftok(path, id);
    return msg;
}

extern int initSysQueueV(MESSAGE *msg){
    if ((msg->msqid = msgget(msg->key, msg->msgflg)) < 0) {
        int errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("(msgget)");
        fprintf(stderr, "Error msgget: %s\n", strerror( errnum ));
        return errnum;
    }
    return 1;
}

extern int sndMESSAGE(MESSAGE *msg, void *sbuf, int buf_length){

    if((msgsnd(msg->msqid, sbuf, buf_length, msg->msgflg)) < 0) {
        msg->err = errno;
        //fprintf(stderr,"%d, %ld, %s, %d\n", msg->msqid, msg->sbuf.mtype, msg->sbuf.prefix, (int)(msg->buf_length));
        perror("(msgsnd)");
        fprintf(stderr, "Error sending msg: %s\n", strerror( msg->err ));
        return -1; //failure, notify caller
    }

    return 1; //success, notify caller
}

extern int rcvMESSAGE(MESSAGE *msg, void *rbuf, int msgtype, int rcvmode){

    msg->ret = msgrcv(msg->msqid, rbuf, sizeof(response_buf), msgtype, rcvmode);
    msg->err = errno;

    if (msg->ret < 0 && errno !=EINTR){
            fprintf(stderr, "Value of errno: %d\n", errno);
            perror("Error printed by perror");
            fprintf(stderr, "Error receiving msg: %s\n", strerror(msg->err));
            return -1; //failure, notify caller 
        }
    return 1; //success, notify caller
}

extern int endMESSAGE(MESSAGE *msg, void *sbuf, int buf_size){
    return sndMESSAGE(msg, sbuf, buf_size);
}

extern void insertMESSAGErbuf(MESSAGE *msg, void *rbuf, int index){
    setCDA(msg->rbuf, index, rbuf);
}

extern void setMESSAGEsbuf(MESSAGE *msg, void *sbuf){
    msg->sbuf = sbuf;
}

extern void *getMESSAGErbuf(MESSAGE *msg, int index){
    return getCDA(msg->rbuf, index);
}

extern void *getMESSAGEsbuf(MESSAGE *msg){
    return msg->sbuf;
}

extern int getMESSAGEmsquid(MESSAGE *msg){
    return msg->msqid;
}

extern void setMESSAGEmsquid(MESSAGE *msg, int msqid){
    msg->msqid = msqid;
}

extern void updateMESSAGErbufsize(MESSAGE *msg, int size){
    CDA *rbuf = newCDA(size);
    freeCDA(msg->rbuf);
    msg->rbuf = rbuf;
}

extern void updateMESSAGEsbufsize(MESSAGE *msg, int size){
    CDA *sbuf = newCDA(size);
    freeCDA(msg->sbuf);
    msg->sbuf = sbuf;
}

extern void freeMESSAGE(MESSAGE *msg){
    free(msg->sbuf);
    for(int i = 0; i < sizeCDA(msg->rbuf); i++)
        free(getCDA(msg->rbuf, i));
    freeCDA(msg->rbuf);
    free(msg);
}
