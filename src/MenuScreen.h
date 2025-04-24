#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include "Button.h"
#include <SFML/Graphics.hpp>

class MenuScreen {
private:
    sf::Font menu_font;
    sf::Text title_text;
    Button play_button{"Play!", sf::Vector2f(960.f, 400.f), sf::Vector2f(500.f, 100.f)};
    Button quit_button{"Quit :*(", sf::Vector2f(960.f, 550.f), sf::Vector2f(500.f, 100.f)};
    bool active;

    void initMenuScreenFont();
    void initMenuScreenText();
public:
    explicit MenuScreen();
    [[nodiscard]] bool isMenuScreenActive() const;
    void update(sf::Vector2f mouse_pos);
    void render(sf::RenderWindow& target) const;
    void pollMenuScreenEvents(sf::RenderWindow& window, const sf::Event& event, const sf::Vector2f& mouse_pos);
};


#endif //MENUSCREEN_H
