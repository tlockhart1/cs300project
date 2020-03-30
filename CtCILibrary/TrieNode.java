package CtCILibrary;

import java.util.*;

/* One node in the trie. Most of the logic of the trie is implemented
 * in this class.
 */
public class TrieNode {
    /* The children of this node in the trie.*/
    private HashMap<Character, TrieNode> children;
    private boolean terminates = false;
    /*keep track of furthest path to termination*/
    private int distance;
    private TrieNode longestChar;
    private TrieNode parent;
    /* The character stored in this node as data. */
    private char character;	

    /* Helper fxn to adjust path pointers to farthest leaf
     * if parent.distance == child.distance, then choose parent path pointer
     * to the child with greater alphanumeric value
     */
    private void updateDistance(TrieNode child){
        TrieNode parent = child.parent;
        while(parent != null){
            /* parent.distance should always be less than 1
             * if parent.longestChar == null
             */
            if(parent.distance < child.distance + 1) { 
                parent.distance = child.distance + 1;
                parent.longestChar = child;
            }
            /* parent.longestChar should not be null if here */
            else if(parent.distance == child.distance + 1){
                    //System.out.println(" X - " + parent.longestChar.character + " Y - " + child.character);
                    if(Character.compare(Character.toLowerCase(parent.longestChar.character), Character.toLowerCase(child.character)) < 0){
                        parent.distance = child.distance + 1;
                        parent.longestChar = child;
                    }

            }
                child = parent;
                parent = parent.parent;
        }
    }

	/* Constructs a trie node and stores this character as the node's value.
	 * Initializes the list of child nodes of this node to an empty hash map. */
    public TrieNode() {
        children = new HashMap<Character, TrieNode>();
        distance = 0;
        longestChar = null; 
        parent = null;
    }

    /* Constructs a trie node and stores in the node the char passed in
     * as the argument. Initializes the list of child nodes of this
     * node to an empty hash map.
     */
    public TrieNode(char character) {
        this();
        this.character = character;
    }

    /* Returns the character data stored in this node. */
    public char getChar() {
        return character;
    }

    /* Returns the distance to the furthest leaf node from current node. */
    public int getDistance(){
        return distance;
    }

    /* Add this word to the trie, and recursively create the child
     * nodes.
     *  */
    public void addWord(String word) {
    	if (word == null || word.isEmpty()) {
    		return;
    	}
    	
        char firstChar = word.charAt(0);

        TrieNode child = getChild(firstChar);
        if (child == null) {
            child = new TrieNode(firstChar);
            children.put(firstChar, child);
            child.parent = this;
        } 

        if (word.length() > 1) {
            child.addWord(word.substring(1));
        } else {
            child.setTerminates(true);
            /* big O complexity increases only by the length of the processed word
             * (2*(word.length)) for each addWord(word);
             */
            updateDistance(child);
        }
    }
    
    

    /* Find a child node of this node that has the char argument as its
     * data. Return null if no such child node is present in the trie.
     */
    public TrieNode getChild(char c) {
    	return children.get(c);
    }

    /* Returns whether this node represents the end of a complete word. */
    public boolean terminates() {
    	return terminates;
    }
    
    /* Set whether this node is the end of a complete word.*/
    public void setTerminates(boolean t) {
    	terminates = t;
    }

    public int getNumChildren(){
        return this.children.size();
    }

    public TrieNode getGreatestDistance(){
        return this.longestChar;
    }
}
