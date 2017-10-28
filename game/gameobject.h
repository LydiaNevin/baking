//
//  gameobj.hpp
//  game
//
//  Created by Lydia Nevin on 10/18/17.
//  Copyright © 2017 Lydia Nevin. All rights reserved.
//

#ifndef gameobj_hpp
#define gameobj_hpp

#include <stdio.h>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class GameObject{
protected:
    sf::Sprite _sprite;
    unsigned int _idx;
    unsigned int _width_units;
    unsigned int _height_units;
    int _layer;
    bool _can_walk;

    bool _animated;
    int _anim_frames;
    
public:
    GameObject(){}
    GameObject(unsigned int i, sf::Texture& t);
    std::string str() const;
    unsigned int get_id() const;
    
    virtual void setPosition(float x, float y, bool animate = false, int frames = 10);
    
    sf::Sprite& getSprite();
    
    bool canWalk();
    void setCanWalk(bool c);
    
    virtual void nextFrame();
    
    virtual float getX(){ return _sprite.getPosition().x; }
    virtual float getY() { return _sprite.getPosition().y; }
    
    
};

#endif /* gameobj_hpp */
