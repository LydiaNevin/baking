#ifndef MESSAGE_BUS
#define MESSAGE_BUS

#include <queue>
#include <string>

class GameSystem;

class MessageBus{
  private:
    std::vector<GameSystem*> _recipients;
    int _counter;
    std::queue<std::string> _messages;
    bool sendToAll();

  public:
    MessageBus(){}
    void addRecipient(GameSystem* gs);
    bool addMessage(const std::string& m);
    bool sendMessage();
    const bool queueEmpty();
    bool sendAll();
    const int howMany();

};

#endif
