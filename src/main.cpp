#include <SFML/Graphics.hpp>
#include <deque>
#include <cstdlib> // for rand()

const int blockSize = 20;
const int width = 800;
const int height = 600;

class Snake {
public:
    Snake();
    void handleInput();
    void update();
    void render(sf::RenderWindow& window);
    bool isGameOver() const;

private:
    std::deque<sf::RectangleShape> snake;
    sf::Vector2f direction;
    sf::Clock clock;
    float speed;
    bool gameOver;

    sf::RectangleShape food; // New variable for food
    void spawnFood(); // New function to spawn food
};

Snake::Snake() {
    snake.push_front(sf::RectangleShape(sf::Vector2f(blockSize, blockSize)));
    snake.front().setPosition(100, 100);
    direction = sf::Vector2f(blockSize, 0);
    speed = 0.1f;
    gameOver = false;

    food.setSize(sf::Vector2f(blockSize, blockSize)); // Initialize food size
    spawnFood(); // Spawn initial food
}

void Snake::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction.y == 0) {
        direction = sf::Vector2f(0, -blockSize);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction.y == 0) {
        direction = sf::Vector2f(0, blockSize);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction.x == 0) {
        direction = sf::Vector2f(-blockSize, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction.x == 0) {
        direction = sf::Vector2f(blockSize, 0);
    }
}

void Snake::update() {
    if (!gameOver) {
        if (clock.getElapsedTime().asSeconds() > speed) {
            clock.restart();
            sf::RectangleShape newSegment(sf::Vector2f(blockSize, blockSize));
            newSegment.setPosition(snake.front().getPosition() + direction);
            snake.push_front(newSegment);

            // Check for collisions with food
            if (snake.front().getGlobalBounds().intersects(food.getGlobalBounds())) {
                spawnFood(); // Spawn new food
            } else {
                // Remove the tail of the snake
                if (snake.size() > 1) {
                    snake.pop_back();
                }
            }

            // Check for collisions with window boundaries
            if (snake.front().getPosition().x < 0 ||
                snake.front().getPosition().x >= width ||
                snake.front().getPosition().y < 0 ||
                snake.front().getPosition().y >= height) {
                gameOver = true;
            }
        }
    }
}

void Snake::render(sf::RenderWindow& window) {
    window.draw(food); // Draw food
    for (const auto& segment : snake) {
        window.draw(segment);
    }
    if (gameOver) {
        // Display "Game Over" message
        sf::Text gameOverText;
        gameOverText.setString("Game Over. Press R to restart.");
        gameOverText.setCharacterSize(24);
        gameOverText.setFillColor(sf::Color::White);
        gameOverText.setPosition(width / 2 - 160, height / 2 - 12);

        window.draw(gameOverText);
    }
}

bool Snake::isGameOver() const {
    return gameOver;
}

void Snake::spawnFood() {
    // Randomly position the food within the window
    int x = rand() % (width / blockSize) * blockSize;
    int y = rand() % (height / blockSize) * blockSize;

    food.setPosition(x, y);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Snake Game");
    window.setFramerateLimit(10);

    Snake snake;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R && snake.isGameOver()) {
                    snake = Snake(); // Restart the game
                }
                if (event.key.code == sf::Keyboard::Escape || snake.isGameOver() ) {
                    window.close();
                }
            }
        }

        snake.handleInput();
        snake.update();

        window.clear();
        snake.render(window);
        window.display();
    }

    return 0;
}
