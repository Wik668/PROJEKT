#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <functional>

// Klasa Zombie dziedzicząca po sf::Sprite, reprezentująca przeciwnika zombie w grze
class Zombie : public sf::Sprite {
public:
    enum Direction { Right, Left, Up, Down };

    // Konstruktor inicjalizujący zombie z podaną liczbą klatek na sekundę
    Zombie(int fps);

    // Metoda poruszająca zombie z uwzględnieniem kolizji
    void moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY);

    // Metody dodające klatki animacji w różnych kierunkach
    void add_animation_frame_right(const sf::IntRect& frame);
    void add_animation_frame_left(const sf::IntRect& frame);
    void add_animation_frame_up(const sf::IntRect& frame);
    void add_animation_frame_down(const sf::IntRect& frame);

    // Metoda dodająca wszystkie klatki animacji zombie
    void addZombieAnimationFrames();

    // Metoda aktualizująca stan zombie w grze
    void step();

    // Metody dostępu i modyfikacji zdrowia zombie
    int getHealth() const;
    void setHealth(int hp);
    void takeDamage(int damage);
    void heal(int amount);

    // Metoda klonująca zombie
    Zombie clone() const;

    // Statyczne metody do tworzenia zombie i sprawdzania kolizji z bohaterem
    static void createZombie(std::vector<std::unique_ptr<Zombie>>& zombies, sf::Texture& zombie_texture, sf::RenderWindow& window);
    static void checkHeroZombieCollisions(std::vector<std::unique_ptr<Zombie>>& zombies, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText);

private:
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

#endif // ZOMBIE_H
