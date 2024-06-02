#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Medkit : public sf::Sprite {
private:
    float healingAmount;

public:
    Medkit(float healingAmount);

    float getHealingAmount() const;

    static void initializeMedkits(sf::RenderWindow& window, sf::Texture& medkit_texture, std::vector<Medkit>& medkits);
    static void createMedkit(float x, float y, sf::Texture& medkit_texture, std::vector<Medkit>& medkits);
    static void checkHeroMedkitCollisions(sf::Sprite& hero, std::vector<Medkit>& medkits, float& health, void (*updateHealthText)());
};
