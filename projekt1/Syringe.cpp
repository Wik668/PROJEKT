#include "Syringe.h"
#include <cstdlib>
#include <algorithm>

Syringe::Syringe(float effectDuration) : effectDuration(effectDuration) {}

float Syringe::getEffectDuration() const {
    return effectDuration;
}

void Syringe::initializeSyringes(sf::RenderWindow& window, sf::Texture& syringe_texture, std::vector<Syringe>& syringes) {
    for (int i = 0; i < 3; ++i) {
        const int margin = 50;
        int maxX = window.getSize().x - margin;
        int maxY = window.getSize().y - margin;
        int minX = margin;
        int minY = margin;

        int posX = rand() % (maxX - minX + 1) + minX;
        int posY = rand() % (maxY - minY + 1) + minY;

        createSyringe(static_cast<float>(posX), static_cast<float>(posY), syringe_texture, syringes);
    }
}

void Syringe::createSyringe(float x, float y, sf::Texture& syringe_texture, std::vector<Syringe>& syringes) {
    Syringe syringe(5.0f); // Effect duration is 5 seconds
    syringe.setTexture(syringe_texture);
    syringe.setPosition(x, y);
    syringe.setScale(0.09f, 0.09f); // Scale if needed
    syringes.push_back(syringe);
}

void Syringe::checkHeroSyringeCollisions(sf::Sprite& hero, std::vector<Syringe>& syringes, float& move_speed, bool& invulnerable, sf::Clock& syringeClock) {
    for (auto it = syringes.begin(); it != syringes.end();) {
        if (hero.getGlobalBounds().intersects(it->getGlobalBounds())) {
            move_speed *= 2;
            invulnerable = true;
            syringeClock.restart();
            it = syringes.erase(it); // Remove collected Syringe
        } else {
            ++it;
        }
    }
}
