#include <sys/types.h>

#define JAVA_PATH "/usr/lib/jvm/java-11-openjdk-amd64/bin/java"

extern pid_t forkPassageProcessor();
/*calls exec, error checks*/
static void launchPassageProcessor();
