#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>

const int gridSize = 20;
const int width = 800;
const int height = 600;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Snake {
    std::deque<sf::Vector2i> body;
    Direction direction;
};

struct Food {
    sf::Vector2i position;
};

bool isFoodEaten(const Snake& snake, const Food& food) {
    return snake.body.front() == food.position;
}

void handleInput(Snake& snake, bool& gameStarted, bool& gameOver) {
    if (!gameStarted && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        gameStarted = true;
        return;
    }

    if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        gameOver = false;
        snake.body.clear();
        snake.body.push_front(sf::Vector2i(5, 5));
        snake.direction = RIGHT;
        return;
    }

    if (gameStarted) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && snake.direction != DOWN)
            snake.direction = UP;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && snake.direction != UP)
            snake.direction = DOWN;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && snake.direction != RIGHT)
            snake.direction = LEFT;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && snake.direction != LEFT)
            snake.direction = RIGHT;
    }
}

void moveSnake(Snake& snake, bool ateFood) {
    sf::Vector2i head = snake.body.front();

    switch (snake.direction) {
        case UP:
            snake.body.push_front(sf::Vector2i(head.x, head.y - 1));
            break;
        case DOWN:
            snake.body.push_front(sf::Vector2i(head.x, head.y + 1));
            break;
        case LEFT:
            snake.body.push_front(sf::Vector2i(head.x - 1, head.y));
            break;
        case RIGHT:
            snake.body.push_front(sf::Vector2i(head.x + 1, head.y));
            break;
    }

    if (!ateFood) {
        snake.body.pop_back();
    }
}

void checkCollision(Snake& snake, Food& food, bool& gameOver) {
    sf::Vector2i head = snake.body.front();

    // Check collision with walls
    if (head.x < 0 || head.x >= width / gridSize || head.y < 0 || head.y >= height / gridSize) {
        gameOver = true;
    }

    // Check collision with itself
    for (auto it = snake.body.begin() + 1; it != snake.body.end(); ++it) {
        if (*it == head) {
            gameOver = true;
        }
    }

    // Check collision with food
    if (isFoodEaten(snake, food)) {
        food.position = sf::Vector2i(rand() % (width / gridSize), rand() % (height / gridSize));
        snake.body.push_back(snake.body.back());  // Increase the size of the snake
    }
}

void update(sf::RenderWindow& window, Snake& snake, Food& food, bool& gameStarted, bool& gameOver) {
    handleInput(snake, gameStarted, gameOver);

    if (gameStarted && !gameOver) {
        moveSnake(snake, isFoodEaten(snake, food));
        checkCollision(snake, food, gameOver);
    }

    window.clear();

    // Draw food
    sf::RectangleShape foodShape(sf::Vector2f(gridSize, gridSize));
    foodShape.setPosition(food.position.x * gridSize, food.position.y * gridSize);
    foodShape.setFillColor(sf::Color::Red);
    window.draw(foodShape);

    // Draw snake
    for (const auto& segment : snake.body) {
        sf::RectangleShape snakeSegment(sf::Vector2f(gridSize, gridSize));
        snakeSegment.setPosition(segment.x * gridSize, segment.y * gridSize);
        snakeSegment.setFillColor(sf::Color::Green);
        window.draw(snakeSegment);
    }

    if (!gameStarted) {
        // Display "Press Enter to Play" text
        sf::Font font;
        if (font.loadFromFile("../src/Montserrat-Bold.ttf")) {
            sf::Text text("Press Enter to Play", font, 30);
            text.setPosition(width / 2, height / 2);
            window.draw(text);
        }
    }

    if (gameOver) {
        // Display "Game Over. Restart." text
        sf::Font font;
        if (font.loadFromFile("../src/DancingScript-Bold.ttf")) {
            sf::Text text("Game Over. Restart.", font, 30);
            text.setPosition(width / 2, height / 2);
            window.draw(text);
        }
    }

    window.display();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Snake Game");

    Snake snake;
    snake.body.push_front(sf::Vector2i(5, 5));
    snake.direction = RIGHT;

    Food food;
    food.position = sf::Vector2i(rand() % (width / gridSize), rand() % (height / gridSize));

    bool gameStarted = false;
    bool gameOver = false;

    sf::Clock clock;
    float deltaTime = 0.0f;
    float delay = 0.1f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        deltaTime += clock.restart().asSeconds();

        if (deltaTime > delay) {
            update(window, snake, food, gameStarted, gameOver);
            deltaTime = 0.0f;
        }
    }

    return 0;
}
