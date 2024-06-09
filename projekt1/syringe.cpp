#include "Syringe.h"
#include <cstdlib>
#include <cmath>
#include "AnimatedSprite.h"

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

    Syringe newSyringe(5.0f);  // Effect duration of 5 seconds
    newSyringe.setTexture(syringe_texture);
    newSyringe.setPosition(newPosition);
    newSyringe.setScale(0.8f, 0.8f);  // Adjust scale if needed
    syringes.push_back(newSyringe);

    syringeRespawnClock.restart();
}

void Syringe::checkHeroSyringeCollisions(std::vector<Syringe>& syringes, AnimatedSprite& hero, sf::Clock& syringeClock, float& move_speed, float& orgmove_speed, bool& invulnerable) {
    for (auto it = syringes.begin(); it != syringes.end();) {
        if (hero.getGlobalBounds().intersects(it->getGlobalBounds())) {
             // Store the original move speed
            move_speed =0.2; // Double the movement speed
            invulnerable = true;
            syringeClock.restart();
            it = syringes.erase(it); // Remove the syringe after collision
        } else {
            ++it;
        }
    }
}
