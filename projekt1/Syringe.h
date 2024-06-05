#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Syringe : public sf::Sprite {
private:
    float effectDuration;

public:
    Syringe(float effectDuration);

    float getEffectDuration() const;

    static void initializeSyringes(sf::RenderWindow& window, sf::Texture& syringe_texture, std::vector<Syringe>& syringes);
    static void createSyringe(float x, float y, sf::Texture& syringe_texture, std::vector<Syringe>& syringes);
    static void checkHeroSyringeCollisions(sf::Sprite& hero, std::vector<Syringe>& syringes, float& move_speed, bool& invulnerable, sf::Clock& syringeClock);
};
