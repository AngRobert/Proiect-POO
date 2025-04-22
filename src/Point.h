#ifndef POINT_H
#define POINT_H

#include <SFML/Graphics.hpp> // For sf::CircleShape and sf::RenderTarget

class Point {
private:
    sf::CircleShape point_shape;

    void generatePosition(); // Declared here, defined in Point.cpp

public:
    explicit Point(); // Constructor
    void draw(sf::RenderTarget& target) const; // Draw the point
    [[nodiscard]] sf::Vector2f getPosition() const; // Get position of the point
};

#endif // POINT_H
