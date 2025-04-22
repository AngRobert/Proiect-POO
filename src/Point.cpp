#include "Point.h"
#include <random> // For generating random position

void Point::generatePosition() {
    // Makes sure the point cannot spawn under the rhythm circle
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribWidth(50, 1870);
    std::uniform_int_distribution<> distribHeight(50, 1030);
    float X, Y;
    do {
        X = static_cast<float>(distribWidth(gen));
        Y = static_cast<float>(distribHeight(gen));
    } while (X >= 810.f && X <= 1110.f && Y >= 0.f && Y <= 310.f);

    point_shape.setPosition(X, Y);
}

Point::Point() {
    // Constructor implementation
    this->point_shape.setRadius(7);
    this->point_shape.setOrigin(this->point_shape.getGlobalBounds().width / 2,
                                this->point_shape.getGlobalBounds().height / 2);
    this->point_shape.setFillColor(sf::Color(243, 255, 13, 255));
    this->generatePosition(); // Generate initial position
}

void Point::draw(sf::RenderTarget& target) const {
    target.draw(point_shape); // Draw the point on the target
}

sf::Vector2f Point::getPosition() const {
    return this->point_shape.getPosition(); // Return the position of the point
}
