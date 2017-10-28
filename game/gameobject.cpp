//
//  gameobj.cpp
//  game
//
//  Created by Lydia Nevin on 10/18/17.
//  Copyright © 2017 Lydia Nevin. All rights reserved.
//

#include "gameobject.h"

#include <string>

using namespace std;

GameObject::GameObject(unsigned int i, sf::Texture& t){
    _idx = i;
    _sprite = sf::Sprite(t);
    _height_units = 1;
    _width_units = 1;
    _layer = 0;
    _can_walk = true;
    _animated = false;
}

std::string GameObject::str() const{
    return to_string(_idx%10);
}

unsigned int GameObject::get_id() const{
    return _idx;
}

void GameObject::setPosition(float x, float y, bool animate, int frames){


    _sprite.setPosition(x,y);
}

void GameObject::nextFrame(){
    if(!_animated){
        return;
    }
}

sf::Sprite& GameObject::getSprite(){
    return _sprite;
}

bool GameObject::canWalk(){
    return _can_walk;
}

void GameObject::setCanWalk(bool c){
    _can_walk = c;
}
