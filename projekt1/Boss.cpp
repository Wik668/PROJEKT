#include "Boss.h"
#include "utils.h"

Boss::Boss(float speed) : speed(speed), health(500.0f), current_frame(0), animation_delay(0.1f),
    dying(false), dead(false), deathAnimationDuration(1.0f), postDeathDelay(5.0f) {
    animationClock.restart();
}

void Boss::shoot(std::vector<SlimeProjectile>& slimeProjectiles, const sf::Texture& projectileTexture, sf::Vector2f target) {
    if (!dying && shootClock.getElapsedTime().asSeconds() > 2.0f) {
        sf::Vector2f direction = normalize(target - getPosition());
        slimeProjectiles.emplace_back(projectileTexture, direction, 0.5f);
        slimeProjectiles.back().setPosition(getPosition());
        shootClock.restart();
    }
}

void Boss::shoot(std::vector<Fireball>& fireballs, const sf::Texture& fireballTexture, sf::Vector2f target) {
    if (!dying && shootClock.getElapsedTime().asSeconds() > 2.0f) {
        sf::Vector2f direction = normalize(target - getPosition());
        fireballs.emplace_back(fireballTexture, direction, 0.3f);
        fireballs.back().setPosition(getPosition());
        shootClock.restart();
    }
}

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

void Boss::takeDamage(int damage) {
    if (!dying) {
        health -= damage;
        if (health <= 0) {
            dieAnimation();
        }
    }
}

void Boss::dieAnimation() {
    dying = true;
    dead = false;
    current_frame = 0;
    animationClock.restart();
    deathClock.restart(); // Restart the death clock
}

bool Boss::isDying() const {
    return dying;
}

bool Boss::isDead() const {
    return dead;
}

bool Boss::canSpawnNewEnemies() const {
    return deathClock.getElapsedTime().asSeconds() >= deathAnimationDuration + postDeathDelay;
}

float Boss::getHealth() const {
    return health;
}

void Boss::add_animation_frame_right(const sf::IntRect& frame) {
    animation_frames_right.push_back(frame);
}

void Boss::add_death_animation_frame(const sf::IntRect& frame) {
    death_animation_frames.push_back(frame);
}

void Boss::moveWithCollision(const sf::FloatRect& bounds, float dx, float dy) {
    if (!dying) {
        sf::Vector2f newPos = getPosition() + sf::Vector2f(dx, dy);
        if (bounds.contains(newPos)) {
            setPosition(newPos);
        }
    }
}
