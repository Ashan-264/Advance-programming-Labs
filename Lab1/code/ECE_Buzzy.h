#pragma once
#include <SFML/Graphics.hpp>
#include "ECE_LaserBlast.h"

class ECE_Buzzy : public sf::Sprite {
public:
    explicit ECE_Buzzy(const sf::Texture& texture);
    void update(float time, const sf::RenderWindow& window);
    bool checkLaserCollision(ECE_LaserBlast& laser);
};

