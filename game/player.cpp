//
//  player.cpp
//  game
//
//  Created by Lydia Nevin on 10/19/17.
//  Copyright © 2017 Lydia Nevin. All rights reserved.
//

#include "player.h"
#include <cmath>
#include <iostream>
#include <cassert>

// NEED TO MAKE STOPPING/STARTING THREAD SAFE W/ FRAME DRAWING!
using namespace std;

const float Player::edgeBoundary = 0.0;

Player::Player(unsigned int i, sf::Texture& t, int grid_size): GameObject(i, t){
    _grid_size = grid_size;
    _speed = 13.0;
    _x_direction = 0;
    _y_direction = 0;
    _moving = false;
    _mvmt_frames = 0;
}

void Player::moveDirection(int dx, int dy){
    if(dx != 0){
        _x_direction = dx;
    }
    if(dy != 0){
        _y_direction = dy;
    }
    if(dx || dy){
        _moving = true;
        _stop_flag = false;
    }
}

void Player::haltX(){
    _x_direction = 0;
    if(_y_direction == 0){
        _stop_flag = true;
    }
}

void Player::haltY(){
    _y_direction = 0;
    if(_x_direction == 0){
        _stop_flag = true;
    }
}

void Player::setSpeed(float speed){
    _speed = speed;
}

int Player::getX() const{
    float x = _sprite.getPosition().x;
    int xBlocks = int(x/_grid_size + 0.5);
    return xBlocks;
}

int Player::getXRound() const{
    float x = _sprite.getPosition().x;
    int xBlocks = int(x/_grid_size + 0.5 + edgeBoundary * _x_direction);
    return xBlocks;
}

int Player::getY() const{
    float y = _sprite.getPosition().y;
    int yBlocks = int(y/_grid_size + 0.5);
    return yBlocks;
}

int Player::getYRound() const{
    float y = _sprite.getPosition().y;
    int yBlocks = int(y/_grid_size + 0.5 + edgeBoundary * _y_direction);
    return yBlocks;
}

int Player::getXdirection() const {
    return _x_direction;
}

int Player::getYdirection() const {
    return _y_direction;
}

sf::Vector2i Player::getTarget() const{
    return _target;
}


sf::Vector2i Player::getSpot() const{
    return sf::Vector2i(getX(), getY());
}

bool Player::onEdgeX() const{
    float center_fraction = 0.5 - edgeBoundary;
    return std::abs(getX()*_grid_size-_sprite.getPosition().x) > (center_fraction*_grid_size);
}

bool Player::onEdgeY() const {
    float center_fraction = 0.5 - edgeBoundary;
    return std::abs(getY()*_grid_size-_sprite.getPosition().y) > (center_fraction*_grid_size);
}

void Player::nextFrame(){
    if(_moving){
        // set movement appropriately if anything changed
        if(_stop_flag){
            _stop_flag = false;
            stop();
        }
        if(_mvmt_frames == 0){
            recomputeTarget();
        }

        assert(_mvmt_frames < 15);
        if(_mvmt_frames > 0){
            _sprite.setPosition(_sprite.getPosition() + _unit);
            _mvmt_frames--;
        }

    }

}

void Player::recomputeTarget(bool strict){
   
    //verify that we are on a grid space, or very close
    sf::Vector2f pos = _sprite.getPosition();
    int x = getX();
    int y = getY();
    if(strict){
        assert(abs(pos.x/_grid_size - x) < 0.02);
        assert(abs(pos.y/_grid_size - y) < 0.02);
    }
    
    // square up exactly with the grid space
    sf::Vector2f current_pos = sf::Vector2f(x*_grid_size, y*_grid_size);
    _sprite.setPosition(current_pos);
    
    // set new grid space target
    _target.x = x + _x_direction;
    _target.y = y + _y_direction;
    sf::Vector2f float_target(_target.x * _grid_size, _target.y*_grid_size);
    
    // set # of frames which determines speed
    if (_x_direction && _y_direction){
        _mvmt_frames = 14;
    }
    else if (_x_direction || _y_direction){
        _mvmt_frames = 10;
    }
    else{
        // we already stopped and now we've settled
        _mvmt_frames = 0;
        _unit.x = 0.0;
        _unit.y = 0.0;
        _moving = false;
        return;

    }
    
    // set unit vector for use in stepping
    
    sf::Vector2f total_movement = float_target - current_pos;
    _unit = total_movement / (float)_mvmt_frames;
    
    
}

void Player::stop(){
    _x_direction = 0;
    _y_direction = 0;
//    assert (_x_direction == 0);
//    assert (_y_direction == 0);
    _target.x = getX();
    _target.y = getY();
    sf::Vector2f float_target(_target.x * _grid_size, _target.y*_grid_size);
    sf::Vector2f current_pos = _sprite.getPosition();
    
    sf::Vector2f total_movement = float_target - current_pos;
    float magnitude = sqrt(total_movement.x * total_movement.x + total_movement.y * total_movement.y);
    float per_frame = _grid_size/10.0;
    _mvmt_frames = (int)(magnitude / per_frame + 0.5);
    assert(abs(_mvmt_frames - magnitude/per_frame) < 0.1);
    //cout<< "in stop " << _mvmt_frames << endl;
    if(_mvmt_frames > 0){
        _unit = total_movement / (float)_mvmt_frames;
    }
    else{
        _unit.x = 0.0;
        _unit.y = 0.0;
        _moving = false;
        _mvmt_frames = 0;
    }
}

