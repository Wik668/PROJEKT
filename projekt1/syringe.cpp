#include "Syringe.h"
#include <cstdlib>
#include <cmath>

Syringe::Syringe(float effectDuration) : effectDuration(effectDuration) {}

float Syringe::getEffectDuration() const {
    return effectDuration;
}

bool isFarEnoughh(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance) {
    return std::abs(position.x - heroPosition.x) >= minDistance && std::abs(position.y - heroPosition.y) >= minDistance;
}





void Syringe::spawnSyringe(std::vector<Syringe>& syringes, const sf::Texture& syringe_texture, const sf::RenderWindow& window, sf::Clock& syringeRespawnClock, const sf::Sprite& hero, float minDistance) {
    const int margin = 50;
    int maxX = window.getSize().x - margin;
    int maxY = window.getSize().y - margin;
    int minX = margin;
    int minY = margin;

    sf::Vector2f newPosition;
    do {
        int posX = rand() % (maxX - minX + 1) + minX;
        int posY = rand() % (maxY - minY + 1) + minY;
        newPosition = sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY));
    } while (!isFarEnoughh(newPosition, hero.getPosition(), minDistance));


    syringeRespawnClock.restart();
}
