#include "Medkit.h"
#include <cstdlib>
#include <cmath>

Medkit::Medkit(float healingAmount) : healingAmount(healingAmount) {}

float Medkit::getHealingAmount() const {
    return healingAmount;
}

bool isFarEnoughmedkit(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance) {
    float distance = std::sqrt((position.x - heroPosition.x) * (position.x - heroPosition.x) + (position.y - heroPosition.y) * (position.y - heroPosition.y));
    return distance >= minDistance;
}

void Medkit::createMedkit(std::vector<Medkit>& medkits, sf::Texture& medkit_texture, sf::RenderWindow& window, const sf::Sprite& hero, float minDistance) {
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
    } while (!isFarEnoughmedkit(newPosition, hero.getPosition(), minDistance));

    Medkit medkit(20.0f); // Example healing amount
    medkit.setTexture(medkit_texture);
    medkit.setPosition(newPosition);
    medkit.setScale(0.09f, 0.09f); // Scale if needed
    medkits.push_back(medkit);
}
