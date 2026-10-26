#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SFML/Graphics.hpp>

const int FIELD_SIZE = 10;
const int CELL_SIZE = 60;

struct Point {
    int x, y;
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    HELP
};

class Game {
private:
    std::vector<Point> snake;
    Direction direction;
    Point food;
    int score;
    bool gameOver;
    GameState state;
    GameState prevState;
    
    sf::Clock moveClock;
    float moveInterval;

    void spawnFood();
    bool isSnakeAt(const Point& point) const;
    void moveSnake();
    void startGame();

public:
    Game();
    
    void update();
    void render(sf::RenderWindow& window);
    void handleInput(const sf::Event& event);
    void reset();
    
    bool isGameOver() const;
    int getScore() const;
    GameState getState() const;
};

#endif