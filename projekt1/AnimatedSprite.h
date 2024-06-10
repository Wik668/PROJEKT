#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

// Klasa AnimatedSprite dziedzicząca po sf::Sprite
class AnimatedSprite : public Sprite {
public:
    // Kierunki animacji
    enum Direction { Right, Left, Up, Down };

    // Konstruktor klasy AnimatedSprite
    AnimatedSprite(int fps);

    // Dodawanie klatek animacji dla różnych kierunków
    void add_animation_frame_right(const IntRect& frame);
    void add_animation_frame_left(const IntRect& frame);
    void add_animation_frame_up(const IntRect& frame);
    void add_animation_frame_down(const IntRect& frame);

    // Dodawanie klatek stania dla różnych kierunków
    void add_standing_frame_right(const IntRect& frame);
    void add_standing_frame_left(const IntRect& frame);
    void add_standing_frame_up(const IntRect& frame);
    void add_standing_frame_down(const IntRect& frame);

    // Przejście do następnej klatki animacji
    void step();

    // Ruch z kolizjami
    void moveWithCollision(const FloatRect& bounds, float offsetX, float offsetY);

    // Inicjalizacja bohatera
    void initializeHero(const Texture& character_texture);

private:
    vector<IntRect> framesRight;  // Klatki animacji w prawo
    vector<IntRect> framesLeft;   // Klatki animacji w lewo
    vector<IntRect> framesUp;     // Klatki animacji w górę
    vector<IntRect> framesDown;   // Klatki animacji w dół
    Clock clock;                  // Zegar do kontroli czasu animacji
    Time frameTime;               // Czas trwania jednej klatki animacji
    int currentFrame;             // Aktualna klatka animacji
    int animationFps;             // Liczba klatek na sekundę
    Direction direction;          // Aktualny kierunek animacji
public:
    // Pobieranie klatek animacji
    const vector<IntRect>& getFrames() const;

    // Ustawienie stanu nietykalności
    void setInvulnerable(bool invulnerable) {
        this->invulnerable = invulnerable;
    }

    // Sprawdzenie stanu nietykalności
    bool isInvulnerable() const {
        return invulnerable;
    }

private:
    bool invulnerable = false;  // Flaga nietykalności
};
