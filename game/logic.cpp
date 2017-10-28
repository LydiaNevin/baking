#include <iostream>
#include <string>

#include "logic.h"
#include "message.h"

using namespace std;

const string LogicSystem::quit_prompt =
    "Quit? press y to confirm, or any other input to cancel.";

const string LogicSystem::from_me = "LOG: ";
const string LogicSystem::to_me = "log ";

LogicSystem::LogicSystem(MessageBus& mb): GameSystem(mb){
  _quit_request = false;
}

bool LogicSystem::logMessage(const string& message){
  if(message.find(from_me) == 0){
    // it's our own message back. We don't need that? I think?
    // at least for now... can't hurt to have an id
    return true;
  }
  _message_queue.push(message);
  return true;
}

bool LogicSystem::handleMessage(const string& message){
  if(message.compare("Initial message.") == 0){
    postMessage("Simple response.");
    return true;
  }
  if(!message.compare("quit") || !message.compare("q") || !message.compare("Quit")){
    _quit_request = true;
    postMessage(quit_prompt);
    return true;
  }
  if(_quit_request){
    if(!message.compare("y")){
      exit(0);
    }
    else if(message.compare(quit_prompt) != 0){
      _quit_request = false; // still handle this input so don't return
    }
  }

  return true;
}


bool LogicSystem::postMessage(const string& message){
    // cout << "How many " << messageBus->howMany()<<endl;
    return _message_bus.addMessage(from_me + message);
}
