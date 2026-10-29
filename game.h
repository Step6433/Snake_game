#ifndef GAME_H
#define GAME_H

// Подключение стандартной библиотеки для динамических массивов и графической библиотеки SFML
#include <vector>
#include <SFML/Graphics.hpp>

// Константы игрового поля: размер поля в клетках и размер одной клетки в пикселях
const int FIELD_SIZE = 10;
const int CELL_SIZE = 60;

// Структура для хранения координат точки на игровом поле
struct Point {
    int x, y;
};

// Перечисление возможных направлений движения змейки
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Перечисление состояний игры: меню, игровой процесс, конец игры, справка
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    HELP
};

// Основной класс игры, инкапсулирующий всю игровую логику
class Game {
private:
    // Вектор координат сегментов змейки (первый элемент — голова)
    std::vector<Point> snake;
    Direction direction;        // Текущее направление движения
    Point food;                 // Координаты еды на поле
    int score;                  // Текущий счёт игрока
    bool gameOver;              // Флаг окончания игры
    GameState state;            // Текущее состояние игры
    GameState prevState;        // Предыдущее состояние (для возврата из справки)
    
    sf::Clock moveClock;        // Таймер для контроля скорости движения
    float moveInterval;         // Интервал между движениями в секундах

    // Приватные методы для внутренней логики игры
    void spawnFood();                           // Генерация еды на случайной свободной клетке
    bool isSnakeAt(const Point& point) const;   // Проверка, занимает ли змейка указанную точку
    void moveSnake();                           // Вычисление новой позиции головы и проверка столкновений
    void startGame();                           // Инициализация параметров для начала игры

public:
    Game();  // Конструктор класса
    
    // Публичные методы для взаимодействия с игрой извне
    void update();                              // Обновление состояния игры (вызов движения по таймеру)
    void render(sf::RenderWindow& window);      // Отрисовка текущего состояния игры в окно
    void handleInput(const sf::Event& event);   // Обработка пользовательского ввода
    void reset();                               // Сброс игры в начальное состояние (меню)
    
    bool isGameOver() const;    // Проверка, завершена ли игра
    int getScore() const;       // Получение текущего счёта
    GameState getState() const; // Получение текущего состояния игры
};

#endif