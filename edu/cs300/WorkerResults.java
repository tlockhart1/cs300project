package edu.cs300;

class WorkerResults{
    private int passageIndex;
    private String passageName;
    private String longestWord;
    private int present;

    public WorkerResults
    (
        int passageIndex,
        String passageName,
        String longestWord,
        int present
    ){
        this.passageIndex= passageIndex;
        this.passageName = passageName;
        this.longestWord = longestWord;
        this.present = present;
    }

    public int getPassageIndex(){
        return passageIndex;
    }

    public String getPassageName(){
        return passageName;
    }

    public String getLongestWord(){
        return longestWord;
    }

    public int isPresent(){
        return present;
    }
}