#ifndef MEDKIT_H
#define MEDKIT_H

#include <SFML/Graphics.hpp>
#include <vector>

// Klasa Medkit reprezentująca apteczkę w grze
class Medkit : public sf::Sprite {
public:
    // Konstruktor inicjalizujący apteczkę z podaną ilością leczenia
    Medkit(float healingAmount);

    // Metoda zwracająca ilość leczenia
    float getHealingAmount() const;

    // Statyczna metoda tworząca apteczkę
    static void createMedkit(std::vector<Medkit>& medkits, sf::Texture& medkit_texture, sf::RenderWindow& window, const sf::Sprite& hero, float minDistance);

    // Statyczna metoda sprawdzająca kolizje między bohaterem a apteczkami
    static void checkHeroMedkitCollisions(std::vector<Medkit>& medkits, sf::Sprite& hero, float& health, std::function<void()> updateHealthText);

private:
    float healingAmount; // Ilość leczenia oferowanego przez apteczkę
};

#endif // MEDKIT_H
