#ifndef AMMO_H
#define AMMO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class Ammo : public sf::Sprite {
public:
    Ammo(float x, float y, const sf::Texture& texture);

    static void spawnAmmo(std::vector<Ammo>& ammoPacks, const sf::Texture& ammoTexture, const sf::RenderWindow& window, sf::Clock& ammoRespawnClock, const sf::Sprite& hero, float minDistance);
    static void checkHeroAmmoCollisions(std::vector<Ammo>& ammoPacks, sf::Sprite& hero, bool& unlimitedAmmo, sf::Clock& unlimitedAmmoClock, bool& reloading, std::function<void()> updateAmmoText, bool& tripleShotActive, sf::Clock& tripleShotClock);
    void setScale(float x, float y);
private:
    static bool isFarEnough(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance);
};

#endif // AMMO_H
