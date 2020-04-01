CC = gcc -std=c99 -D_GNU_SOURCE -pthread
JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
all: SystemVheader Trie PassageProcessor searchmanager
SystemVheader:
	$(JC) -h . edu/cs300/MessageJNI.java
Trie: 
	$(JC) CtCILibrary/*.java
PassageProcessor: 
	$(JC) edu/cs300/*java
searchmanager: searchmanager.o msghandling.o cda.o -lm
	$(CC) searchmanager.o msghandling.o cda.o -o searchmanager -lm
edu_cs300_MessageJNI.o: system5_msg.c 
	$(CC) -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux system5_msg.c
libsystem5msg.so: 
	$(CC) -shared -o libsystem5msg.so edu_cs300_MessageJNI.o -lc
searchmanager.o: searchmanager.c cda.h msghandling.h longest_word_search.h  queue_ids.h
	$(CC) $(DEBUG) -c searchmanager.c
msghandling.o: msghandling.c queue_ids.h msghandling.h cda.h longest_word_search.h
	$(CC) -c msghandling.c 
cda.o: cda.c cda.h
	$(CC) -c cda.c

clean: 
	$(RM) -r *.exe *.o edu/cs300/*.class CtCILibrary/*.class searchmanager
