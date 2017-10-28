#ifndef GAME_SYSTEM
#define GAME_SYSTEM

#include <vector>
#include <iostream>
#include "message.h"

class GameSystem{
protected:
  MessageBus& _message_bus;
  std::queue<std::string> _message_queue;

public:
  GameSystem(MessageBus& mb) : _message_bus(mb){}
  virtual bool handleMessage(const std::string& message){return true;}
  virtual bool postMessage(const std::string& message){return true;}
  virtual bool logMessage(const std::string& message){return true;}
  bool handleNext();
};

#endif
