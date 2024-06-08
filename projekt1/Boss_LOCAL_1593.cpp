#include "Boss.h"
#include "utils.h"

Boss::Boss(float speed) : speed(speed), health(500.0f), current_frame(0), animation_delay(0.1f) {
    animationClock.restart();
}

void Boss::shoot(std::vector<SlimeProjectile>& slimeProjectiles, const sf::Texture& projectileTexture, sf::Vector2f target) {
    if (shootClock.getElapsedTime().asSeconds() > 2.0f) {
        sf::Vector2f direction = normalize(target - getPosition());
        slimeProjectiles.emplace_back(projectileTexture, direction, 0.5f);
        slimeProjectiles.back().setPosition(getPosition());
        shootClock.restart();
    }
}

void Boss::shoot(std::vector<Fireball>& fireballs, const sf::Texture& fireballTexture, sf::Vector2f target) {
    if (shootClock.getElapsedTime().asSeconds() > 2.0f) {
        sf::Vector2f direction = normalize(target - getPosition());
        fireballs.emplace_back(fireballTexture, direction, 0.3f);
        fireballs.back().setPosition(getPosition());
        shootClock.restart();
    }
}

void Boss::step() {
    if (animationClock.getElapsedTime().asSeconds() > animation_delay) {
        if (!animation_frames_right.empty()) {
            setTextureRect(animation_frames_right[current_frame]);
            current_frame = (current_frame + 1) % animation_frames_right.size();
        }
        animationClock.restart();
    }
}

void Boss::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
       // dieAnimation(); //dodac funckje animacji smierci bossa
    }
}
void Boss::dieAnimation()
{
    // do uzupelnienia
}
float Boss::getHealth() const {
    return health;
}

void Boss::add_animation_frame_right(const sf::IntRect& frame) {
    animation_frames_right.push_back(frame);
}

void Boss::moveWithCollision(const sf::FloatRect& bounds, float dx, float dy) {
    sf::Vector2f newPos = getPosition() + sf::Vector2f(dx, dy);
    if (bounds.contains(newPos)) {
        setPosition(newPos);
    }
}
