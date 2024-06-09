#ifndef SYRINGE_H
#define SYRINGE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "AnimatedSprite.h"

class Syringe : public sf::Sprite {
public:
    Syringe(float effectDuration);

    float getEffectDuration() const;

    static void spawnSyringe(std::vector<Syringe>& syringes, const sf::Texture& syringe_texture, const sf::RenderWindow& window, sf::Clock& syringeRespawnClock, const sf::Sprite& hero, float minDistance);
    static void checkHeroSyringeCollisions(std::vector<Syringe>& syringes, AnimatedSprite& hero, sf::Clock& syringeClock, float& move_speed, float& orgmove_speed, bool& invulnerable);

private:
    float effectDuration;
};

#endif // SYRINGE_H
