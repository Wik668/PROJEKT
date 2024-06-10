#ifndef NINJA_H
#define NINJA_H

#include "SFML/Audio/Music.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

// Klasa Ninja dziedzicząca po sf::Sprite, reprezentująca postać ninja w grze
class Ninja : public sf::Sprite {
public:
    enum Direction { Right, Left, Up, Down };

    // Konstruktor inicjalizujący ninja z podaną liczbą klatek na sekundę
    Ninja(int fps);

    // Metoda poruszająca ninja z uwzględnieniem kolizji
    void moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY);

    // Metody dodające klatki animacji w różnych kierunkach
    void add_animation_frame_right(const sf::IntRect& frame);
    void add_animation_frame_left(const sf::IntRect& frame);
    void add_animation_frame_up(const sf::IntRect& frame);
    void add_animation_frame_down(const sf::IntRect& frame);

    // Metody dodające klatki animacji znikania i pojawiania się
    void add_disappear_frame(const sf::IntRect& frame);
    void add_reappear_frame(const sf::IntRect& frame);

    // Metoda dodająca wszystkie klatki animacji ninja
    void addNinjaAnimationFrames();

    // Metoda aktualizująca stan ninja w grze
    void step(const sf::Vector2f& heroPosition, const sf::FloatRect& windowBounds);

    // Metody dostępu i modyfikacji zdrowia ninja
    int getHealth() const;
    void setHealth(int hp);
    void takeDamage(int damage);
    void heal(int amount);

    // Statyczne metody do tworzenia ninja i sprawdzania kolizji z bohaterem
    static bool isFarEnoughninja(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance);
    static void createNinja(std::vector<Ninja>& ninjas, sf::Texture& ninja_texture, sf::RenderWindow& window, const sf::Vector2f& heroPosition);
    static void checkHeroNinjaCollisions(std::vector<Ninja>& ninjas, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText);

    // Metody do tworzenia i aktualizowania klonów ninja
    void createClone(const sf::Texture* ninja_texture);
    void updateClones();

    // Metoda dostępu do klonów ninja
    std::vector<Ninja> getClones() const;

private:
    std::vector<sf::IntRect> framesRight;
    std::vector<sf::IntRect> framesLeft;
    std::vector<sf::IntRect> framesUp;
    std::vector<sf::IntRect> framesDown;
    std::vector<sf::IntRect> framesDisappear;
    std::vector<sf::IntRect> framesReappear;

    std::vector<std::pair<Ninja, sf::Time>> clones; // Przechowywanie klonów z czasem ich utworzenia

    int currentFrame;
    int animationFps;
    int health;
    Direction direction;
    sf::Clock clock;
    sf::Clock teleportTimer;
    sf::Time frameTime;
    bool isDisappearing;
    bool isReappearing;

    // Metoda zwracająca aktualne klatki animacji
    const std::vector<sf::IntRect>& getFrames() const;

    // Metoda teleportująca ninja w kierunku bohatera
    void teleport(const sf::Vector2f& heroPosition, const sf::FloatRect& bounds);
};

#endif // NINJA_H
