#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


class Enemy;
class Minigame;
class Game {
private:
    sf::RenderWindow* window;
    sf::Event event;
    sf::VideoMode video_mode;
    std::vector<Minigame> minigames;
    void initVariables()
    {
        this->window = nullptr;
    }
    void initWindow() {
        this->video_mode.height = 1080;
        this->video_mode.width = 1920;
        this->window = new sf::RenderWindow(this->video_mode, "SWIM!", sf::Style::Titlebar | sf::Style::Close);
    }
public:
    //Constructori / Destructori:
    Game() {
        this->initVariables();
        this->initWindow();
    }
    ~Game() {
        delete this->window;
    }
    [[nodiscard]] bool isRunning() const {
        return this->window->isOpen();
    }
    void update() {
        this->pollEvents();
    }
    void render() {
        /*
         *-renders everything
         */
        this->window->clear(sf::Color(255, 255, 255, 255));
        // DRAW GAME

        // ---
        this->window->display();
    }
    void pollEvents() {
        while (this->window->pollEvent(this->event)) {
            switch (this->event.type) {
                case sf::Event::Closed:
                    this->window->close();
                    break;
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::Escape == this->event.key.code) {
                        this->window->close();
                    }
                    break;
                default:
                    break;
            }
        }
    }
};



class Player {};

class Enemy {
private:
    int enemy_difficulty;
    int enemy_speed;
    int enemy_size;
    std::string enemy_type;
public:
    explicit Enemy(const int difficulty_ = 1, int speed_ = 0, int size_ = 1, const std::string& enemy_type_ = "null") :
        enemy_difficulty(difficulty_), enemy_speed(speed_), enemy_size(size_), enemy_type(enemy_type_) {};
    Enemy(const Enemy& other_enemy) :
        enemy_difficulty(other_enemy.enemy_difficulty), enemy_speed(other_enemy.enemy_speed),
        enemy_size(other_enemy.enemy_size) {}
    ~Enemy() = default;
    Enemy& operator=(const Enemy& other_enemy) {
        enemy_difficulty = other_enemy.enemy_difficulty;
        enemy_speed = other_enemy.enemy_speed;
        enemy_size = other_enemy.enemy_size;
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
        os << "has " << enemy.enemy_speed << "speed, " << enemy.enemy_size << "size, "
        << enemy.enemy_difficulty << "difficulty\n";
        return os;
    }
};

class Minigame {
private:
    const int max_enemies;
    const int minigame_difficulty;
    std::vector<Enemy> enemies;
public:
    explicit Minigame(const int max_enemies_ = 1, const int minigame_difficulty_ = 1, const std::vector<Enemy>& enemies_ = {}) :
    max_enemies(max_enemies_), minigame_difficulty(minigame_difficulty_), enemies(enemies_) {}
    friend std::ostream& operator<<(std::ostream& os, const Minigame& minigame) {
        os << "Minigame of difficulty " << minigame.minigame_difficulty << "has "
        << minigame.max_enemies << "max enemies\n";
        for (std::size_t i = 0; i < minigame.enemies.size(); i++) {
            os << "Enemy " << i << " " << minigame.enemies[i];
        }
        return os;
    }
};

int main() {
    // Init Game
    Game game;
    while (game.isRunning()) {
        //Update
        game.update();

        //Render
        game.render();
    }
    return 0;
}
