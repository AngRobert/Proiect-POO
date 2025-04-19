#include <iostream>
#include <cmath>
#include <random>
#include <utility>
#include <vector>
#include <chrono>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Point {
private:
    sf::CircleShape point_shape;

    void generatePosition() {
        //Makes sure the point cannot spawn under the rhythm circle
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

public:
    void draw(sf::RenderTarget& target) const {
        target.draw(point_shape);
    }
    explicit Point() {
        this->point_shape.setRadius(7);
        this->point_shape.setOrigin(this->point_shape.getGlobalBounds().width / 2,
                                this->point_shape.getGlobalBounds().height / 2);
        this->point_shape.setFillColor(sf::Color(243, 255, 13, 255));
        this->generatePosition();
    }
    [[nodiscard]] sf::Vector2f getPosition() const {
        return this->point_shape.getPosition();
    }
};

class RhythmCircle {
/*  CIRCLE WHICH DISPLAYS THE OPTIMAL TIMING OF YOUR STROKE IN THE WATER, WHILE ALSO KEEPING COUNT
 *  OF HOW MANY STROKES YOU'VE DONE SINCE YOUR LAST BREATH
 */
private:
    sf::Texture circle_texture;
    sf::Sprite circle_sprite;
    sf::CircleShape outer_circle{150.f};
    sf::Clock circle_timer;

    void resetCircle() {
        std::cout << "RESET CIRCLE" << std::endl;
        this->circle_timer.restart();
        this->outer_circle.setScale(0.8f, 0.8f);
    }
public:
    void draw(sf::RenderTarget& target) const {
        target.draw(outer_circle);
        target.draw(circle_sprite);
    }
    explicit RhythmCircle(const std::string& circle_texture_filename = "", const sf::Vector2f circle_pos = sf::Vector2f(0, 0)) {
        if (!this->circle_texture.loadFromFile(circle_texture_filename)) {
            std::cerr << "Failed to load circle texture" << std::endl;
        }
        this->circle_sprite.setTexture(circle_texture);
        this->circle_sprite.setOrigin(this->circle_sprite.getGlobalBounds().width / 2.f,
                                        this->circle_sprite.getGlobalBounds().height / 2.f);
        this->outer_circle.setOrigin(this->outer_circle.getGlobalBounds().width / 2.f,
                                        this->outer_circle.getGlobalBounds().height / 2.f);
        this->circle_sprite.setPosition(circle_pos);
        this->circle_sprite.setScale(1.f, 1.f);
        this->outer_circle.setOutlineColor(sf::Color(64, 255, 0, 255));
        this->outer_circle.setOutlineThickness(8.f);
        this->outer_circle.setFillColor(sf::Color(3, 177, 252, 0));
        this->outer_circle.setPosition(circle_pos);
        std::cout << this->circle_sprite.getGlobalBounds().width << std::endl;
        std::cout << this->circle_sprite.getGlobalBounds().height << std::endl;
    }
    void updateCircle() {
        if (this->circle_timer.getElapsedTime().asSeconds() > 1.2f) {
            this->resetCircle();
        }
        this->outer_circle.setScale(this->outer_circle.getScale().x * 0.988012f, this->outer_circle.getScale().y * 0.988012f);
    }
    // bool isTimingCorrect() const {
    //     return this->circle_timer.getElapsedTime().asSeconds() > 0.8f && this->circle_timer.getElapsedTime().asSeconds() < 1.2f;
    // }
};
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
    void checkOutOfBounds() {
        //Handles the out-of-bounds issue

        sf::Vector2f playerPos = this->player_sprite.getPosition();
        float clampedX = std::clamp(playerPos.x, 25.f, static_cast<float>(1920));
        float clampedY = std::clamp(playerPos.y, 25.f, static_cast<float>(1080));
        this->player_sprite.setPosition(clampedX, clampedY);
    }
    bool isPlayerMoving() const {
        return this->player_movement_speed > 50.f;
    }
public:
    explicit Player(float const player_movement_speed_ = 0.f) : player_movement_speed(player_movement_speed_), breath(100.f) {
        this->initPlayer();
    }
    [[nodiscard]] float getBreath() const {
        return this->breath;
    }
    [[nodiscard]] bool isCollidingWithPoint(const sf::Vector2f point_pos) const {
        return this->player_sprite.getGlobalBounds().contains(point_pos);
    }
    void draw(sf::RenderTarget& target) const {
        target.draw(this->player_sprite);
    }
    void doBreath() {
        this->next_stroke_is_breath = true;
    }
    void updatePlayer(sf::Vector2f location, float deltaTime) {
        // location = mouse position

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
        this->checkOutOfBounds();
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
                this->player_movement_speed = 150.f;    // THE SPEED RESETS TO NORMAL
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
    RhythmCircle rhythm_circle{"textures/rhythmcircle_texture.png", sf::Vector2f(960.f, 150.f)};
    std::vector<Point> points;
    sf::Font font;
    sf::Text breath_text;
    sf::Text breath_value;
    sf::Text game_over;
    sf::Text score_text;
    sf::Text score_value;
    sf::RenderWindow* window;
    sf::Event event{};
    sf::Clock clock;
    sf::VideoMode video_mode;
    sf::Vector2f mouse_position_window;
    Player player;
    int points_counter = 0;
    int max_points = 5;
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
    void initPoints() {
        for (int i = 0; i < max_points; i++) {
            points.emplace_back();
        }
    }
    void updateMousePosition() {
        const sf::Vector2i pixel_position = sf::Mouse::getPosition(*this->window);
        this->mouse_position_window = this->window->mapPixelToCoords(pixel_position);
    }
    void updateText() {
        this->breath_value.setString(std::to_string(static_cast<int>(this->player.getBreath())));
        this->score_value.setString(std::to_string(this->points_counter));
    }
    void updatePoints() {
        auto it = points.begin();
        while (it != points.end()) {
            if (player.isCollidingWithPoint(it->getPosition())) {
                it = points.erase(it);
                it = points.insert(it, Point());
                ++it;
                this->points_counter++;
            }
            else {
                ++it;
            }
        }

    }
    void renderCircle() const {
        this->rhythm_circle.draw(*this->window);
    }
    void renderText() const {
        this->window->draw(this->breath_text);
        this->window->draw(this->breath_value);
        this->window->draw(this->score_text);
        this->window->draw(this->score_value);
    }
    void renderPlayer() const {
        this->player.draw(*this->window);
    }
    void renderPoints() const {
        for (auto& point : this->points) {
            point.draw(*this->window);
        }
    }
public:
    explicit Game() : window(nullptr) {
        this->initWindow();
        this->initFont();
        this->initPoints();
        sf::Vector2f text_pos{1730.f, 400.f};
        this->initText(breath_text, text_pos, "BREATH:");
        text_pos.y = 440.f;
        this->initText(breath_value, text_pos, "");
        text_pos.x = 860.f;
        text_pos.y = 510.f;
        this->initText(game_over, text_pos, "GAME OVER !");
        text_pos.x = 30.f;
        text_pos.y = 30.f;
        this->initText(score_text, text_pos, "Points: ");
        text_pos.x += 130.f;
        this->initText(score_value, text_pos, "");
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
            this->updatePoints();
            this->rhythm_circle.updateCircle();
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
        this->renderCircle();
        this->renderPoints();
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
