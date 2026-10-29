#ifndef GAME_H
#define GAME_H

// Подключение стандартной библиотеки контейнеров и графического фреймворка SFML
#include <vector>
#include <SFML/Graphics.hpp>

// Константы игрового поля: размер сетки и размер одной клетки в пикселях
const int FIELD_SIZE = 10;
const int CELL_SIZE = 60;

// Структура для хранения целочисленных координат на сетке
struct Point {
    int x, y;
};

// Возможные направления движения змейки
enum Direction {
    UP, DOWN, LEFT, RIGHT
};

// Конечный автомат состояний игры для управления логикой отрисовки и ввода
enum GameState {
    MENU, PLAYING, GAME_OVER, HELP
};

// Основной класс, инкапсулирующий логику, состояние и отрисовку игры
class Game {
private:
    // Состояние игры и игровые объекты
    std::vector<Point> snake; // Вектор координат сегментов (индекс 0 - голова)
    Direction direction;      // Текущий вектор движения
    Point food;               // Позиция еды на поле
    int score;                // Текущий счет
    int teleports;            // Счетчик прохождений сквозь границы (для варианта)
    bool gameOver;            // Флаг завершения игры
    GameState state;          // Текущее состояние (меню, игра и т.д.)
    GameState prevState;      // Состояние до открытия справки (для корректного возврата)

    // Управление временем
    sf::Clock moveClock;      // Таймер для контроля скорости движения
    float moveInterval;       // Интервал между шагами в секундах

    // Внутренняя логика (скрыта от внешнего интерфейса)
    void spawnFood();                           // Размещение еды на свободной клетке
    bool isSnakeAt(const Point& point) const;   // Проверка коллизии головы с телом
    void moveSnake();                           // Расчет новой позиции и обработка границ
    void startGame();                           // Инициализация параметров и начало раунда

public:
    Game(); // Конструктор по умолчанию

    // Публичный интерфейс для взаимодействия с главным циклом (main.cpp)
    void update();                              // Обновление логики по таймеру
    void render(sf::RenderWindow& window);      // Отрисовка текущего кадра
    void handleInput(const sf::Event& event);   // Обработка нажатий клавиш
    void reset();                               // Полный сброс и возврат в главное меню

    // Геттеры для чтения состояния извне
    bool isGameOver() const;
    int getScore() const;
    GameState getState() const;
};

#endif