#include "gamesystem.h"

using namespace std;

bool GameSystem::handleNext(){
  if(_message_queue.empty()){
    return false;
  }
  else{
    string m = _message_queue.front();
    handleMessage(m);
    _message_queue.pop();
    return true;
  }
}
