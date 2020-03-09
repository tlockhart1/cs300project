# Spring_cs300_project

Git Repository for project files

## All commands and code have been tested on cs426.ua.edu

- Set up JAVA_HOME environment variable

`export JAVA_HOME=/usr/java/latest`

- Compile java files in both packages

`javac edu/cs300/*java`

`javac CtCILibrary/*.java`

- Create header file for System V C message functions

`javac -h . edu/cs300/MessageJNI.java`

- Compile C native functions into a library for use with MessageJNI

`gcc -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux system5_msg.c -o edu_cs300_MessageJNI.o`

`gcc -shared -o libsystem5msg.so edu_cs300_MessageJNI.o -lc`

- Compile test send and receive functions

`gcc -std=c99 -D_GNU_SOURCE msgsnd_pr.c -o msgsnd`

`gcc -std=c99 -D_GNU_SOURCE msgrcv_lwr.c -o msgrcv`


## Commands to run sample programs

- Create a message with the prefix and ID and puts it on a queue
`./msgsnd con`

- Java program reads queue contents using native C function and creates and sends a response message via the System V msg queue

`java -cp . -Djava.library.path=. edu.cs300.MessageJNI`

- Retrieves message from System V queue and prints it
`./msgrcv`

- Program to illustrate use of Java threading, Trie and BlockingArrayQUEUE
`java edu.cs300.ParallelTextSearch con`

## Commands to check/delete message queues

- Determine the status of your message queue via `ipcs -a`
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x0d03bc96 0          crimson2    666        0            0           
0xffffffff 98305      crimson3 666        0            0           
0x0303fabb 65538      crimson1   666        0            0           
**0x0c030904 131075     anderson   666        8            1**           
------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
------ Semaphore Arrays --------
key        semid      owner      perms      nsems     

*0x0C030904 is the queue created by `./msgsnd con`.  It has 1 message of 8 bytes.*


- Removes the queue along with any messages `ipcrm -Q 0x0c030904`
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x0d03bc96 0          crimson2    666        0            0           
0xffffffff 98305      crimson3 666        0            0           
0x0303fabb 65538      crimson1   666        0            0           

------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
------ Semaphore Arrays --------
key        semid      owner      perms      nsems 


>Note:  If your queue key is 0x0xffffffff, you must follow the directions to create the queue file in your home directory and update queue_ids.h
