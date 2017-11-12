#ifndef GRID
#define GRID

#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "message.h"
#include "gamesystem.h"
#include "gameobject.h"
#include "player.h"

class Grid : public GameSystem{



public:
    
    Grid(MessageBus& mb, const std::string& initial_file);
    
    bool loadFile(const std::string& filename);
    
    ~Grid();
    bool handleMessage(const std::string& message);
    bool logMessage(const std::string& message);

    // function that decides what message to send based on event
    // eg move player or open menu or doing various game actions- not executing or interpreting them
    // deeply, just surface level
    void postEvent(const sf::Event& event);
    void nextFrame();
    sf::Sprite& getPlayer();
    
    // get all contents (called by parent thread for drawing)
    std::vector<std::vector<std::vector<GameObject> > >& getAllSprites();
  
    void addTexture(const std::string& filename);
    
    unsigned int getWidth() { if(grid.size()) return grid[0].size(); else return 0;}
    unsigned int getHeight() { return grid.size(); }


private:
    
    std::vector<std::vector<std::vector<GameObject> > > grid;
    sf::Texture* textures;
    
    unsigned int _num_textures;
    
    static const std::string to_me;
    static const std::string from_me;
    std::string file;
    
    Player _player;
    
    int _step_size;
    int _height;
    int _width;
    
    
    void getContents(unsigned int r, unsigned int c) const;
    void gAdd(unsigned int i, unsigned int r, unsigned int c);
    bool gRemove(unsigned int i, unsigned int r, unsigned int c);
    void gClear(unsigned int r, unsigned int c);
    // void printGrid() const;
    bool getPlayerLoc() const;
    bool moveToward(int dr, int dc);
    bool valid(int r, int c);
    bool valid(sf::Vector2f target);
    bool postMessage(const std::string& message) const;
    
    int getCellX(sf::Vector2f loc);
    int getCellY(sf::Vector2f loc);
    
    void handleCollision();
    // later, add load and save functions so that we can load
    // multiple maps w/ the same grid object. Or maybe just have
    // one for each area?

};

#endif
