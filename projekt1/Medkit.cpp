#include "Medkit.h"
#include <cstdlib>
#include <algorithm>

Medkit::Medkit(float healingAmount) : healingAmount(healingAmount) {}

float Medkit::getHealingAmount() const {
    return healingAmount;
}

void Medkit::initializeMedkits(sf::RenderWindow& window, sf::Texture& medkit_texture, std::vector<Medkit>& medkits) {
    for (int i = 0; i < 3; ++i) {
        const int margin = 50;
        int maxX = window.getSize().x - margin;
        int maxY = window.getSize().y - margin;
        int minX = margin;
        int minY = margin;

        // Generate random position with margin consideration
        int posX = rand() % (maxX - minX + 1) + minX;
        int posY = rand() % (maxY - minY + 1) + minY;

        // Create medkit at random position
        createMedkit(static_cast<float>(posX), static_cast<float>(posY), medkit_texture, medkits);
    }
}

void Medkit::createMedkit(float x, float y, sf::Texture& medkit_texture, std::vector<Medkit>& medkits) {
    Medkit medkit(20.0f); // Example healing amount
    medkit.setTexture(medkit_texture);
    medkit.setPosition(x, y);
    medkit.setScale(0.09f, 0.09f); // Scale if needed
    medkits.push_back(medkit);
}

void Medkit::checkHeroMedkitCollisions(sf::Sprite& hero, std::vector<Medkit>& medkits, float& health, void (*updateHealthText)()) {
    for (auto it = medkits.begin(); it != medkits.end();) {
        if (hero.getGlobalBounds().intersects(it->getGlobalBounds())) {
            health = std::min(100.0f, health + it->getHealingAmount());
            updateHealthText();
            it = medkits.erase(it); // Remove collected medkit
        } else {
            ++it;
        }
    }
}
