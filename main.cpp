#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include <chrono>
#include <string>
#include <thread>
#include <valarray>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Player {
private:
    sf::Texture player_texture;
    sf::Sprite player_sprite;
    sf::Vector2f current_player_position;

    float player_movement_speed;
    void setPlayerSize(float const desiredWidth, float const desiredHeight) {
        sf::Vector2u const original_player_texture_size = player_texture.getSize();
        float const scaleX = desiredWidth / static_cast<float>(original_player_texture_size.x);
        float const scaleY = desiredHeight / static_cast<float>(original_player_texture_size.y);
        player_sprite.setScale(scaleX, scaleY);
    }
    void init_player() {
        if (!this->player_texture.loadFromFile("textures/player_texture.png")) {
            std::cerr << "Failed to load player texture!" << std::endl;
        }
        this->player_sprite.setTexture(this->player_texture);
        this->player_sprite.setOrigin(player_sprite.getGlobalBounds().width / 2.f, player_sprite.getGlobalBounds().height / 2.f);
        this->current_player_position.x = 500.f;
        this->current_player_position.y = 500.f;
        this->player_sprite.setPosition(this->current_player_position.x, this->current_player_position.y);
        this->player_movement_speed = 200.f;
        setPlayerSize(100.f, 300.f);
        // std::cout << player_sprite.getPosition().x << " " << player_sprite.getPosition().y << std::endl;
        // std::cout << player_sprite.getLocalBounds().width << " " << player_sprite.getLocalBounds().height << std::endl;
        // std::cout << player_sprite.getGlobalBounds().width << " " << player_sprite.getGlobalBounds().height << std::endl;
    }
    void movePlayer(sf::Vector2f normalized_direction, float deltaTime) {
        sf::Vector2f movement = normalized_direction * deltaTime * player_movement_speed;
        player_sprite.move(movement);

    }
    void rotatePlayer(sf::Vector2f normalized_direction) {
        float angle_deg = std::atan2(normalized_direction.y, normalized_direction.x) * 180.f / 3.141592653589f;
        angle_deg += 90.f;

        if (angle_deg < 0) {
            angle_deg += 360.f;
        }

        int const num_directions = 32;
        float angle_step = 360.f / num_directions;
        float new_angle = std::round(angle_deg / angle_step) * angle_step;
        if (new_angle >= 360.f) {
            new_angle -= 360;
        }
        this->player_sprite.setRotation(new_angle);
    }

public:
    Player(){
        this->init_player();
    }
    [[nodiscard]] sf::Sprite getPlayerSprite() {
        return this->player_sprite;
    }
    void updatePlayer(sf::Vector2f location, float deltaTime) {
        // location = mouse position
        sf::Vector2f current_position = this->player_sprite.getPosition();
        sf::Vector2f direction = location - current_position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 3.0f) {
            sf::Vector2f normalized_direction = direction / distance;
            // Moves the player towards the mouse cursor.
            this->movePlayer(normalized_direction, deltaTime);
            // Smoothly rotates the player sprite to "look" towards the mouse cursor.
            this->rotatePlayer(normalized_direction);
        }
    }
};
class Enemy {
private:
    sf::Texture enemy_texture;
    sf::Sprite enemy_sprite;
    int enemy_difficulty;
    int enemy_speed;
    int enemy_size;
    std::string enemy_type;
    void setEnemySize(float const desiredWidth, float const desiredHeight) {
        sf::Vector2u const original_enemy_texture_size = enemy_texture.getSize();
        float const scaleX = desiredWidth / static_cast<float>(original_enemy_texture_size.x);
        float const scaleY = desiredHeight / static_cast<float>(original_enemy_texture_size.y);
        enemy_sprite.setScale(scaleX, scaleY);
    }
    void initEnemy() {
        this->enemy_sprite.setPosition(sf::Vector2f(140.f, 140.f));
        if (!this->enemy_texture.loadFromFile("textures/fish_texture.png")) {
            std::cerr << "Failed to load texture" << std::endl;
        }
        enemy_sprite.setTexture(enemy_texture);
        setEnemySize(100.f, 50.f);
    }
public:
    [[nodiscard]] sf::Sprite getEnemySprite() const {
        return enemy_sprite;
    }
    explicit Enemy(const int difficulty_ = 1, int speed_ = 0, int size_ = 1, std::string enemy_type_ = "null") :
        enemy_difficulty(difficulty_), enemy_speed(speed_), enemy_size(size_), enemy_type(std::move(enemy_type_)) {
        this->initEnemy();
    };
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
class Game {
private:
    sf::RenderWindow* window;
    sf::Event event;
    sf::Clock clock;
    sf::VideoMode video_mode;
    sf::Vector2f mouse_position_window;
    Player player;
    // std::vector<Minigame> minigames;
    void initVariables()
    {
        this->window = nullptr;
    }
    void initWindow() {
        this->video_mode.height = 1080;
        this->video_mode.width = 1920;
        this->window = new sf::RenderWindow(this->video_mode, "SWIM!", sf::Style::Titlebar | sf::Style::Close);
        this->window->setFramerateLimit(60);
    }
    void updateMousePosition() {
        sf::Vector2i pixel_position = sf::Mouse::getPosition(*this->window);
        this->mouse_position_window = this->window->mapPixelToCoords(pixel_position);
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
    float updateDeltaTime() {
        return this->clock.restart().asSeconds();
    }
    void update() {
        // Relative to the screen
        //std::cout << "Mouse pos: " << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << std::endl;
        // Relative to the window
        // std::cout << "Mouse pos: " << sf::Mouse::getPosition(*this->window).x << " " << sf::Mouse::getPosition(*this->window).y << std::endl;
        this->pollEvents();
        this->updateMousePosition();
        this->player.updatePlayer(this->mouse_position_window, this->updateDeltaTime());
    }
    void render() {
        /*
         *-renders everything
         */
        this->window->clear(sf::Color(3, 177, 252, 255));
        // DRAW GAME
        //Enemy enemy1;
        //this->window->draw(enemy1.getEnemySprite());
        this->window->draw(player.getPlayerSprite());
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
