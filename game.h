#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SFML/Graphics.hpp>

const int FIELD_SIZE = 10;
const int CELL_SIZE = 50;

struct Point {
    int x, y;
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Game {
private:
    std::vector<Point> snake;
    Direction direction;
    Point food;
    int score;
    bool gameOver;
    
    sf::Clock moveClock; // Часы для контроля скорости движения
    const float moveInterval = 0.2f; // Интервал движения в секундах (5 раз в секунду)

    void spawnFood();
    bool isSnakeAt(const Point& p) const;
    void moveSnake(); // Новый метод для движения

public:
    Game();
    
    void update();
    void render(sf::RenderWindow& window);
    void handleInput(const sf::Event& event);
    
    bool isGameOver() const;
    int getScore() const;
};

#endif // GAME_H