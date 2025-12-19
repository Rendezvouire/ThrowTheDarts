#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include <iostream>
#include "State.h"

class Game {
public:
    Game();
    void run();
    void pushState(std::unique_ptr<State> state);
    void popState();
    void changeState(std::unique_ptr<State> state);
    void scheduleStateChange(std::unique_ptr<State> state);
    State* currentState();
    sf::RenderWindow window;

private:
    std::stack<std::unique_ptr<State>> states;
};

#endif