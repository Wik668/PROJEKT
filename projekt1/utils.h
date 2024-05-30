#ifndef UTILS_H
#define UTILS_H

#include <SFML/System/Vector2.hpp>
#include <cmath>

inline sf::Vector2f normalize(sf::Vector2f vec) {
    float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
    if (length != 0) {
        return sf::Vector2f(vec.x / length, vec.y / length);
    } else {
        return sf::Vector2f(0, 0);
    }
}

#endif // UTILS_H
