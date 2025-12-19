#include "Game.h"
#include "Menu_State.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(800, 600), "Throw the Darts") {
    pushState(std::make_unique<Menu_State>(*this));
    window.setFramerateLimit(60);
}

std::unique_ptr<State> nextState = nullptr;

void Game::scheduleStateChange(std::unique_ptr<State> state) {
    nextState = std::move(state);
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time dt = clock.restart();

        if (nextState) {
            changeState(std::move(nextState));
        }

        if (currentState()) {
            currentState()->handleInput();
            currentState()->update(dt);
            window.clear(sf::Color::Black);
            currentState()->draw();
            window.display();
        }
    }


}

void Game::pushState(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void Game::popState() {
    if (!states.empty()) states.pop();
}

void Game::changeState(std::unique_ptr<State> state) {
    popState();
    
    pushState(std::move(state));
    
}

State* Game::currentState() {
    if (states.empty()) return nullptr;
    
    return states.top().get();
}
