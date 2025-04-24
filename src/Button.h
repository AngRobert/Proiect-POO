#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>


class Button {
private:
    sf::Font button_font;
    sf::Text button_text;
    sf::RectangleShape button_shape;

    void initButtonFont();
    void initButtonText(const std::string& button_string);
    void initButtonShape(sf::Vector2f button_pos, sf::Vector2f button_size);
public:
    explicit Button(const std::string& button_string = "", sf::Vector2f button_pos = sf::Vector2f(0, 0),
        sf::Vector2f button_size = sf::Vector2f(0, 0));
    void renderButton(sf::RenderTarget& target) const;
    void updateButton(sf::Vector2f mouse_pos);
    [[nodiscard]] bool isHovered(sf::Vector2f mouse_position) const;
};


#endif //BUTTON_H
