#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(int fps) : currentFrame(0), animationFps(fps), direction(Right) {}

void AnimatedSprite::add_animation_frame_right(const IntRect& frame) {
    framesRight.push_back(frame);
}

void AnimatedSprite::add_animation_frame_left(const IntRect& frame) {
    framesLeft.push_back(frame);
}

void AnimatedSprite::add_animation_frame_up(const IntRect& frame) {
    framesUp.push_back(frame);
}

void AnimatedSprite::add_animation_frame_down(const IntRect& frame) {
    framesDown.push_back(frame);
}

void AnimatedSprite::add_standing_frame_right(const IntRect& frame) {
    framesRight.push_back(frame);
}

void AnimatedSprite::add_standing_frame_left(const IntRect& frame) {
    framesLeft.push_back(frame);
}

void AnimatedSprite::add_standing_frame_up(const IntRect& frame) {
    framesUp.push_back(frame);
}

void AnimatedSprite::add_standing_frame_down(const IntRect& frame) {
    framesDown.push_back(frame);
}

void AnimatedSprite::step() {
    if (Keyboard::isKeyPressed(Keyboard::A) ||
        Keyboard::isKeyPressed(Keyboard::D) ||
        Keyboard::isKeyPressed(Keyboard::W) ||
        Keyboard::isKeyPressed(Keyboard::S)) {

        frameTime += clock.restart();
        Time timePerFrame = seconds(1.0f / animationFps);

        while (frameTime >= timePerFrame) {
            frameTime -= timePerFrame;
            currentFrame = (currentFrame + 1) % (getFrames().size() - 1); // Skip standing frame
            setTextureRect(getFrames()[currentFrame]);
        }
    } else {
        setTextureRect(getFrames().back()); // Set the last frame as standing frame
    }
}

void AnimatedSprite::moveWithCollision(const FloatRect& bounds, float offsetX, float offsetY) {
    Vector2f oldPosition = getPosition();
    Sprite::move(offsetX, offsetY);
    FloatRect spriteBounds = getGlobalBounds();

    if (spriteBounds.left < bounds.left ||
        spriteBounds.top < bounds.top ||
        spriteBounds.left + spriteBounds.width > bounds.left + bounds.width ||
        spriteBounds.top + spriteBounds.height > bounds.top + bounds.height) {

        setPosition(oldPosition);
    } else {
        if (offsetX > 0) direction = Right;
        else if (offsetX < 0) direction = Left;
        else if (offsetY > 0) direction = Down;
        else if (offsetY < 0) direction = Up;
    }
}

const vector<IntRect>& AnimatedSprite::getFrames() const {
    switch (direction) {
    case Right: return framesRight;
    case Left: return framesLeft;
    case Up: return framesUp;
    case Down: return framesDown;
    }
    // To avoid warning, returning framesRight as a default case.
    return framesRight;
}
