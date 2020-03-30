#include <stdlib.h>


typedef struct message MESSAGE;

extern size_t strlcpy(char *dst, const char *src, size_t size);

extern MESSAGE *newMESSAGE(int mode, int perm, const char *path, int id, int size);

extern int initSysQueueV(MESSAGE *msg);

extern int sndMESSAGE(MESSAGE *msg, void *sbuf, int buf_length);

extern int rcvMESSAGE(MESSAGE *msg, void *rbuf, int msgtype, int rcvmode);

extern void setMESSAGEsbuf(MESSAGE *msg, void *sbuf);

extern void insertMESSAGErbuf(MESSAGE *msg, void *rbuf, int index);

extern int endMESSAGE(MESSAGE *msg, void *sbuf, int buf_size);

extern void *getMESSAGErbuf(MESSAGE *msg, int index);

extern void *getMESSAGEsbuf(MESSAGE *msg);

extern int getMESSAGEmsquid(MESSAGE *msg);

extern void setMESSAGEmsquid(MESSAGE *msg, int msqid);

extern void updateMESSAGErbufsize(MESSAGE *msg, int size);

extern void updateMESSAGEsbufsize(MESSAGE *msg, int size);

extern void freeMESSAGE(MESSAGE *msg);