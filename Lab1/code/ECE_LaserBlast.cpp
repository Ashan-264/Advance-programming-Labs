#include "ECE_LaserBlast.h"

ECE_LaserBlast::ECE_LaserBlast(const sf::Texture& tex, sf::Vector2f vel, bool fromPlayer)
    : sf::Sprite(tex), velocity(vel), playerOwned(fromPlayer) {}

void ECE_LaserBlast::update(float dt) {
    move(velocity * dt);
}

bool ECE_LaserBlast::isOffScreen(const sf::RenderWindow& win) const {
    auto a = getGlobalBounds();
    return (a.top + a.height < 0) || (a.top > win.getSize().y);
}

bool ECE_LaserBlast::isFromPlayer() const {
    return playerOwned;
}

void ECE_LaserBlast::createPlayerLaser(std::list<ECE_LaserBlast>& lasers, const sf::Texture& tex, const sf::FloatRect& playerBounds) {
    ECE_LaserBlast blast(tex, {0.f, 400.f}, true);
    blast.setPosition(
        playerBounds.left + playerBounds.width / 2.f - 2.f,
        playerBounds.top + playerBounds.height
    );
    lasers.push_back(blast);
}

void ECE_LaserBlast::createEnemyLaser(std::list<ECE_LaserBlast>& lasers, const sf::Texture& tex, const sf::FloatRect& enemyBounds) {
    ECE_LaserBlast blast(tex, {0.f, -300.f}, false);
    blast.setPosition(
        enemyBounds.left + enemyBounds.width / 2.f - 2.f,
        enemyBounds.top
    );
    lasers.push_back(blast);
}

void ECE_LaserBlast::updateAllLasers(std::list<ECE_LaserBlast>& lasers, float dt, const sf::RenderWindow& window) {
    for (auto it = lasers.begin(); it != lasers.end();) {
        it->update(dt);
        
        if (it->isOffScreen(window)) {
            it = lasers.erase(it);
        } else {
            ++it;
        }
    }
}