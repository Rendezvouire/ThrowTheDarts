#include "Menu_State.h"
#include "Game_State.h"
#include "Game.h"

Menu_State::Menu_State(Game& game) : State(game), selected(0) {
    font.loadFromFile("Nexa-Heavy.ttf");

    titleText.setFont(font);
    titleText.setString("Throw the Darts");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(150, 100);

    playText.setFont(font);
    playText.setString("Play");
    playText.setCharacterSize(40);
    playText.setFillColor(sf::Color::Green);
    playText.setPosition(350, 250);

    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(350, 320);
}

void Menu_State::handleInput() {
    sf::Event event;
    while (game.window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            game.window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);

            if (playText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                game.scheduleStateChange(std::make_unique<Game_State>(game));
            } else if (exitText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                game.window.close();
            }
        }

        // Highlight saat hover
        sf::Vector2i mousePos = sf::Mouse::getPosition(game.window);
        playText.setFillColor(playText.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? sf::Color::Green : sf::Color::White);
        exitText.setFillColor(exitText.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? sf::Color::Green : sf::Color::White);
    }
}

void Menu_State::update(sf::Time dt) {}

void Menu_State::draw() {
    game.window.draw(titleText);
    game.window.draw(playText);
    game.window.draw(exitText);
}