#include <iostream>
#include <string>
#include <regex>

#include "logic.h"
#include "message.h"

using namespace std;

void MessageBus::addRecipient(GameSystem* gs){
  if(gs != NULL){
    _recipients.push_back(gs);
  }
}

bool MessageBus::addMessage(const string& s){
  _messages.push(s);
  return true;
}

bool MessageBus::sendMessage(){
  if(_messages.empty()){
    return false;
  }
  if(sendToAll()){
    return true;
  }
  else{
    return false;
  }
}

bool MessageBus::sendAll(){
  bool ok = true;
  while(!queueEmpty() && ok){
    ok = sendMessage();
  }
  return ok;
}

const int MessageBus::howMany(){
  return _messages.size();
}

bool MessageBus::sendToAll(){
  // add more destinations as complexity increases.
  //cout<<"Sending first message of "<<messages.size()<<endl;
  const string s = _messages.front();
  _messages.pop();
  //cout<<messages.size()<<endl;
  bool ok = true;
  for(int i = 0; i < _recipients.size(); i++){
    if(_recipients[i] != NULL){
      ok = ok && _recipients[i]->logMessage(s);
    }

  }
  if(ok){
    cout<<s<<endl;
  }
  else{
    cerr<<"ERROR: Invalid message: "<<s<<endl;
  }
  return ok;

}

const bool MessageBus::queueEmpty(){
  return _messages.empty();
}
