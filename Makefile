CC = gcc -std=c99 -D_GNU_SOURCE -pthread
make: searchmanager.o msghandling.o cda.o -lm
	$(CC) searchmanager.o msghandling.o cda.o -o searchmanager -lm
edu_cs300_MessageJNI.o: system5_msg.c 
	$(CC) -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux system5_msg.c
libsystem5msg.so: 
	$(CC) -shared -o libsystem5msg.so edu_cs300_MessageJNI.o -lc
searchmanager.o: searchmanager.c cda.h msghandling.h longest_word_search.h  queue_ids.h
	$(CC) -c searchmanager.c
msghandling.o: msghandling.c queue_ids.h msghandling.h cda.h longest_word_search.h
	$(CC) -c msghandling.c 
cda.o: cda.c cda.h
	$(CC) -c cda.c
clean: 
	$(RM) *.exe *.o searchmanager
