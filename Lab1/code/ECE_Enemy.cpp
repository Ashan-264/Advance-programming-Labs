#include "ECE_Enemy.h"

// Constructor implementation
ECE_Enemy::ECE_Enemy(const sf::Texture& tex) : sf::Sprite(tex) {}

// update method
void ECE_Enemy::update(float dt, float direction, bool& dropDown, const sf::RenderWindow& win) {
    move(direction * speed * dt, 0.f);

    sf::FloatRect bounds = getGlobalBounds();
    if (bounds.left < 0 ||
        bounds.left + bounds.width > win.getView().getSize().x) {
        dropDown = true;
    }
}

// shouldFire method
bool ECE_Enemy::shouldFire() {
    return (std::rand() % 1000 < 1);
}

// drop method
void ECE_Enemy::drop() {
    move(0.f, -20.f);
}

// hasReachedTop method
bool ECE_Enemy::hasReachedTop(const sf::RenderWindow& win) const {
    return getGlobalBounds().top <= 0.f;
}

// checkLaserCollision method
bool ECE_Enemy::checkLaserCollision(ECE_LaserBlast& laser) {
    return getGlobalBounds().intersects(laser.getGlobalBounds());
}