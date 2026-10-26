#include "game.h"
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Snake Game");
    Game game;
    
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            game.handleInput(*event);
        }
        
        if (!game.isGameOver()) {
            game.update();
        }
        
        game.render(window);
        
        if (game.isGameOver()) {
            sf::Font font;
            if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
                sf::Text gameOverText(font);
                gameOverText.setString("Game Over!\nScore: " + std::to_string(game.getScore()) + "\nPress ESC to exit");
                gameOverText.setCharacterSize(40);
                gameOverText.setFillColor(sf::Color::White);
                
                sf::FloatRect textRect = gameOverText.getLocalBounds();
                gameOverText.setOrigin(sf::Vector2f(
                    textRect.size.x / 2, 
                    textRect.size.y / 2
                ));
                gameOverText.setPosition(sf::Vector2f(300, 300));
                
                window.draw(gameOverText);
            }
        }
        
        window.display();
    }
    
    return 0;
}