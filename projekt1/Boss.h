#ifndef BOSS_H
#define BOSS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Dołączenie modułu SFML Audio
#include <vector>
#include "SlimeProjectile.h"
#include "Fireball.h"
#include <functional>

// Klasa Boss dziedzicząca po sf::Sprite
class Boss : public sf::Sprite {
public:
    // Konstruktor klasy Boss
    Boss(float speed);

    // Funkcje strzelania pociskami
    void shoot(std::vector<SlimeProjectile>& slimeProjectiles, const sf::Texture& projectileTexture, sf::Vector2f target);
    void shoot(std::vector<Fireball>& fireballs, const sf::Texture& fireballTexture, sf::Vector2f target);

    // Przejście do następnej klatki animacji
    void step();

    // Otrzymanie obrażeń
    void takeDamage(int damage);

    // Pobranie zdrowia
    float getHealth() const;

    // Dodawanie klatek animacji
    void add_animation_frame_right(const sf::IntRect& frame);
    void add_death_animation_frame(const sf::IntRect& frame);

    // Ruch z kolizjami
    void moveWithCollision(const sf::FloatRect& bounds, float dx, float dy);

    // Animacja śmierci
    void dieAnimation();

    // Sprawdzenie, czy boss powinien zostać usunięty
    bool shouldBeRemoved() const;

    // Sprawdzenie, czy boss umiera
    bool isDying() const;

    // Sprawdzenie, czy boss jest martwy
    bool isDead() const;

    // Sprawdzenie, czy boss może spawnować nowych wrogów
    bool canSpawnNewEnemies() const;

    // Statyczne funkcje pomocnicze
    static void addBossAnimationFrames(Boss& boss);
    static void createBoss(std::vector<Boss>& bosses, sf::Texture& boss_texture, sf::RenderWindow& window);
    static void checkHeroBossCollisions(std::vector<Boss>& bosses, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText);

private:
    float speed;                          // Prędkość bossa
    float health;                         // Zdrowie bossa
    std::size_t current_frame;            // Aktualna klatka animacji
    float animation_delay;                // Opóźnienie między klatkami animacji
    sf::Clock animationClock;             // Zegar animacji
    sf::Clock shootClock;                 // Zegar strzelania
    sf::Clock deathClock;                 // Zegar śmierci
    std::vector<sf::IntRect> animation_frames_right; // Klatki animacji w prawo
    std::vector<sf::IntRect> death_animation_frames; // Klatki animacji śmierci
    bool dying;                           // Flaga umierania
    bool dead;                            // Flaga śmierci
    float deathAnimationDuration;         // Czas trwania animacji śmierci
    float postDeathDelay;                 // Opóźnienie po śmierci

    // Funkcja sprawdzająca, czy odległość między dwoma punktami jest wystarczająca
    static bool isFarEnough(const sf::Vector2f& pos1, float minDistance);
};

#endif // BOSS_H
