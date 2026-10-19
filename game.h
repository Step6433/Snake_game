#ifndef GAME_H
#define GAME_H

#include <vector>

const int FIELD_SIZE = 10;

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

    // Новые методы для генерации еды
    void spawnFood();
    bool isSnakeAt(const Point& p) const;

public:
    Game();
    
    void update();
    void render() const;
    void handleInput(char key);
    
    bool isGameOver() const;
    int getScore() const;
};

#endif // GAME_H