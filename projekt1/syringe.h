#ifndef SYRINGE_H
#define SYRINGE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Syringe : public sf::Sprite {
public:
    Syringe(float effectDuration);

    float getEffectDuration() const;

    static void initializeSyringes(sf::RenderWindow& window, sf::Texture& syringeTexture, std::vector<Syringe>& syringes);
    static void createSyringe(float x, float y, sf::Texture& syringeTexture, std::vector<Syringe>& syringes);
    static void checkHeroSyringeCollisions(sf::Sprite& hero, std::vector<Syringe>& syringes, float& moveSpeed, bool& invulnerable, sf::Clock& syringeClock);
    static void spawnSyringe(std::vector<Syringe>& syringes, const sf::Texture& syringeTexture, const sf::RenderWindow& window, sf::Clock& syringeRespawnClock, const sf::Sprite& hero, float minDistance);

private:
    float effectDuration;
};

#endif // SYRINGE_H
