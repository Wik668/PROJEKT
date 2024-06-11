#ifndef AMMO_H
#define AMMO_H

#include "GameObject.h"
#include <vector>
#include <functional>
#include <SFML/System.hpp>

class Ammo : public GameObject {
public:
    // Konstruktor klasy Ammo inicjalizujący obiekt na podstawie pozycji (x, y) oraz tekstury
    Ammo(float x, float y, const sf::Texture& texture);

    // Funkcja statyczna sprawdzająca, czy pozycja jest wystarczająco daleko od pozycji bohatera
    static bool isFarEnough(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance);

    // Funkcja statyczna odpowiedzialna za tworzenie nowych paczek amunicji
    static void spawnAmmo(std::vector<Ammo>& ammoPacks, const sf::Texture& ammoTexture, const sf::RenderWindow& window, sf::Clock& ammoRespawnClock, const sf::Sprite& hero, float minDistance);

    // Funkcja statyczna sprawdzająca kolizje między bohaterem a paczkami amunicji
    static void checkHeroAmmoCollisions(std::vector<Ammo>& ammoPacks, sf::Sprite& hero, bool& unlimitedAmmo, sf::Clock& unlimitedAmmoClock, bool& reloading, std::function<void()> updateAmmoText, bool& tripleShotActive, sf::Clock& tripleShotClock);

    // Metoda aktualizacji amunicji (implementacja wymagana przez klasę bazową)
    void update(float) override {}
};

#endif // AMMO_H
