#include "Game_State.h"
#include "Menu_State.h"
#include "Game.h"
#include <cmath>
#include <iostream>

int dartsThrown;

Game_State::Game_State(Game& game)
    : State(game), angle(0.f), rotationSpeed(60.f), isGameOver(false), inPauseMenu(false), level(1) {

    dartsThrown = 0;

    font.loadFromFile("Nexa-Heavy.ttf");
    menuButton.setFont(font);
    menuButton.setString("II");
    menuButton.setCharacterSize(40);
    menuButton.setFillColor(sf::Color::White);
    menuButton.setPosition(700, 30);

    // Game Over Text
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Yellow);
    gameOverText.setString("Game Over");
    gameOverText.setPosition(300, 250);

    // Win Text
    winText.setFont(font);
    winText.setCharacterSize(40);
    winText.setFillColor(sf::Color::Yellow);
    winText.setString("You Win!");
    winText.setPosition(310, 250);

    // Circle
    target.setRadius(50);
    target.setOrigin(50, 50);
    target.setPosition(400, 300);
    target.setFillColor(sf::Color::Red);

    // Replay Button
    replayButton.setSize(sf::Vector2f(150, 50));
    replayButton.setFillColor(sf::Color::White);
    replayButton.setPosition(325, 320);

    replayText.setFont(font);
    replayText.setString("Replay");
    replayText.setCharacterSize(24);
    replayText.setFillColor(sf::Color::Black);
    replayText.setPosition(360, 330);

    // Play Again Button
    playAgainButton.setSize(sf::Vector2f(150, 50));
    playAgainButton.setFillColor(sf::Color::White);
    playAgainButton.setPosition(325, 320);

    playAgainText.setFont(font);
    playAgainText.setString("Play Again");
    playAgainText.setCharacterSize(20);
    playAgainText.setFillColor(sf::Color::Black);
    playAgainText.setPosition(345, 330);
}

int Game_State::countStuckDarts() {
    int count = 0;
    for (const auto& dart : darts) {
        if (dart.stuck) count++;
    }
    return count;
}

void Game_State::handleInput() {
    sf::Event event;
    while (game.window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            game.window.close();

        if (!isGameOver && !inPauseMenu && dartsThrown < 10) {
            if (event.type == sf::Event::MouseButtonPressed) {
                throwDart();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                throwDart();
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            auto mousePos = sf::Mouse::getPosition(game.window);
            if (menuButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                inPauseMenu = !inPauseMenu;
            }
        }

        if (isGameOver && event.type == sf::Event::MouseButtonPressed) {
            auto mousePos = sf::Mouse::getPosition(game.window);
            if (replayButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                // Reset game
                darts.clear();
                isGameOver = false;
                isWin = false;
                dartsThrown = 0;
            }
        }

        if (isWin && event.type == sf::Event::MouseButtonPressed) {
            auto mousePos = sf::Mouse::getPosition(game.window);
            if (playAgainButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                // Reset untuk level selanjutnya
                level++;
                rotationSpeed += 20.f;
                darts.clear();
                dartsThrown = 0;
                isWin = false;
            }
        }
    }
}


void Game_State::update(sf::Time dt) {
    if (isGameOver || inPauseMenu) return;

    const float PI = 3.14159f;
    // const float spread  = 5.f * PI/180.f;  // 5deg dalam radian
    float R = target.getRadius();
    float cx = target.getPosition().x;
    float cy = target.getPosition().y;

    angle += rotationSpeed * dt.asSeconds() * PI/180.f;
    if (angle > 2*PI) angle -= 2*PI;
    target.setRotation(angle * 180.f/PI);

    float thresholdY = cy + R + 5.f;

    for (auto& dart : darts) {
        if (!dart.stuck) {
            // terbang ke atas
            dart.shape.move(0.f, -dart.yVelocity * dt.asSeconds());

            if (dart.shape.getPosition().y <= thresholdY) {
            int stuckSoFar = countStuckDarts();

            float desiredAbs = PI/2;
            dart.angleRel = desiredAbs - angle;

            // Cek tabrakan sebelum menancap
                if (checkCollision(dart)) {
                    isGameOver = true;
                    return;
                }

            dart.stuck = true;

            float absAng = angle + dart.angleRel;
            float x = cx + R * std::cos(desiredAbs);
            float y = cy + R * std::sin(desiredAbs);
            dart.shape.setPosition(x, y);
            dart.shape.setRotation(desiredAbs * 180.f / PI + 90.f);
            }
        }
        else {
            // yang sudah tertancap: ikut muter
            float absAng = angle + dart.angleRel;
            float x = cx + R * std::cos(absAng);
            float y = cy + R * std::sin(absAng);
            dart.shape.setPosition(x, y);
            dart.shape.setRotation(absAng * 180.f/PI + 90.f);
        }
    }

    if (countStuckDarts() >= 10) {
    isWin = true;
    winClock.restart();
    }
}

void Game_State::draw() {
    game.window.draw(target);
    for (auto& dart : darts) {
        game.window.draw(dart.shape);
    }
    game.window.draw(menuButton);

    if (isGameOver) {
        game.window.draw(gameOverText);
    } else if (isWin) {
        game.window.draw(winText);
        game.window.draw(playAgainButton);
        game.window.draw(playAgainText);
    }

    if (inPauseMenu) {
        drawPauseMenu();
    }

    if (isGameOver) {
    game.window.draw(gameOverText);
    game.window.draw(replayButton);
    game.window.draw(replayText);
    }
}

void Game_State::throwDart() {
    if (dartsThrown >= 10) return;

    Dart dart;
    dart.id = dartsThrown;

    dart.shape.setSize(sf::Vector2f(5, 30));
    dart.shape.setFillColor(sf::Color::White);
    dart.shape.setOrigin(2.5f, 30);
    dart.shape.setPosition(target.getPosition().x, 550);
    dart.yVelocity = 300.f;
    dart.stuck = false;
    dart.angleRel = 0.f;
    dart.stickAngle = 0.f;  
    darts.push_back(dart);
    dartsThrown++;
}

bool Game_State::checkCollision(const Dart& newDart) {
    float radius = target.getRadius();
    float newAngle = angle + newDart.angleRel;

    float newX = target.getPosition().x + radius * std::cos(newAngle);
    float newY = target.getPosition().y + radius * std::sin(newAngle);

    for (const auto& dart : darts) {
        if (!dart.stuck || &dart == &newDart) continue;

        float stuckAngle = angle + dart.angleRel;
        float stuckX = target.getPosition().x + radius * std::cos(stuckAngle);
        float stuckY = target.getPosition().y + radius * std::sin(stuckAngle);

        float dx = stuckX - newX;
        float dy = stuckY - newY;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 20.f) { // threshold antar panah
            return true;
        }
    }
    return false;
}

void Game_State::drawPauseMenu() {
    sf::RectangleShape menuBox(sf::Vector2f(200, 150));
    menuBox.setFillColor(sf::Color(0, 0, 0, 150));
    menuBox.setPosition(300, 200);

    sf::Text resume("Resume", font, 20);
    resume.setPosition(340, 210);
    sf::Text retry("Retry", font, 20);
    retry.setPosition(340, 240);
    sf::Text mainMenu("Main Menu", font, 20);
    mainMenu.setPosition(340, 270);

    game.window.draw(menuBox);
    game.window.draw(resume);
    game.window.draw(retry);
    game.window.draw(mainMenu);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        auto pos = sf::Mouse::getPosition(game.window);
        if (resume.getGlobalBounds().contains(pos.x, pos.y)) {
            inPauseMenu = false;
        } else if (retry.getGlobalBounds().contains(pos.x, pos.y)) {
            darts.clear();
            isGameOver = false;
            inPauseMenu = false;
            dartsThrown = 0;
        } else if (mainMenu.getGlobalBounds().contains(pos.x, pos.y)) {
            game.scheduleStateChange(std::make_unique<Menu_State>(game));
        }
    }
}
