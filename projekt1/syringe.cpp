#include "Syringe.h"
#include <cstdlib>
#include <cmath>
#include "AnimatedSprite.h"

// Konstruktor klasy Syringe
Syringe::Syringe(float effectDuration) : effectDuration(effectDuration) {}

// Metoda zwracająca czas trwania efektu
float Syringe::getEffectDuration() const {
    return effectDuration;
}

// Funkcja sprawdzająca, czy pozycja jest wystarczająco daleko od bohatera
bool isFarEnoughh(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance) {
    return std::abs(position.x - heroPosition.x) >= minDistance && std::abs(position.y - heroPosition.y) >= minDistance;
}

// Metoda tworząca nową strzykawkę
void Syringe::spawnSyringe(std::vector<Syringe>& syringes, const sf::Texture& syringe_texture, const sf::RenderWindow& window, sf::Clock& syringeRespawnClock, const sf::Sprite& hero, float minDistance) {
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
    } while (!isFarEnoughh(newPosition, hero.getPosition(), minDistance));

    Syringe newSyringe(5.0f);  // Czas trwania efektu 5 sekund
    newSyringe.setTexture(syringe_texture);
    newSyringe.setPosition(newPosition);
    newSyringe.setScale(0.8f, 0.8f);  // Dostosowanie skali jeśli potrzebne
    syringes.push_back(newSyringe);

    syringeRespawnClock.restart();
}

// Metoda sprawdzająca kolizje strzykawek z bohaterem
void Syringe::checkHeroSyringeCollisions(std::vector<Syringe>& syringes, AnimatedSprite& hero, sf::Clock& syringeClock, float& move_speed, float& orgmove_speed, bool& invulnerable) {
    for (auto it = syringes.begin(); it != syringes.end();) {
        if (hero.getGlobalBounds().intersects(it->getGlobalBounds())) {
            // Ustawienie nowej prędkości ruchu
            orgmove_speed=0.45f;
            move_speed = 0.9f; // Podwojenie prędkości ruchu
            invulnerable = true;
            syringeClock.restart();
            it = syringes.erase(it); // Usunięcie strzykawki po kolizji
        } else {
            ++it;
        }
    }
}
