package edu.cs300;
import java.util.concurrent.*;
import java.util.*;
import java.io.File;
import java.io.FileNotFoundException;

@SuppressWarnings("rawtypes")
public class PassageProcessor {

   @SuppressWarnings("unchecked")
   public static void main(String[] args) {

    ArrayList<SearchRequest> request = new ArrayList<SearchRequest>();
    ArrayList<String> samples = getSamples();
    int treeCount = samples.size();
    ArrayBlockingQueue[] workers = new ArrayBlockingQueue[treeCount];
    ArrayBlockingQueue resultsOutputArray = new ArrayBlockingQueue(treeCount * 10);

    for (int i = 0; i < treeCount; i++) {

      workers[i] = new ArrayBlockingQueue(10);

    }

    for (int i = 0; i < treeCount; i++) {

      new Worker(samples.get(i), i, workers[i], resultsOutputArray).start();

    }

    int prefix_num = 0;

    while(request.add(MessageJNI.readPrefixRequestMsg())){
      System.err.println
          (
            "**prefix("
            + Integer.toString(request.get(prefix_num).getID())
            + ") " 
            + request.get(prefix_num).getPrefix() 
            + " received"
          );
      if(request.get(request.size() - 1).getID() == 0){
        System.err.println("Terminating...");
        System.exit(0);
      } 

      try {
        for(int i = 0; i < treeCount; i++){
          
          workers[i].put(request.get(prefix_num)); 

        }
      } catch (InterruptedException e) {

        e.printStackTrace();

      };

      int counter = 0;

      while (counter < treeCount) {

        try {
          WorkerResults results = (WorkerResults) resultsOutputArray.take();
          
          MessageJNI.writeLongestWordResponseMsg(
            prefix_num,
            request.get(prefix_num).getPrefix(),
            results.getPassageIndex(), 
            results.getPassageName(),
            results.getLongestWord(),
            treeCount, 
            results.isPresent());
          counter++;

        } catch (InterruptedException e) {

          e.printStackTrace();

        };

      }

      prefix_num++;

    }
  }

  private static ArrayList<String> getSamples() {
    
    File file = new File("passages.txt");
    ArrayList<String> samples = new ArrayList<String>();

    try {

      Scanner read_samples = new Scanner(file);

      while(read_samples.hasNextLine()){

        samples.add(read_samples.nextLine());

      }

      read_samples.close();
      
    } catch (FileNotFoundException e1) {

      e1.printStackTrace();

    }

    return samples;
  }
}
