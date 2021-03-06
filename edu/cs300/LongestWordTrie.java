package edu.cs300;

import CtCILibrary.*;

import java.util.*;

public class LongestWordTrie extends Trie{

    public LongestWordTrie(ArrayList<String> words){
        super(words);
    }

    public LongestWordTrie(String[] words){
        super(words);
    }
    /* Overides parent method, returning the longest
     * word if a prefix is found instead of just true or false
     */
    public String getLongestWord(String prefix){
        String word = new String();
        TrieNode lastNode = getRoot();
        for (int i = 0; i < prefix.length(); i++){
            lastNode = lastNode.getChild(prefix.charAt(i));
            if(lastNode == null) return "";
            word = word + lastNode.getChar();
        }
        lastNode = lastNode.getGreatestDistance();
       while(lastNode != null){
           word = word + lastNode.getChar();
           lastNode = lastNode.getGreatestDistance();
        }
        return word;
    }

}