#pragma once
#include <SFML/Graphics.hpp>
#include "ECE_LaserBlast.h"

class ECE_Enemy : public sf::Sprite {
public:
    explicit ECE_Enemy(const sf::Texture& tex);

    void update(float dt, float direction, bool& dropDown, const sf::RenderWindow& win);
    bool shouldFire();
    void drop();
    bool hasReachedTop(const sf::RenderWindow& win) const;

    bool checkLaserCollision(ECE_LaserBlast& laser);

private:
    float speed = 50.f;
};