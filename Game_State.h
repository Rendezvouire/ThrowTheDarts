#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>

struct Dart {
    sf::RectangleShape shape;
    int id;
    float yVelocity;
    bool stuck = false;
    float angleRel = 0.f; // sudut relatif dari pusat target
    float stickAngle;
};

class Game_State : public State {
public:
    Game_State(Game& game);
    void handleInput() override;
    void update(sf::Time dt) override;
    void draw() override;
    int countStuckDarts();

private:
    sf::Font font;
    sf::Text menuButton;
    sf::Text gameOverText;
    sf::Text winText;
    sf::CircleShape target;
    sf::Clock winClock;
    sf::RectangleShape replayButton;
    sf::Text replayText;
    sf::RectangleShape playAgainButton;
    sf::Text playAgainText;
    std::vector<Dart> darts;
    float angle;
    float rotationSpeed;
    bool isGameOver;
    bool inPauseMenu;
    bool isWin = false;
    int level;

    void throwDart();
    bool checkCollision(const Dart& dart);
    void drawPauseMenu();
};