#pragma once
#include <SFML/Graphics.hpp>
#include <list>

class ECE_LaserBlast : public sf::Sprite {
public:
    ECE_LaserBlast(const sf::Texture& tex, sf::Vector2f vel, bool fromPlayer = true);

    void update(float dt);
    bool isOffScreen(const sf::RenderWindow& win) const;
    bool isFromPlayer() const;

    // New static methods to manage all lasers
    static void updateAllLasers(std::list<ECE_LaserBlast>& lasers, float dt, const sf::RenderWindow& window);
    static void createPlayerLaser(std::list<ECE_LaserBlast>& lasers, const sf::Texture& tex, const sf::FloatRect& playerBounds);
    static void createEnemyLaser(std::list<ECE_LaserBlast>& lasers, const sf::Texture& tex, const sf::FloatRect& enemyBounds);

private:
    sf::Vector2f velocity;
    bool playerOwned;
};