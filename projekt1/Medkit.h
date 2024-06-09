#ifndef MEDKIT_H
#define MEDKIT_H

#include <SFML/Graphics.hpp>
#include <vector>

class Medkit : public sf::Sprite {
public:
    Medkit(float healingAmount);

    float getHealingAmount() const;

    static void createMedkit(std::vector<Medkit>& medkits, sf::Texture& medkit_texture, sf::RenderWindow& window, const sf::Sprite& hero, float minDistance);
    static void checkHeroMedkitCollisions(std::vector<Medkit>& medkits, sf::Sprite& hero, float& health, std::function<void()> updateHealthText);

private:
    float healingAmount;
};

#endif // MEDKIT_H
