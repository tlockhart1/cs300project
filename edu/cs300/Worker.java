package edu.cs300;

import java.io.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@SuppressWarnings("rawtypes") 
class Worker extends Thread{

  private LongestWordTrie textTrieTree;
  private ArrayBlockingQueue prefixRequestArray;
  private ArrayBlockingQueue resultsOutputArray;
  private int id;
  private String passageName;

  public Worker(String passage,int id,ArrayBlockingQueue prefix, ArrayBlockingQueue results){
    this.prefixRequestArray=prefix;
    this.resultsOutputArray=results;
    this.id=id;
    this.passageName = passage;//put name of passage here
  }

  @SuppressWarnings("unchecked")
  public void run() {
    System.err.println("Worker-"+this.id+" ("+this.passageName+") thread started ...");
    textTrieTree=new LongestWordTrie(getWords(passageName));
    while (true){
      try {
        SearchRequest prefix = (SearchRequest)this.prefixRequestArray.take();
        String found = this.textTrieTree.getLongestWord(prefix.getPrefix());
        
        int present;
        if(found.isEmpty()) present = 0;
        else present = 1;
        WorkerResults request_response = new WorkerResults(id, passageName, found, present);

        String output = "Worker - " + Integer.toString(id) + " " + prefix.getID() + ":" + prefix.getPrefix() + " ==> ";
        if(request_response.isPresent() == 1)
          output = output + request_response.getLongestWord();
        else
          output = output + "not found";
        System.err.println(output);

        resultsOutputArray.put(request_response);
      
      } catch(InterruptedException e){
        System.out.println(e.getMessage());
      }
    }
  }
  
  private ArrayList<String> getWords(String fileName){
      File passage = new File(fileName); 
      ArrayList<String> words = new ArrayList<String>();
      Pattern insideWord = Pattern.compile("[^a-zA-Z]+");
      Pattern delimiter = Pattern.compile("[^a-zA-Z'-]+");
      Matcher badword;
      try{
        Scanner scanner = new Scanner(passage);
        scanner.useDelimiter(delimiter);
        while(scanner.hasNext()){
          String token = scanner.next().toLowerCase();
          //System.err.println(token);
          badword = insideWord.matcher(token);
          if(!badword.find() && token.length() >= 3) words.add(token);
        }
        scanner.close();
      }catch (FileNotFoundException e1){
          e1.printStackTrace();
      }
      return words;
  }
}
