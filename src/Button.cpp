#include "Button.h"
#include "GameExceptions.h"
#include <string>

Button::Button(const std::string& button_string, const sf::Vector2f button_pos, const sf::Vector2f button_size,
    const sf::Color button_color) {
    this->initButtonFont();
    this->initButtonShape(button_pos, button_size, button_color);
    this->initButtonText(button_string);
}

void Button::initButtonShape(const sf::Vector2f button_pos, const sf::Vector2f button_size, const sf::Color button_color) {
    this->button_shape.setOutlineColor(sf::Color::Black);
    this->button_shape.setOutlineThickness(1);
    this->button_shape.setFillColor(button_color);
    this->button_shape.setSize(button_size);
    this->button_shape.setOrigin(this->button_shape.getGlobalBounds().width / 2,
                                    this->button_shape.getGlobalBounds().height / 2);
    this->button_shape.setPosition(button_pos);
}

void Button::renderButton(sf::RenderTarget& target) const {
    target.draw(this->button_shape);
    target.draw(this->button_text);
}

void Button::updateButton(const sf::Vector2f mouse_pos) {
    if (this->isHovered(mouse_pos)) {
        this->button_text.setFillColor(sf::Color::Black);
    }
    else {
        this->button_text.setFillColor(sf::Color::White);
    }
}

void Button::initButtonFont() {
    if (!this->button_font.loadFromFile("textures/fonts/Comic_Sans_MS.ttf")) {
        throw ResourceLoadException{"Failed to load button font!"};
    }
}

void Button::initButtonText(const std::string& button_string) {
    button_text.setFont(this->button_font);
    button_text.setCharacterSize(40);
    button_text.setString(button_string);
    button_text.setOrigin(button_text.getGlobalBounds().width / 2, button_text.getGlobalBounds().height / 2);
    // const sf::FloatRect button_bounds = this->button_shape.getGlobalBounds();
    button_text.setPosition(sf::Vector2f(this->button_shape.getPosition().x,
                                        this->button_shape.getPosition().y));
    button_text.setFillColor(sf::Color::White);
}

bool Button::isHovered(const sf::Vector2f mouse_position) const {
    const sf::FloatRect button_bounds = this->button_shape.getGlobalBounds();
    sf::Vector2f button_pos = this->button_shape.getPosition();
    button_pos.x -= button_bounds.width / 2;
    button_pos.y -= button_bounds.height / 2;

    return (mouse_position.x >= button_pos.x && mouse_position.x <= button_pos.x + button_bounds.width &&
            mouse_position.y >= button_pos.y && mouse_position.y <= button_pos.y + button_bounds.height);
}
