#include "game.h"
#include <cstdlib>
#include <ctime>

// Конструктор: инициализация начального состояния игры
Game::Game() : direction(RIGHT), score(0), teleports(0), gameOver(false), state(MENU), prevState(MENU), moveInterval(0.2f) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

// Генерация еды на случайной свободной клетке
void Game::spawnFood() {
    do {
        food.x = std::rand() % FIELD_SIZE;
        food.y = std::rand() % FIELD_SIZE;
    } while (isSnakeAt(food));
}

// Проверка, занимает ли змейка указанную точку
bool Game::isSnakeAt(const Point& point) const {
    for (const auto& segment : snake) {
        if (segment.x == point.x && segment.y == point.y) {
            return true;
        }
    }
    return false;
}

// Отрисовка игрового поля, змейки, еды и интерфейса
void Game::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    if (state == MENU) {
        sf::Font font;
        if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            sf::Text title(font);
            title.setString("SNAKE GAME");
            title.setCharacterSize(48);
            title.setFillColor(sf::Color::Green);
            sf::FloatRect titleRect = title.getLocalBounds();
            title.setOrigin(sf::Vector2f(titleRect.size.x / 2, titleRect.size.y / 2));
            title.setPosition(sf::Vector2f(300, 180));
            window.draw(title);

            sf::Text subtitle(font);
            subtitle.setString("NO WALLS MODE");
            subtitle.setCharacterSize(24);
            subtitle.setFillColor(sf::Color(100, 200, 100));
            sf::FloatRect subRect = subtitle.getLocalBounds();
            subtitle.setOrigin(sf::Vector2f(subRect.size.x / 2, subRect.size.y / 2));
            subtitle.setPosition(sf::Vector2f(300, 240));
            window.draw(subtitle);

            sf::Text startText(font);
            startText.setString("Press ENTER to start\nPress H for help");
            startText.setCharacterSize(24);
            startText.setFillColor(sf::Color::White);
            sf::FloatRect startRect = startText.getLocalBounds();
            startText.setOrigin(sf::Vector2f(startRect.size.x / 2, startRect.size.y / 2));
            startText.setPosition(sf::Vector2f(300, 350));
            window.draw(startText);
        }
        return;
    }

    if (state == HELP) {
        sf::Font font;
        if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            sf::Text title(font);
            title.setString("Help");
            title.setCharacterSize(48);
            title.setFillColor(sf::Color::Green);
            sf::FloatRect titleRect = title.getLocalBounds();
            title.setOrigin(sf::Vector2f(titleRect.size.x / 2, titleRect.size.y / 2));
            title.setPosition(sf::Vector2f(300, 100));
            window.draw(title);

            sf::Text rules(font);
            rules.setString("Goal: collect food and teleport\nRules: walls don't kill you\nTeleport counter increases\nControls: arrow keys\n\nPress H to return");
            rules.setCharacterSize(24);
            rules.setFillColor(sf::Color::White);
            sf::FloatRect rulesRect = rules.getLocalBounds();
            rules.setOrigin(sf::Vector2f(rulesRect.size.x / 2, rulesRect.size.y / 2));
            rules.setPosition(sf::Vector2f(300, 300));
            window.draw(rules);
        }
        return;
    }

    // Отрисовка игрового поля
    for (int y = 0; y < FIELD_SIZE; ++y) {
        for (int x = 0; x < FIELD_SIZE; ++x) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
            cell.setPosition(sf::Vector2f(x * CELL_SIZE + 1, y * CELL_SIZE + 1));
            cell.setFillColor(sf::Color(30, 30, 30));
            window.draw(cell);
        }
    }

    // Отрисовка еды
    sf::RectangleShape foodShape(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
    foodShape.setPosition(sf::Vector2f(food.x * CELL_SIZE + 2, food.y * CELL_SIZE + 2));
    foodShape.setFillColor(sf::Color::Red);
    window.draw(foodShape);

    // Отрисовка змейки
    for (size_t i = 0; i < snake.size(); ++i) {
        sf::RectangleShape segment(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
        segment.setPosition(sf::Vector2f(snake[i].x * CELL_SIZE + 2, snake[i].y * CELL_SIZE + 2));

        if (i == 0) {
            segment.setFillColor(sf::Color::Green);
        } else {
            segment.setFillColor(sf::Color(100, 200, 100));
        }

        window.draw(segment);
    }

    // Отрисовка счёта и телепортаций
    sf::Font font;
    if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        sf::Text scoreText(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(10, 520));
        window.draw(scoreText);

        sf::Text teleportText(font);
        teleportText.setString("Teleports: " + std::to_string(teleports));
        teleportText.setCharacterSize(24);
        teleportText.setFillColor(sf::Color::White);
        teleportText.setPosition(sf::Vector2f(10, 550));
        window.draw(teleportText);
    }
}

// Обновление состояния игры
void Game::update() {
    if (state != PLAYING) {
        return;
    }

    if (moveClock.getElapsedTime().asSeconds() >= moveInterval) {
        moveSnake();
        moveClock.restart();
    }
}

// Движение змейки и проверка столкновений
void Game::moveSnake() {
    Point newHead = snake[0];

    switch (direction) {
        case UP:
            newHead.y--;
            break;
        case DOWN:
            newHead.y++;
            break;
        case LEFT:
            newHead.x--;
            break;
        case RIGHT:
            newHead.x++;
            break;
    }

    // Телепортация через границы поля
    if (newHead.x < 0 || newHead.x >= FIELD_SIZE ||
        newHead.y < 0 || newHead.y >= FIELD_SIZE) {

        if (newHead.x < 0) newHead.x = FIELD_SIZE - 1;
        else if (newHead.x >= FIELD_SIZE) newHead.x = 0;
        else if (newHead.y < 0) newHead.y = FIELD_SIZE - 1;
        else if (newHead.y >= FIELD_SIZE) newHead.y = 0;
        teleports++;
    }

    // Столкновение с хвостом
    if (isSnakeAt(newHead)) {
        gameOver = true;
        state = GAME_OVER;
        return;
    }

    bool ateFood = (newHead.x == food.x && newHead.y == food.y);
    snake.insert(snake.begin(), newHead);

    if (ateFood) {
        score++;
        spawnFood();
    } else {
        snake.pop_back();
    }
}

// Обработка ввода
void Game::handleInput(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        const auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent) {
            if (state == MENU) {
                if (keyEvent->code == sf::Keyboard::Key::Enter) {
                    startGame();
                } else if (keyEvent->code == sf::Keyboard::Key::H) {
                    prevState = MENU;
                    state = HELP;
                }
            }
            else if (state == PLAYING) {
                if (keyEvent->code == sf::Keyboard::Key::H) {
                    prevState = PLAYING;
                    state = HELP;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Up && direction != DOWN) {
                    direction = UP;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Down && direction != UP) {
                    direction = DOWN;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Left && direction != RIGHT) {
                    direction = LEFT;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Right && direction != LEFT) {
                    direction = RIGHT;
                }
            }
            else if (state == HELP) {
                if (keyEvent->code == sf::Keyboard::Key::H) {
                    state = prevState;
                }
            }
        }
    }
}

// Запуск игры
void Game::startGame() {
    snake.clear();
    snake.push_back({5, 5});
    direction = RIGHT;
    score = 0;
    teleports = 0;
    gameOver = false;
    state = PLAYING;
    moveClock.restart();
    spawnFood();
}

// Сброс в меню
void Game::reset() {
    snake.clear();
    snake.push_back({5, 5});
    direction = RIGHT;
    score = 0;
    teleports = 0;
    gameOver = false;
    state = MENU;
    prevState = MENU;
    moveClock.restart();
    spawnFood();
}

bool Game::isGameOver() const {
    return gameOver;
}

int Game::getScore() const {
    return score;
}

GameState Game::getState() const {
    return state;
}