#include "Medkit.h"
#include <cstdlib>
#include <cmath>

// Konstruktor klasy Medkit inicjalizujący apteczkę z podaną ilością leczenia
Medkit::Medkit(float healingAmount) : healingAmount(healingAmount) {}

// Metoda zwracająca ilość leczenia oferowanego przez apteczkę
float Medkit::getHealingAmount() const {
    return healingAmount;
}

// Funkcja sprawdzająca, czy pozycja jest wystarczająco daleko od pozycji bohatera
bool isFarEnoughmedkit(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance) {
    float distance = std::sqrt((position.x - heroPosition.x) * (position.x - heroPosition.x) + (position.y - heroPosition.y) * (position.y - heroPosition.y));
    return distance >= minDistance;
}

// Statyczna metoda tworząca nową apteczkę i dodająca ją do wektora apteczek
void Medkit::createMedkit(std::vector<Medkit>& medkits, sf::Texture& medkit_texture, sf::RenderWindow& window, const sf::Sprite& hero, float minDistance) {
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
    } while (!isFarEnoughmedkit(newPosition, hero.getPosition(), minDistance));

    Medkit medkit(20.0f); // Przykładowa ilość leczenia
    medkit.setTexture(medkit_texture);
    medkit.setPosition(newPosition);
    medkit.setScale(0.09f, 0.09f); // Skalowanie w razie potrzeby
    medkits.push_back(medkit);
}

// Statyczna metoda sprawdzająca kolizje między bohaterem a apteczkami
void Medkit::checkHeroMedkitCollisions(std::vector<Medkit>& medkits, sf::Sprite& hero, float& health, std::function<void()> updateHealthText) {
    for (auto it = medkits.begin(); it != medkits.end();) {
        if (hero.getGlobalBounds().intersects(it->getGlobalBounds())) {
            health = std::min(100.0f, health + it->getHealingAmount());
            updateHealthText();
            it = medkits.erase(it); // Usunięcie zebranej apteczki
        } else {
            ++it;
        }
    }
}
