#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


// DE IMPLEMENTAT:
// clasele urmatoare: Enemy(contine elemente din clasele fish si frog), Game(pentru joc, normal), Minigame(contine elemente din clasele minigameurilor), etc.
class Game {
private:
    sf::RenderWindow window;
    sf::VideoMode video_mode;
public:
    //Constructori / Destructori:
    Game() {};
    ~Game() {};
};

class Player {};

int main() {

    sf::Window window(sf::VideoMode(800, 600), "My first game", sf::Style::Titlebar | sf::Style::Close);
    sf::Event ev;

    while (window.isOpen()) {
        while (window.pollEvent(ev)) {
            switch (ev.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (ev.key.code == sf::Keyboard::Escape) {
                        window.close();
                        break;
                    }
                default:
                    break;
            }
        }
    }
    return 0;
}
