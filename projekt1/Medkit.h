#ifndef MEDKIT_H
#define MEDKIT_H

#include "GameObject.h"
#include <vector>
#include <functional>

class Medkit : public GameObject {
public:
    // Konstruktor inicjalizujący apteczkę z podaną ilością leczenia
    Medkit(float healingAmount);

    // Metoda zwracająca ilość leczenia oferowanego przez apteczkę
    float getHealingAmount() const;

    // Statyczna metoda tworząca nową apteczkę i dodająca ją do wektora apteczek
    static void createMedkit(std::vector<Medkit>& medkits, sf::Texture& medkit_texture, sf::RenderWindow& window, const sf::Sprite& hero, float minDistance);

    // Statyczna metoda sprawdzająca kolizje między bohaterem a apteczkami
    static void checkHeroMedkitCollisions(std::vector<Medkit>& medkits, sf::Sprite& hero, float& health, std::function<void()> updateHealthText);

    // Metoda aktualizacji apteczki (implementacja wymagana przez klasę bazową)
    void update(float) override {}

private:
    float healingAmount;
};

// Funkcja sprawdzająca, czy pozycja jest wystarczająco daleko od pozycji bohatera
bool isFarEnoughmedkit(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance);

#endif // MEDKIT_H
