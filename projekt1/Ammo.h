#ifndef AMMO_H
#define AMMO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

// Klasa Ammo dziedzicząca po sf::Sprite
class Ammo : public sf::Sprite {
public:
    // Konstruktor klasy Ammo
    Ammo(float x, float y, const sf::Texture& texture);

    // Funkcja do tworzenia nowych paczek amunicji
    static void spawnAmmo(std::vector<Ammo>& ammoPacks, const sf::Texture& ammoTexture, const sf::RenderWindow& window, sf::Clock& ammoRespawnClock, const sf::Sprite& hero, float minDistance);

    // Funkcja do sprawdzania kolizji paczek amunicji z bohaterem
    static void checkHeroAmmoCollisions(std::vector<Ammo>& ammoPacks, sf::Sprite& hero, bool& unlimitedAmmo, sf::Clock& unlimitedAmmoClock, bool& reloading, std::function<void()> updateAmmoText, bool& tripleShotActive, sf::Clock& tripleShotClock);

    // Ustawienie skali dla obiektu Ammo
    void setScale(float x, float y);

private:
    // Funkcja sprawdzająca czy odległość między pozycjami jest wystarczająca
    static bool isFarEnough(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance);
};

#endif // AMMO_H
