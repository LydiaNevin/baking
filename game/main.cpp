//
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <iostream>
#include <queue>
#include <chrono>
#include <thread>

#include "message.h"
#include "gamesystem.h"
#include "logic.h"
#include "grid.h"
#include "gameobject.h"
#include "tinyxml2.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

#define FPS_TARGET 30

using namespace std;

void loopLogic(LogicSystem* logic_system){
    // LogicSystem& logicSystem = *((LogicSystem*)logic_ptr);
    bool did_work = true;
    
    while(true){
        did_work = logic_system->handleNext();
        if(!did_work){
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
}

void loopGrid(Grid* grid){
    // Grid& grid = *(Grid*)grid_ptr;
    bool did_work = true;
    
    while(true){
        did_work = grid->handleNext();
        if(!did_work){
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
}

void loopTerminal(MessageBus* message_bus){
    // MessageBus& messageBus = *((MessageBus*)mb_ptr);
    string input;
    while(true){
        cout<<"> ";
        getline(cin,input);
        message_bus->addMessage(input);
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void loopMessages(MessageBus* message_bus){
    // MessageBus& messageBus = *((MessageBus*)mb_ptr);
    bool sent;
    while(true){
        sent = message_bus->sendMessage();
        if(!sent){
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
}



int main(){


    MessageBus message_bus;
    LogicSystem logic_system(message_bus);

    //
    
    message_bus.addRecipient(&logic_system);

    
    
    // string input;
    // while(true){
    //   messageBus.sendAll();
    //   cout<<"> ";
    //   getline(cin,input);
    //   messageBus.addMessage(input);
    // }
    

    //int step_size = 128;

    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(768,768), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);
    window.setKeyRepeatEnabled(false);
    
    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "smiley.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    // create the Grid, which keeps track of where everything is
    // (and also player movement, for now... )
    Grid grid(message_bus, "basicsamplegrid.xml");
    message_bus.addRecipient(&grid);
    
    grid.addTexture("crystal.png");
    
    
    // create threads
    
    // logic thread
    thread logic_thread(loopLogic, &logic_system);
    // grid thread
    thread grid_thread(loopGrid, &grid);
    
    // read terminal input thread
    thread term_thread(loopTerminal, &message_bus);
    
    // message sending thread
    thread message_thread(loopMessages, &message_bus);
    
    //display thread
    //thread display_thread(loopDisplay, &window, &grid);
    
    // Start the game loop
    
    chrono::steady_clock::time_point t_last_frame = chrono::steady_clock::now();
    bool running = true;
    while (running)
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                //window.close();
                running = false;
            }
            
            // Escape pressed: exit
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                //window.close();
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else{
                grid.postEvent(event);
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        vector<vector<vector<GameObject> > >& all_sprites = grid.getAllSprites();
        for(int i = 0; i < grid.getHeight(); i++){
            for(int j = 0; j < grid.getWidth(); j++){
                for(int k = 0; k < all_sprites[i][j].size(); k++){
                    window.draw(all_sprites[i][j][k].getSprite());
                }
            }
        }
        window.draw(grid.getPlayer());
        //wait for next frame time
        this_thread::sleep_for(chrono::milliseconds(33) - (chrono::steady_clock::now()-t_last_frame));
        // Update the window
        window.display();
        grid.nextFrame();
        t_last_frame = chrono::steady_clock::now();
    
    }
    // it says "release resources" idk what to do about that
    window.close();
    exit(0); // window was closed
    
    // eventually there will be like display threads and stuff for rendering
    // also threads that wait on UI inputs and process those into messages?
    // (like for real not from the terminal)
//    logic_thread.join();
//    grid_thread.join();
//    term_thread.join();
//    message_thread.join();
    //display_thread.join();
    
//    return(0);
}




