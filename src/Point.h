#ifndef POINT_H
#define POINT_H

#include <SFML/Graphics.hpp>
template<typename T>
class Point {
private:
    sf::CircleShape point_shape;
    std::string colour = "";
    T value;

    void generateProperties();

public:
    explicit Point();
    void draw(sf::RenderTarget& target) const;
    [[nodiscard]] sf::Vector2f getPosition() const;
    [[nodiscard]] T getValue() const;

};

#include "Point.cpp"
#endif // POINT_H
