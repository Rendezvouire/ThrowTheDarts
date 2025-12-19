#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

class Menu_State : public State {
public:
    Menu_State(Game& game);
    void handleInput() override;
    void update(sf::Time dt) override;
    void draw() override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text playText;
    sf::Text exitText;
    int selected;
};