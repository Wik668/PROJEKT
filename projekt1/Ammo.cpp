#include "Ammo.h"
#include <cstdlib>
#include <cmath>

// Konstruktor klasy Ammo inicjalizujący obiekt na podstawie pozycji (x, y) oraz tekstury
Ammo::Ammo(float x, float y, const sf::Texture& texture) {
    setTexture(texture);
    setPosition(x, y);
    setScale(0.05f, 0.05f); // Poprawione wywołanie ustawienia skali
}

// Funkcja statyczna sprawdzająca, czy pozycja jest wystarczająco daleko od pozycji bohatera
bool Ammo::isFarEnough(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance) {
    return std::abs(position.x - heroPosition.x) >= minDistance && std::abs(position.y - heroPosition.y) >= minDistance;
}

// Funkcja statyczna odpowiedzialna za tworzenie nowych paczek amunicji
void Ammo::spawnAmmo(std::vector<Ammo>& ammoPacks, const sf::Texture& ammoTexture, const sf::RenderWindow& window, sf::Clock& ammoRespawnClock, const sf::Sprite& hero, float minDistance) {
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
    } while (!isFarEnough(newPosition, hero.getPosition(), minDistance));

    Ammo newAmmo(newPosition.x, newPosition.y, ammoTexture);
    ammoPacks.push_back(newAmmo);
    ammoRespawnClock.restart();
}

// Funkcja statyczna sprawdzająca kolizje między bohaterem a paczkami amunicji
void Ammo::checkHeroAmmoCollisions(std::vector<Ammo>& ammoPacks, sf::Sprite& hero, bool& unlimitedAmmo, sf::Clock& unlimitedAmmoClock, bool& reloading, std::function<void()> updateAmmoText, bool& tripleShotActive, sf::Clock& tripleShotClock) {
    for (auto it = ammoPacks.begin(); it != ammoPacks.end();) {
        if (hero.getGlobalBounds().intersects(it->getGlobalBounds())) {
            unlimitedAmmo = true;
            tripleShotActive = true;
            unlimitedAmmoClock.restart();
            tripleShotClock.restart();
            it = ammoPacks.erase(it); // Usunięcie zebranej paczki amunicji
            if (reloading) {
                reloading = false;
                updateAmmoText();
            }
        } else {
            ++it;
        }
    }
}

// Funkcja ustawiająca skalę obiektu Ammo
void Ammo::setScale(float x, float y) {
    sf::Sprite::setScale(x, y);
}
