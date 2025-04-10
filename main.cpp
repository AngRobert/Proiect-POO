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
    sf::Clock strokeTimer{};
    std::string previous_stroke;
    std::string current_stroke;
    int strokes_counter = 0;
    int fatigue_counter = 0;
    bool alive = true;
    bool next_stroke_is_breath = false;
    float player_movement_speed;
    float breath;
    void initPlayer() {
        if (!this->player_texture.loadFromFile("textures/player_texture.png")) {
            std::cerr << "Failed to load player texture!" << std::endl;
        }
        this->player_sprite.setTexture(this->player_texture);
        this->player_sprite.setOrigin(player_sprite.getGlobalBounds().width / 2.f, 0.f);
        this->current_player_position.x = 500.f;
        this->current_player_position.y = 500.f;
        this->player_sprite.setPosition(this->current_player_position.x, this->current_player_position.y);
        sf::Vector2u const original_player_texture_size = player_texture.getSize();
        float const scaleX = 100 / static_cast<float>(original_player_texture_size.x);
        float const scaleY = 300 / static_cast<float>(original_player_texture_size.y);
        player_sprite.setScale(scaleX, scaleY);
        std::cout << *this;
    }
    void movePlayer(sf::Vector2f const normalized_direction, float const deltaTime) {
        sf::Vector2f const movement = normalized_direction * deltaTime * player_movement_speed;
        player_sprite.move(movement);
    }
    void updateBreath() {

        /* BREATH LOGIC
         * IF NOT STROKING, GOES DOWN BY 3?/SEC. IF STROKE, GOES DOWN BY 5/SEC. DONE
         * FOR EACH CONSECUTIVE STROKE WITHOUT BREATHING, RATE GOES UP BY 5/SEC. THIS WAY, HAVING MORE STROKES/BREATH IS MORE RISKY DONE
         * SHOULD ALSO STAY INCREASED IF SAY THERE WERE >4 STROKES IN A ROW (STROKE RATE THAT HIGH ISN'T SUSTAINABLE OR SOMETHING)
         * ALSO, FOR EACH STROKE IN A ROW, SPEED SHOULD ALSO INCREASE.
         * CASES IN WHICH FATIGUE SHOULD PE APPLIED:
         *      STROKING THE SAME ARM MORE THAN 3 TIMES IN A ROW (IN THIS CASE SPEED SHOULD ALSO NOT GO UP)
         * OBVIOUSLY, IF BREATH GOES BELOW 0, PLAYER IS DEAD.
         */

        this->breath -= 0.05f; //   3 / sec

        // checks if player should be alive
        if (this->breath < 0) {
            this->alive = false;
        }

        if (this->isPlayerMoving()) {
            this->breath -= static_cast<float>(this->strokes_counter) * 0.0833333f; // 5 / sec
        }
    }
    void rotatePlayer(sf::Vector2f normalized_direction) {
        float angle_deg = std::atan2(normalized_direction.y, normalized_direction.x) * 180.f / 3.141592653589f;
        angle_deg += 90.f;

        if (angle_deg < 0) {
            angle_deg += 360.f;
        }

        constexpr int num_directions = 32;
        float angle_step = 360.f / num_directions;
        float new_angle = std::round(angle_deg / angle_step) * angle_step;
        if (new_angle >= 360.f) {
            new_angle -= 360;
        }
        this->player_sprite.setRotation(new_angle);
    }
    bool isPlayerMoving() const {
        return this->player_movement_speed > 50.f;
    }
public:
    explicit Player(float const player_movement_speed_ = 0.f) : player_movement_speed(player_movement_speed_), breath(100.f) {
        this->initPlayer();
    }
    [[nodiscard]] sf::Sprite getPlayerSprite() const {
        return this->player_sprite;
    }
    [[nodiscard]] float getBreath() const {
        return this->breath;
    }
    void doBreath() {
        this->next_stroke_is_breath = true;
    }
    void updatePlayer(sf::Vector2f location, float deltaTime) {
        // location = mouse position
        std::cout << *this;
        this->updateBreath();
        if (strokeTimer.getElapsedTime().asSeconds() > 2.f && this->isPlayerMoving()) {
            std::cout << "Two seconds have passed since the last stroke" << "\n";
            // this->strokes_counter = 0;
            this->player_movement_speed = 50.f;
        }
        sf::Vector2f current_position = this->player_sprite.getPosition();
        sf::Vector2f direction = location - current_position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 10.0f) {
            sf::Vector2f normalized_direction = direction / distance;
            // Moves the player towards the mouse cursor.
            this->movePlayer(normalized_direction, deltaTime);
            // Rotates the player sprite to "look" towards the mouse cursor.
            this->rotatePlayer(normalized_direction);
        }
    }
    bool isAlive() const {
        return this->alive;
    }
    void doStroke(sf::Keyboard::Key const key) {
        if (this->strokeTimer.getElapsedTime().asSeconds() >= 1.0f) {       // IF THE STROKE IS "VALID" (NOT SPAMMED)
            this->strokeTimer.restart();

            if (key == sf::Keyboard::Q) {
                this->current_stroke = "left";
            }
            else {
                this->current_stroke = "right";
            }

            if (this->next_stroke_is_breath) {
                this->breath += 35.f;
                next_stroke_is_breath = false;
                this->player_movement_speed -= 40.f;
                this->strokes_counter = 0;
            }

            this->strokes_counter ++;
            if (!this->isPlayerMoving()) {           // IF THE PLAYER STOPPED MOVING BY NOT STROKING IN 2 SECONDS
                this->player_movement_speed = 150.f;    // THE SPEED RESETS TO THE NORMAL
            }
            else {
                if (this->current_stroke == this->previous_stroke) {
                    // SPEED SHOULDN'T CHANGE, FATIGUE INCREASES
                    this->fatigue_counter ++;
                }
                else {
                    // SPEED GROWS, SO DOES THE RATE AT WHICH THE BREATH GOES DOWN
                    this->fatigue_counter = 0;
                    this->player_movement_speed += 20.f;
                }
            }
            this->previous_stroke = this->current_stroke;

        }

    }
    friend std::ostream& operator<<(std::ostream& os, const Player& player) {
        std::cout << "Player started at location " << player.current_player_position.x << "," << player.current_player_position.y
        << ", has speed: " << player.player_movement_speed << std::endl;
        return os;
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
        enemy_sprite = other_enemy.enemy_sprite;
        enemy_texture = other_enemy.enemy_texture;
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
    sf::Font font;
    sf::Text breath_text;
    sf::Text breath_value;
    sf::Text game_over;
    sf::RenderWindow* window;
    sf::Event event{};
    sf::Clock clock;
    sf::VideoMode video_mode;
    sf::Vector2f mouse_position_window;
    Player player;
    bool end_game = false;
    // std::vector<Minigame> minigames;
    void initFont() {
        if (!this->font.loadFromFile("textures/fonts/Comic_Sans_MS.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }
    }
    void initText(sf::Text& text, sf::Vector2f const text_position, const std::string& text_string) {
        text.setFont(this->font);
        text.setPosition(text_position);
        text.setCharacterSize(35);
        text.setFillColor(sf::Color::White);
        text.setString(text_string);
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
    void updateText() {
        this->breath_value.setString(std::to_string(static_cast<int>(this->player.getBreath())));
    }
    void renderText() const {
        this->window->draw(this->breath_text);
        this->window->draw(this->breath_value);
    }
    void renderPlayer() const {
        this->window->draw(this->player.getPlayerSprite());
    }

public:
    explicit Game() : window(nullptr) {
        this->initWindow();
        this->initFont();
        sf::Vector2f text_pos{1730.f, 400.f};
        this->initText(breath_text, text_pos, "BREATH:");
        text_pos.y = 440.f;
        this->initText(breath_value, text_pos, "");
        text_pos.x = 860.f;
        text_pos.y = 510.f;
        this->initText(game_over, text_pos, "GAME OVER !");
    }
    ~Game() {
        delete this->window;
    }
    [[nodiscard]] bool isRunning() const {
        return this->window->isOpen();
    }
    [[nodiscard]] bool isGameOver() const {
        return this->end_game;
    }

    float updateDeltaTime() {
        return this->clock.restart().asSeconds();
    }
    void renderGameOver() {
        this->window->clear(sf::Color(255, 0, 0, 255));
        this->window->draw(game_over);
        this->window->display();
    }
    void update() {
        this->pollEvents();
        if (this->player.isAlive()) {
            this->updateMousePosition();
            this->updateText();
            this->player.updatePlayer(this->mouse_position_window, this->updateDeltaTime());
        }
        else {
            this->end_game = true;
        }
    }
    void render() const {
        /*
         *-renders everything
         */
        this->window->clear(sf::Color(3, 177, 252, 255));
        this->renderPlayer();
        this->renderText();
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
                    else if (sf::Keyboard::W == this->event.key.code) {
                        this->player.doBreath();
                    }
                    else if (sf::Keyboard::Q == this->event.key.code || sf::Keyboard::E == this->event.key.code) {
                        this->player.doStroke(this->event.key.code);
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
        if (!game.isGameOver())
            game.render();
        else {
            game.renderGameOver();
        }
    }
    return 0;
}
