#include "MenuScreen.h"

#include <iostream>

MenuScreen::MenuScreen() : active(true) {
    this->initMenuScreenFont();
    this->initMenuScreenText();
}

bool MenuScreen::isMenuScreenActive() const {
    return this->active;
}

void MenuScreen::initMenuScreenFont() {
    if (!this->menu_font.loadFromFile("textures/fonts/Comic_Sans_MS.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
}

void MenuScreen::initMenuScreenText() {
    this->title_text.setFont(menu_font);
    this->title_text.setCharacterSize(80);
    this->title_text.setFillColor(sf::Color::White);
    this->title_text.setString("SWIM!");
    this->title_text.setOrigin(this->title_text.getGlobalBounds().width / 2,
                                this->title_text.getGlobalBounds().height / 2);
    this->title_text.setPosition(960.f, 75.f);
}

void MenuScreen::render(sf::RenderWindow& target) const {
    target.clear(sf::Color(3, 177, 252, 255));
    target.draw(this->title_text);
    this->play_button.renderButton(target);
    this->quit_button.renderButton(target);
    target.display();
}

void MenuScreen::pollMenuScreenEvents(sf::RenderWindow& window, const sf::Event& event, const sf::Vector2f& mouse_pos) {
    switch (event.type) {
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (this->play_button.isHovered(mouse_pos)) {
                    this->active = false;
                }
                else if (this->quit_button.isHovered(mouse_pos)) {


                    window.close();
                }
            }
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            break;
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;
    }
}

void MenuScreen::update(const sf::Vector2f mouse_pos) {
    this->play_button.updateButton(mouse_pos);
    this->quit_button.updateButton(mouse_pos);
}
