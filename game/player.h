//
//  player.hpp
//  game
//
//  Created by Lydia Nevin on 10/19/17.
//  Copyright © 2017 Lydia Nevin. All rights reserved.
//

#ifndef player_hpp
#define player_hpp

#include <stdio.h>
#include "gameobject.h"

class Player: public GameObject{
private:
    
    int _x_direction;
    int _y_direction;
    
    unsigned int _mvmt_frames;
    float _speed;
    
    int _grid_size;
    
    bool _moving;
    bool _stop_flag;
    
    sf::Vector2i _target;
    sf::Vector2f _unit;

    bool onEdgeX() const;
    bool onEdgeY() const;
    bool pastTarget() const;
    int getYRound() const;
    int getXRound() const;
    static const float edgeBoundary;
    
public:
    Player(){}
    Player(unsigned int i, sf::Texture& t, int grid_size = 128);
    
    void moveDirection(int dx, int dy);
    void haltX();
    void haltY();
    void setSpeed(float speed);
    void stop();
    
    int getX() const;
    int getY() const;
    
    void nextFrame();
    
    sf::Vector2i getTarget() const;
    sf::Vector2i getSpot() const;
    
    // reasons to recompute target:
    // 1. We moved past it
    // 2. changed direction
    void recomputeTarget();
    sf::Vector2i checkTarget();
    
    
};

#endif /* player_hpp */
