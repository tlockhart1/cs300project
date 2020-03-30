#!/bin/bash
#run????????????????

echo "${@: $#}";
./searchmanager ${@: $#}; 
#echo "${JAVA_11} -cp . -Djava.library.path=. edu.cs300.ParallelTextSearch";
#${JAVA_11} -cp . -Djava.library.path=. edu.cs300.ParallelTextSearch;