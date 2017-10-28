#include <vector>
#include <iostream>

#include "grid.h"
#include "ResourcePath.hpp"

#define MAX_TEXTURES 10

using namespace std;

const string Grid::to_me = "grd ";
const string Grid::from_me = "GRD: ";

Grid::Grid(
  MessageBus& mb,
  unsigned int h, unsigned int w,
           unsigned int r, unsigned int c,
           int s_size)
: GameSystem(mb){
  grid = vector<vector<vector<GameObject> > >(h, vector<vector<GameObject> >(w));
  _step_size = s_size;
  num_textures = 0;
  textures = new sf::Texture[MAX_TEXTURES];
  addTexture("smiley.png");
  _player = Player(0, textures[0]);
  addTexture("empty.png");
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
        grid[i][j].push_back(GameObject(1,textures[1]));
        grid[i][j][0].setPosition(_step_size*j, _step_size*i);
    }
  }
}

Grid::~Grid(){
    delete[] textures;
}

std::vector<std::vector<vector<GameObject> > >& Grid::getAllSprites(){
    return grid;
}

void Grid::addTexture(const std::string& filename){
    sf::Texture t;
    if (!t.loadFromFile(resourcePath() + filename)) {
        return EXIT_FAILURE;
    }
    if(num_textures < MAX_TEXTURES){
        textures[num_textures] = t;
        num_textures += 1;
    }

}

sf::Sprite& Grid::getPlayer(){
    return _player.getSprite();
}

void Grid::nextFrame(){
    // check if the player is running into something
    sf::Vector2i target_pos = _player.getTarget();
    if(!valid(target_pos.y, target_pos.x)){
        // we should be in the center box so we settle and don't keep going
        _player.haltX();
        _player.haltY();
    }
    _player.nextFrame();
}

string pop_word(string& m){
  int leadIndex = 0;
  int spaceIndex = m.find(" ");
  while(spaceIndex==leadIndex){ // first character in search window is a space
    leadIndex++; //move search window forward by 1
    if(leadIndex >= m.size()){
      // if we've reached the end, there aren't any more words
      return "";
    }
    // find next space, hopefully after a word
    spaceIndex = m.find(" ", leadIndex);
  }
  string word = m.substr(leadIndex, spaceIndex);
  m = m.substr(spaceIndex + 1);
  return word;
}


bool Grid::logMessage(const string& message){
  if(message.find(from_me) == 0){
    return true;
  }
  int to_me_index = message.find(to_me);
  if(to_me_index == string::npos){
    return true;
  }
  
  _message_queue.push(message.substr(to_me_index + to_me.size()));
  return true;
}

bool Grid::handleMessage(const string& message){
  string my_msg = message;
  // should be "get" "add" or "remove" followed by 2 or 3 numbers
  string word = pop_word(my_msg);
  if(word.compare("print") == 0){
      cout<<"Deprecated"<<endl;
    return true;
  }
  if(word.compare("getplayer") == 0){
    return getPlayerLoc();
  }
  int n1 = stoi(pop_word(my_msg));
  int n2 = stoi(pop_word(my_msg));
  int n3;
  if(word.compare("get")==0){
    getContents(n1,n2);
    return true;
  }
  else if(word.compare("clear") == 0){
    gClear(n1,n2);
    return true;
  }
  n3 = stoi(pop_word(my_msg));
  if(word.compare("add") == 0){
    gAdd(n3, n1, n2);
    return true;
  }
  else if(word.compare("remove")==0){ // probably want to add a check
    return gRemove(n3, n1, n2);
  }
  else{
    return false;
  }

  // else{
  //   return false; // this message was for us but was formatted incorrectly
  // }

}

bool Grid::valid(int r, int c){
    bool inGrid = (r>=0 && r < grid.size())&&(c>=0 && c < grid[0].size());
    if(!inGrid){
        return false;
    }
    bool walkable = true;
    for(int i = 0; i < grid[r][c].size(); i++){
        if(!grid[r][c][i].canWalk()){
            walkable = false;
            break;
        }
    }
    return walkable;
}

bool Grid::valid(sf::Vector2f target){
    return valid(getCellX(target),getCellY(target));
}

int Grid::getCellX(sf::Vector2f loc){
    return int(loc.x/_step_size + 0.5);
}

int Grid::getCellY(sf::Vector2f loc){
    return int(loc.y/_step_size + 0.5);
}

// needs to be updated
bool Grid::moveToward(int dr, int dc){
    int player_r = _player.getY() + dr;
    int player_c = _player.getX() + dc;
    if(!valid(player_r , player_c)){
        return false;
    }

    _player.moveDirection(dc, dr);
    return true;
}

void Grid::getContents(unsigned int r, unsigned int c) const{
  const vector<GameObject>& content = grid[r][c];
  string outMsg = "";
  for(int i = 0; i < content.size(); i++){
    outMsg = outMsg + content[i].str() + " ";
  }
  postMessage(outMsg);
}

void Grid::gAdd(unsigned int id, unsigned int r, unsigned int c){
  GameObject o = GameObject(id, textures[id]);
  o.setPosition(c*_step_size, r*_step_size);
  o.setCanWalk(false);
  grid[r][c].push_back(o);
    
}


// this thing is not thread safe!! need to fix. 
bool Grid::gRemove(unsigned int id, unsigned int r, unsigned int c){
  vector<GameObject>& content = grid[r][c];
  bool found = false;
  int i;
  for(i = 0; i < content.size(); i++){
    if(content[i].get_id()==id){
      found = true;
      break;
    }
  }
  if(!found){
    return false;
  }
  for(i = i + 1; i < content.size(); i++){
    content[i-1] = content[i];
  }
  content.pop_back();
  return true;
}

void Grid::gClear(unsigned int r, unsigned int c){
  grid[r][c].clear();
}

bool Grid::postMessage(const string& message) const{
    // cout << "How many " << messageBus->howMany()<<endl;
    return _message_bus.addMessage(from_me + message);
}

void Grid::postEvent(const sf::Event& event){
    if(event.type == sf::Event::KeyPressed){
        int code = event.key.code;
        if(code==22){
            moveToward(-1, 0);
        }
        else if(code==0){
            moveToward(0, -1);
        }
        else if(code==18){
            moveToward(1, 0);
        }
        else if(code==3){
             moveToward(0, 1);
        }
    }
    if(event.type == sf::Event::KeyReleased){
        int code = event.key.code;
        if(code==22){
            _player.haltY();
        }
        else if(code==0){
            _player.haltX();
        }
        else if(code==18){
            _player.haltY();
        }
        else if(code==3){
            _player.haltX();
        }
    }
}
//void Grid::printGrid() const{
//  for(int r = 0; r < grid.size();r++){
//    for(int c = 0; c < grid[r].size();c++){
//      const cellContent& content = grid[r][c];
//      if(content.size() > 0){
//        cellObj n_out;
//        n_out = *min_element(content.begin(), content.end());
//        if(n_out == PLAYER){
//          cout<<"* ";
//        }
//        else{
//          cout<<n_out<<" ";
//        }
//      }
//      else{
//        cout<<"_ ";
//      }
//      // need to fix character length so everything lines up
//
//    }
//    cout<<endl;
//  }
//}

bool Grid::getPlayerLoc() const{
  string locString = "player " + to_string(_player.getX()) + " " + to_string(_player.getY());
  return postMessage(locString);
}
