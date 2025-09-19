#include "ECE_Buzzy.h"
#include <algorithm>

//Constructor for ECE_Buzzy
ECE_Buzzy::ECE_Buzzy(const sf::Texture& texture) : sf::Sprite(texture) {}

//Update function to move Buzzy left and right
void ECE_Buzzy::update(float time, const sf::RenderWindow& win) {
    const float speed = 300.f; // speed
    auto pos = getPosition();

    //moving left and right
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) pos.x -= speed * time;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) pos.x += speed * time;

    //keep Buzzy inside the window
    pos.x = std::max(0.f, std::min(
        pos.x, (float)win.getView().getSize().x - (float)getGlobalBounds().width
    ));
    setPosition(pos);
}

bool ECE_Buzzy::checkLaserCollision(ECE_LaserBlast& laser) {
    return laser.getGlobalBounds().intersects(getGlobalBounds());
}