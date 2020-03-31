#!/bin/bash
#compile???????????

echo "javac edu/cs300/*java";
javac edu/cs300/*java;

echo "javac CtCILibrary/*.java";
javac CtCILibrary/*.java;

echo "javac -h . edu/cs300/MessageJNI.java";
javac -h . edu/cs300/MessageJNI.java;

echo "gcc -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux system5_msg.c -o edu_cs300_MessageJNI.o";
gcc -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux system5_msg.c -o edu_cs300_MessageJNI.o;

echo "gcc -shared -o libsystem5msg.so edu_cs300_MessageJNI.o -lc";
gcc -shared -o libsystem5msg.so edu_cs300_MessageJNI.o -lc;

#call './compile.sh -DDEBUG' to compile with debugging comments
echo "gcc -std=c99 -D_GNU_SOURCE -pthread $1 searchmanager.c passageprocessor.c msghandling.c cda.c -o searchmanager";
gcc -std=c99 -D_GNU_SOURCE -pthread $1 searchmanager.c passageprocessor.c msghandling.c cda.c -o searchmanager;

