#ifndef LOGIC_SYSTEM
#define LOGIC_SYSTEM

#include <string>
#include "message.h"
#include "gamesystem.h"

class LogicSystem: public GameSystem{
private:
  bool _quit_request;
  static const std::string from_me;
  static const std::string to_me;
  static const std::string quit_prompt;

public:
  LogicSystem(MessageBus& mb);
  bool handleMessage(const std::string& message);
  bool postMessage(const std::string& message);
  bool logMessage(const std::string& message);

};

#endif
