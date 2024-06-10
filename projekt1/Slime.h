#ifndef SLIME_H
#define SLIME_H

#include "SlimeProjectile.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <functional>

// Klasa Slime dziedzicząca po sf::Sprite, reprezentująca przeciwnika slime w grze
class Slime : public sf::Sprite {
public:
    enum Direction { Right, Left, Up, Down };

    // Konstruktor inicjalizujący slime z podaną liczbą klatek na sekundę
    Slime(int fps);

    // Metoda poruszająca slime z uwzględnieniem kolizji
    void moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY);

    // Metody dodające klatki animacji w różnych kierunkach
    void add_animation_frame_right(const sf::IntRect& frame);
    void add_animation_frame_left(const sf::IntRect& frame);
    void add_animation_frame_up(const sf::IntRect& frame);
    void add_animation_frame_down(const sf::IntRect& frame);

    // Metoda dodająca wszystkie klatki animacji slime
    void addSlimeAnimationFrames();

    // Metoda aktualizująca stan slime w grze
    void step();

    // Metody dostępu i modyfikacji zdrowia slime
    int getHealth() const;
    void setHealth(int hp);
    void takeDamage(int damage);
    void heal(int amount);

    // Metoda klonująca slime
    Slime clone() const;

    // Metoda strzelająca pociskiem slime
    void shoot(std::vector<SlimeProjectile>& slimeProjectiles, const sf::Texture& projectileTexture, sf::Vector2f target);

    // Statyczne metody do tworzenia slime i sprawdzania kolizji z bohaterem
    static void createSlime(std::vector<Slime>& slimes, sf::Texture& slime_texture, sf::RenderWindow& window, const sf::Vector2f& heroPosition);
    static void checkHeroSlimeCollisions(std::vector<Slime>& slimes, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText);

private:
    sf::Clock shootClock;
    std::vector<sf::IntRect> framesRight;
    std::vector<sf::IntRect> framesLeft;
    std::vector<sf::IntRect> framesUp;
    std::vector<sf::IntRect> framesDown;

    // Metoda zwracająca aktualne klatki animacji
    const std::vector<sf::IntRect>& getFrames() const;

    int currentFrame;
    int animationFps;
    int health;
    int speed;
    Direction direction;
    sf::Clock clock;
    sf::Time frameTime;

    // Statyczna metoda sprawdzająca, czy dwie pozycje są wystarczająco daleko od siebie
    static bool isFarEnough(const sf::Vector2f& pos1, const sf::Vector2f& pos2, float minDistance);
};

#endif // SLIME_H
