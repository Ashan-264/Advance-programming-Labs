
#include "ECE_Buzzy.h"
#include "ECE_Enemy.h"
#include "ECE_LaserBlast.h"
#include "Global.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <list>
#include <vector>



// Global state variable
bool gameOver = false;

static sf::Texture makeSolidTexture(sf::Vector2u size, sf::Color color) {
    sf::Image img; img.create(size.x, size.y, color);
    sf::Texture tex; tex.loadFromImage(img);
    return tex;
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const unsigned W = 800, H = 600;
    sf::RenderWindow window(sf::VideoMode(W, H), "Buzzy Defender");
    window.setFramerateLimit(60);

    // Load textures from files
    sf::Texture buzzyTex;
    if (!buzzyTex.loadFromFile("graphics/Buzzy_blue.png")) {
        std::cerr << "Could not load Buzzy texture!\n";
        return -1;
    }
    
    sf::Texture bulldogTex;
    if (!bulldogTex.loadFromFile("graphics/bulldog.png")) {
        std::cerr << "Could not load bulldog texture!\n";
        return -1;
    }

    sf::Texture tigerTex;
    if (!tigerTex.loadFromFile("graphics/clemson_tigers.png")) {
        std::cerr << "Could not load tiger texture!\n";
        return -1;
    }

    // Scale down the sprites
    const float SPRITE_SCALE = 0.1f; // Scale for bulldogs
    const float TIGER_SCALE = 0.05f; // Smaller scale for tigers since their image might be larger

    // Different colored lasers for player and enemies
    sf::Texture playerLaserTex = makeSolidTexture({4, 12}, sf::Color::Cyan);
    sf::Texture enemyLaserTex = makeSolidTexture({4, 12}, sf::Color::Red);

    // Font + UI
    sf::Font font;
    if (!font.loadFromFile("graphics/arial.ttf")) {
        std::cerr << "Could not load font!\n";
        return -1;
    }

    sf::Text infoText("", font, 24);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(10.f, 10.f);
   
    sf::Text centerText("", font, 32);
    centerText.setFillColor(sf::Color::White);
    centerText.setPosition(W / 4.f, H / 2.f - 50.f);

    // Entities
    ECE_Buzzy buzzy(buzzyTex);
    buzzy.setScale(SPRITE_SCALE, SPRITE_SCALE);  // Make Buzzy smaller
    buzzy.setPosition(W * 0.5f - buzzy.getGlobalBounds().width * 0.5f,
                      20.f);  // Position Buzzy at the top

    std::vector<ECE_Enemy> enemies;
    std::list<ECE_LaserBlast> lasers;

    // Game vars
    int score = 0;
    int lives = 3;
    float enemyDirection = 1.f;
    sf::Clock clock;
    sf::Clock laserCooldown;  // Timer for laser firing
    const float LASER_COOLDOWN_TIME = 0.5f;  // Time in seconds between shots
    GameState state = GameState::Start;

    // Helper to reset game
    auto resetGame = [&]() {
        score = 0;
        lives = 3;
        enemies.clear();
        lasers.clear();
        
        // Create 4 rows of enemies
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 8; ++col) {
                // Alternate between tiger and bulldog textures per row
                const sf::Texture& enemyTexture = (row % 2 == 0) ? tigerTex : bulldogTex;
                enemies.emplace_back(enemyTexture);
                // Use different scale based on the texture
                float scale = (row % 2 == 0) ? TIGER_SCALE : SPRITE_SCALE;
                enemies.back().setScale(scale, scale);
                enemies.back().setPosition(
                    80.f + col * 80.f,  // X position
                    H - 100.f - (row * 60.f)  // Y position, spacing rows 60 pixels apart
                );
            }
        }
        
        buzzy.setPosition(W * 0.5f - buzzy.getGlobalBounds().width * 0.5f,
                          20.f);  // Reset Buzzy to top
        enemyDirection = 1.f;
        state = GameState::Playing;
    };

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();

            if (state == GameState::Start &&
                e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
                resetGame();
            }

            if (state == GameState::GameOver &&
                e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
                resetGame();
            }

            if (state == GameState::Victory &&
                e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
                resetGame();
            }

            if (state == GameState::Playing &&
                e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space &&
                laserCooldown.getElapsedTime().asSeconds() >= LASER_COOLDOWN_TIME) {
                ECE_LaserBlast::createPlayerLaser(lasers, playerLaserTex, buzzy.getGlobalBounds());
                laserCooldown.restart();  // Reset the cooldown timer
            }
        }

        float dt = clock.restart().asSeconds();

        // ------------------------
        // Game logic per state
        // ------------------------
        if (state == GameState::Playing) {
            buzzy.update(dt, window);

            // Enemies move
            bool dropDown = false;
            bool enemyReachedTop = false;
            for (auto& en : enemies) {
                en.update(dt, enemyDirection, dropDown, window);
                
                // Check if any enemy reached the top
                if (en.hasReachedTop(window)) {
                    enemyReachedTop = true;
                    break;
                }
                
                if (en.shouldFire()) {
                    ECE_LaserBlast::createEnemyLaser(lasers, enemyLaserTex, en.getGlobalBounds());
                }
            }
            
            
            
            if (dropDown) {
                enemyDirection *= -1.f;
                for (auto& en : enemies) en.drop();
            }

            // Update lasers
            ECE_LaserBlast::updateAllLasers(lasers, dt, window);

            // Check collisions
            for (auto it = lasers.begin(); it != lasers.end();) {
                bool remove = false;
                if (it->isFromPlayer()) {
                    // Check vs enemies
                    for (auto enIt = enemies.begin(); enIt != enemies.end();) {
                        if (it->getGlobalBounds().intersects(enIt->getGlobalBounds())) {
                            enIt = enemies.erase(enIt);
                            score += 10;
                            remove = true;
                            break;
                        } else {
                            ++enIt;
                        }
                    }
                } else {
                    // Enemy laser vs Buzzy
                    if (it->getGlobalBounds().intersects(buzzy.getGlobalBounds())) {
                        lives--;
                        remove = true;
                        if (lives <= 0) {
                            state = GameState::GameOver;
                        }
                    }
                }

                if (remove) it = lasers.erase(it);
                else ++it;
            }

            // Win condition
            if (enemies.empty()) {
                state = GameState::Victory;
            }
        }

        // ------------------------
        // Drawing
        // ------------------------
        window.clear(sf::Color(20, 20, 30));

        if (state == GameState::Start) {
            centerText.setPosition(W / 2.f - centerText.getGlobalBounds().width / 2.f, H / 3.f);            
            centerText.setString("Start Screen\n\nPress ENTER to Start");
            window.draw(centerText);
        }
        else if (state == GameState::Playing) {
            for (auto& en : enemies) window.draw(en);
            for (auto& l : lasers) window.draw(l);
            window.draw(buzzy);
            
            // UI: score + lives
            infoText.setString("Score: " + std::to_string(score) + "\nLives: " + std::to_string(lives));
            window.draw(infoText);
        }
        else if (state == GameState::GameOver) {
            centerText.setPosition(W / 2.f - centerText.getGlobalBounds().width / 2.f, H / 3.f);
            centerText.setString("Start Screen\n\nGame Over!\nThe enemies have reached your base!\nScore: " + std::to_string(score) + "\n\nPress ENTER to try again");
            window.draw(centerText);
        }
        else if (state == GameState::Victory) {
            centerText.setPosition(W / 2.f - centerText.getGlobalBounds().width / 2.f, H / 3.f);
            centerText.setString("Start Screen\n\nVictory!\nScore: " + std::to_string(score) + "\n\nPress ENTER to Play Again");
            window.draw(centerText);
        }

        window.display();
    }
}
