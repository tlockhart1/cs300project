package edu.cs300;

public class SearchRequest {

  int requestID;
  String prefix;


  public SearchRequest(int reqID, String reqPrefix){
    this.requestID=reqID;
    this.prefix=reqPrefix;
  }

  public String toString(){
    return this.requestID+" "+this.prefix;
  }
  
  public int getID(){
    return this.requestID;
  }

  public String getPrefix(){
    return this.prefix;
  }

}
