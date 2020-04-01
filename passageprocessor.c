/* Initially used this to fork PassageProcessor from SearchManager,
   but decided to run them seperately in there own terminals for clarity*/
#include <unistd.h>
#include <sys/types.h>
#include "passageprocessor.h"
#include <stdlib.h>
#include <stdio.h>

extern pid_t forkPassageProcessor(){
    pid_t cid = fork();

    if(cid < 0){
        fprintf(stderr, "fork() error: %d\n", (int)cid);
    }
    else if(cid == 0){
        launchPassageProcessor();
    }
    else{
        return cid;
    }
}
/*calls exec, error checks*/
static void launchPassageProcessor(){
    int err;
    if((err = execl(JAVA_PATH, JAVA_PATH, "-cp", ".", "-Djava.library.path=.", "edu.cs300.ParallelTextSearch", NULL)) < 0)
        fprintf(stderr, "execl() error: %d\n", err);
}
