#include "Boss.h"
#include "utils.h"
#include <cmath>
#include <functional>

// Konstruktor klasy Boss inicjalizujący podstawowe parametry
Boss::Boss(float speed)
    : speed(speed), health(500.0f), current_frame(0), animation_delay(0.1f),
    dying(false), dead(false), deathAnimationDuration(1.0f), postDeathDelay(1.0f) {
    animationClock.restart();
}

// Metoda strzelająca pociskiem (Fireball) w kierunku celu
void Boss::shoot(std::vector<Fireball>& fireballs, const sf::Texture& fireballTexture, sf::Vector2f target) {
    if (!dying && shootClock.getElapsedTime().asSeconds() > 2.0f) {
        sf::Vector2f direction = normalize(target - getPosition());
        fireballs.emplace_back(fireballTexture, direction, 0.9f);
        fireballs.back().setPosition(getPosition());
        shootClock.restart();
    }
}

// Metoda aktualizująca animację bossa
void Boss::step() {
    if (!dying) {
        if (animationClock.getElapsedTime().asSeconds() > animation_delay) {
            if (!animation_frames_right.empty()) {
                setTextureRect(animation_frames_right[current_frame]);
                current_frame = (current_frame + 1) % animation_frames_right.size();
            }
            animationClock.restart();
        }
    } else {
        if (animationClock.getElapsedTime().asSeconds() > animation_delay) {
            if (!death_animation_frames.empty()) {
                setTextureRect(death_animation_frames[current_frame]);
                if (current_frame < death_animation_frames.size() - 1) {
                    current_frame++;
                } else {
                    dead = true;
                }
            }
            animationClock.restart();
        }
    }
}

// Metoda odbierająca punkty zdrowia bossowi
void Boss::takeDamage(int damage) {
    if (!dying) {
        health -= damage;
        if (health <= 0) {
            dieAnimation();
        }
    }
}

// Metoda inicjalizująca animację śmierci bossa
void Boss::dieAnimation() {
    dying = true;
    dead = false;
    current_frame = 0;
    animationClock.restart();
    deathClock.restart();
}

// Metoda sprawdzająca, czy boss umiera
bool Boss::isDying() const {
    return dying;
}

// Metoda sprawdzająca, czy boss jest martwy
bool Boss::isDead() const {
    return dead;
}

// Metoda sprawdzająca, czy można tworzyć nowych wrogów po śmierci bossa
bool Boss::canSpawnNewEnemies() const {
    return deathClock.getElapsedTime().asSeconds() >= deathAnimationDuration + postDeathDelay;
}

// Metoda zwracająca aktualne zdrowie bossa
float Boss::getHealth() const {
    return health;
}

// Metody dodające klatki animacji w różnych kierunkach
void Boss::add_animation_frame_right(const sf::IntRect& frame) {
    animation_frames_right.push_back(frame);
}

void Boss::add_death_animation_frame(const sf::IntRect& frame) {
    death_animation_frames.push_back(frame);
}

// Metoda poruszająca bossa z uwzględnieniem kolizji z granicami okna
void Boss::moveWithCollision(const sf::FloatRect& bounds, float dx, float dy) {
    if (!dying) {
        sf::Vector2f newPos = getPosition() + sf::Vector2f(dx, dy);
        if (bounds.contains(newPos)) {
            setPosition(newPos);
        }
    }
}

// Metoda sprawdzająca, czy boss jest wystarczająco daleko od podanej pozycji
bool Boss::isFarEnough(const sf::Vector2f& pos1, float minDistance) {
    return std::hypot(pos1.x, pos1.y) > minDistance;
}

// Metoda dodająca wszystkie klatki animacji bossa
void Boss::addBossAnimationFrames(Boss& boss) {
    std::vector<sf::IntRect> frames = {
        sf::IntRect(108, 219, 90, 100),
        sf::IntRect(397, 221, 90, 100),
        sf::IntRect(685, 219, 90, 100),
        sf::IntRect(973, 220, 90, 100),
        sf::IntRect(1260, 220, 90, 100),
        sf::IntRect(1549, 220, 90, 100),
        sf::IntRect(1834, 220, 90, 100),
        sf::IntRect(2122, 220, 90, 100),
        sf::IntRect(2410, 220, 90, 100),
        sf::IntRect(2700, 220, 90, 100),
        sf::IntRect(2989, 220, 90, 100),
        sf::IntRect(3276, 220, 90, 100)
    };

    for (const auto& frame : frames) {
        boss.add_animation_frame_right(frame);
    }

    std::vector<sf::IntRect> deathFrames = {
        sf::IntRect(105, 660, 150, 150),
        sf::IntRect(394, 660, 150, 150),
        sf::IntRect(687, 660, 150, 150),
        sf::IntRect(971, 660, 150, 150),
        sf::IntRect(1258, 660, 150, 150),
        sf::IntRect(1557, 660, 150, 150),
        sf::IntRect(1830, 660, 150, 150),
        sf::IntRect(2115, 660, 150, 150),
        sf::IntRect(2404, 660, 150, 150),
        sf::IntRect(2692, 660, 150, 150),
        sf::IntRect(2980, 660, 150, 150),
        sf::IntRect(3267, 660, 150, 150),
        sf::IntRect(3559, 660, 150, 150),
        sf::IntRect(3847, 660, 150, 150),
        sf::IntRect(4140, 660, 150, 150),
        sf::IntRect(4431, 660, 150, 150),
        sf::IntRect(4716, 660, 150, 150),
        sf::IntRect(5005, 660, 150, 150),
        sf::IntRect(5295, 660, 150, 150),
        sf::IntRect(5579, 660, 150, 150),
        sf::IntRect(5868, 660, 150, 150),
    };

    for (const auto& frame : deathFrames) {
        boss.add_death_animation_frame(frame);
    }
}

// Metoda tworząca nowego bossa
void Boss::createBoss(std::vector<Boss>& bosses, sf::Texture& boss_texture, sf::RenderWindow& window) {
    const int margin = 300;
    int maxX = window.getSize().x - margin;
    int maxY = window.getSize().y - margin;
    int minX = margin;
    int minY = margin;

    sf::Vector2f newPosition;
    do {
        int posX = rand() % (maxX - minX + 1) + minX;
        int posY = rand() % (maxY - minY + 1) + minY;
        newPosition = sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY));
    } while (!Boss::isFarEnough(newPosition, 150.0f));

    Boss boss(5);
    boss.setTexture(boss_texture);
    Boss::addBossAnimationFrames(boss);
    boss.setTextureRect(sf::IntRect(108, 219, 64, 100)); // Ustawienie początkowej klatki
    boss.setScale(3, 3);
    boss.setPosition(newPosition);
    boss.step();
    bosses.push_back(boss);
}

// Metoda sprawdzająca kolizje bohatera z bossem
void Boss::checkHeroBossCollisions(std::vector<Boss>& bosses, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText) {
    for (auto& boss : bosses) {
        if (hero.getGlobalBounds().intersects(boss.getGlobalBounds())) {
            if (!invulnerable) {
                health -= damage * 0.03;
                updateHealthText();
                if (health <= 0.0) {
                    gameEnded = true;
                    gameMusic.stop();
                }
            }
        }
    }
}

// Metoda sprawdzająca, czy boss powinien zostać usunięty
bool Boss::shouldBeRemoved() const {
    return dead && deathClock.getElapsedTime().asSeconds() >= deathAnimationDuration + postDeathDelay;
}
