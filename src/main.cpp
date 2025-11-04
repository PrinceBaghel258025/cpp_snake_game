#include "core.h"
#include <ncurses.h>
#include <iostream>

// Helper function to map ncurses key codes to Direction enum
// Returns the new direction if input was received, otherwise returns current direction
Direction get_input(Direction current) {
    int ch = getch();
    if (ch == ERR) {
        return current; // No input available
    }
    
    switch (ch) {
        case KEY_UP:
            return UP;
        case KEY_DOWN:
            return DOWN;
        case KEY_LEFT:
            return LEFT;
        case KEY_RIGHT:
            return RIGHT;
        case 'q':
        case 'Q':
            return current; // Don't change direction, but could exit
        default:
            return current; // No change
    }
}

// Render function to draw the game state
void render(const Snake& snake, const Food& food) {
    clear();
    
    // Draw border
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // std::cout << "Terminal size: " << max_y << "x" << max_x << std::endl;
    // napms(10000);
    
    // Draw top and bottom borders
    for (int x = 0; x < max_x; x++) {
        mvaddch(0, x, '-');
        mvaddch(max_y - 1, x, '-');
    }
    
    // Draw left and right borders
    for (int y = 0; y < max_y; y++) {
        mvaddch(y, 0, '|');
        mvaddch(y, max_x - 1, '|');
    }
    
    // // Draw corners
    mvaddch(0, 0, '+');
    mvaddch(0, max_x - 1, '+');
    mvaddch(max_y - 1, 0, '+');
    mvaddch(max_y - 1, max_x - 1, '+');
    // std::cout << "corner positions: " << 0 << "," << 0 << " " << 0 << "," << max_x - 1 << " " << max_y - 1 << "," << 0 << " " << max_y - 1 << "," << max_x - 1 << std::endl;


    // Draw snake body
    const auto& body = snake.get_body();
    for (size_t i = 0; i < body.size(); i++) {
        int y = body[i].y;
        int x = body[i].x;
        // std::cout << "Body segment: " << y << "," << x << std::endl;
        
        // Make sure coordinates are within bounds
        if (y >= 1 && y < max_y - 1 && x >= 1 && x < max_x - 1) {
            if (i == 0) {
                // std::cout << "Head: " << y << "," << x << std::endl;
                mvaddch(y, x, '@'); // Head
            } else {
                // std::cout << "Body: " << y << "," << x << std::endl;
                mvaddch(y, x, 'o'); // Body
            }
        }
    }
    
    // Draw food
    if (food.pos.y >= 1 && food.pos.y < max_y - 1 && 
        food.pos.x >= 1 && food.pos.x < max_x - 1) {
        mvaddch(food.pos.y, food.pos.x, '*');
        // std::cout << "Food: " << food.pos.y << "," << food.pos.x << std::endl;
    }
    
    refresh();
}

int main() {
    // Initialize ncurses
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE); // Enable function keys (arrow keys)
    nodelay(stdscr, TRUE); // Non-blocking input
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Initialize game
    Snake snake = Snake(Point{max_x, max_y});
    bool game_over = false;
    Food food = Food(Point{max_x, max_y});
    int frame_delay_ms = 80;
    
   
    // std::cout << "Terminal size: " << max_y << "x" << max_x << std::endl;
    Direction current_direction = RIGHT;
    
    // Initial render
    // render(snake, food);
    
    // Game loop
    while (!game_over) {
        // Handle non-blocking input
        Direction new_direction = get_input(current_direction);
        
        // Update direction if it changed
        if (new_direction != current_direction) {
            snake.set_direction(new_direction);
            current_direction = new_direction;
        }
        
        // Move snake
        snake.move();
        
        // Check if snake ate food
        if (snake.head() == food.pos) {
            snake.grow();
            food.generate_food(snake);
        }
        
        // Check for collisions
        if (snake.collided_with_self() || hit_wall(snake.head(), max_y, max_x)) {
        // if ( hit_wall(snake.head(), max_y, max_x)) {
            game_over = true;
        }
        
        // Render updated frame
        render(snake, food);
        
        // Frame delay
        napms(frame_delay_ms);
    }
    
    // Cleanup
    endwin();
    return 0;
}