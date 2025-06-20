#include <random>
template<typename T>
void Point<T>::generateProperties() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribWidth(50, 1870);
    std::uniform_int_distribution<int> distribHeight(50, 1030);
    std::uniform_int_distribution<int> distribColor(0, 1);
    const int c = distribColor(gen);
    float X, Y;
    do {
        X = static_cast<float>(distribWidth(gen));
        Y = static_cast<float>(distribHeight(gen));
    } while (X >= 810.f && X <= 1110.f && Y >= 0.f && Y <= 310.f);
    if (c == 0) {
        this->colour = "red";
        this->value = 2.5f;
        this->point_shape.setFillColor(sf::Color::Red);
    }
    else {
        this->colour = "yellow";
        this->value = 1;
        this->point_shape.setFillColor(sf::Color(243, 255, 13, 255));
    }
    point_shape.setPosition(X, Y);
}

template<typename T>
Point<T>::Point() {
    this->point_shape.setRadius(7);
    this->point_shape.setOrigin(this->point_shape.getGlobalBounds().width / 2,
                                this->point_shape.getGlobalBounds().height / 2);
    this->generateProperties();
}

template<typename T>
void Point<T>::draw(sf::RenderTarget& target) const {
    target.draw(point_shape);
}

template<typename T>
sf::Vector2f Point<T>::getPosition() const {
    return this->point_shape.getPosition();
}

template<typename T>
T Point<T>::getValue() const {
    return this->value;
}

