#ifndef ANIMATED_SPRITE_H  // Sprawdza, czy ANIMATED_SPRITE_H nie jest już zdefiniowane
#define ANIMATED_SPRITE_H  // Definiuje ANIMATED_SPRITE_H

#include <SFML/Graphics.hpp>  // Dołącza bibliotekę SFML Graphics
#include <vector>  // Dołącza bibliotekę vector

using namespace sf;  // przestrzen nazw sf (SFML)
using namespace std;  // standardowej przestrzen nazw zeby nie uzywac std::

class AnimatedSprite : public Sprite {  // Definiuje klasę AnimatedSprite, która dziedziczy po klasie Sprite
private:
    vector<IntRect> framesRight;  // Wektor przechowujący ramki animacji poruszania się w prawo
    vector<IntRect> framesLeft;  // Wektor przechowujący ramki animacji poruszania się w lewo
    vector<IntRect> framesUp;  // Wektor przechowujący ramki animacji poruszania się do góry
    vector<IntRect> framesDown;  // Wektor przechowujący ramki animacji poruszania się w dół
    int currentFrame;  // Aktualna ramka animacji
    int animationFps;  // Liczba klatek na sekundę dla animacji
    Time frameTime;  // Czas trwania jednej ramki
    Clock clock;  // Zegar do śledzenia czasu
    enum Direction { Up, Down, Left, Right } direction;  // Wyliczenie kierunków

public:
    AnimatedSprite(int fps);  // Konstruktor klasy

    // Metody do dodawania ramek animacji dla różnych kierunków
    void add_animation_frame_right(const IntRect& frame);
    void add_animation_frame_left(const IntRect& frame);
    void add_animation_frame_up(const IntRect& frame);
    void add_animation_frame_down(const IntRect& frame);

    // Metody do dodawania ramek stojących dla różnych kierunków
    void add_standing_frame_right(const IntRect& frame);
    void add_standing_frame_left(const IntRect& frame);
    void add_standing_frame_up(const IntRect& frame);
    void add_standing_frame_down(const IntRect& frame);

    void step();  // Metoda do przesuwania animacji o jeden krok
    void moveWithCollision(const FloatRect& bounds, float offsetX, float offsetY);  // Metoda do poruszania się z detekcją kolizji

private:
    const vector<IntRect>& getFrames() const;  // Metoda do pobierania odpowiedniego wektora ramek w zależności od kierunku
};

#endif // ANIMATED_SPRITE_H
