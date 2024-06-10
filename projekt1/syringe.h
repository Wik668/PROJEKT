#ifndef SYRINGE_H
#define SYRINGE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "AnimatedSprite.h"

// Klasa Syringe dziedzicząca po sf::Sprite, reprezentująca strzykawkę w grze
class Syringe : public sf::Sprite {
public:
    // Konstruktor inicjalizujący strzykawkę z podanym czasem działania efektu
    Syringe(float effectDuration);

    // Metoda zwracająca czas działania efektu strzykawki
    float getEffectDuration() const;

    // Statyczna metoda do tworzenia strzykawek
    static void spawnSyringe(std::vector<Syringe>& syringes, const sf::Texture& syringe_texture, const sf::RenderWindow& window, sf::Clock& syringeRespawnClock, const sf::Sprite& hero, float minDistance);

    // Statyczna metoda do sprawdzania kolizji bohatera ze strzykawkami
    static void checkHeroSyringeCollisions(std::vector<Syringe>& syringes, AnimatedSprite& hero, sf::Clock& syringeClock, float& move_speed, float& orgmove_speed, bool& invulnerable);

private:
    float effectDuration; // Czas działania efektu strzykawki
};

#endif // SYRINGE_H
