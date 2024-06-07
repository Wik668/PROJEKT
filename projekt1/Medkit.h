#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Medkit : public sf::Sprite {
public:
    explicit Medkit(float healingAmount);
    float getHealingAmount() const;

    static void createMedkit(std::vector<Medkit>& medkits, sf::Texture& medkit_texture,
    sf::RenderWindow& window, const sf::Sprite& hero, float minDistance);

private:
    float healingAmount;
};
