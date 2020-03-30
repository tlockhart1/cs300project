package edu.cs300;

public class tester{

    public static void main(String[] args){
        String words[] = {"pathetic",
                        "lame",
                        "lamen",
                        "game", 
                        "lamentation", 
                        "blue", 
                        "black",
                        "path",
                        "paint",
                        "patent",
                        "lamentate",
                        "lamentate"};

        LongestWordTrie tree = new LongestWordTrie(words);
        System.out.println(tree.getLongestWord(args[0]));

    }

}