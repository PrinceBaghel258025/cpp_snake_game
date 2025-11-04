#include "core.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Game boundaries (will be set by terminal size in Game class)
// Using reasonable defaults for now
static int game_width = 148;
static int game_height = 40;

// Snake implementation
Snake::Snake() {
    // Initialize snake with a body of 3 segments starting at center
    // start top left corner
    body.push_back({0, 0});
    body.push_back({1, 0});
    body.push_back({2, 0});
    direction = RIGHT;
}

void Snake::grow() {
    // Append to body at tail - add two segments to compensate for
    // the tail that was removed by move() (which was called before this)
    // Net effect: snake grows by 1 segment
    if (!body.empty()) {
        body.push_back(body.back());
        body.push_back(body.back());
    }
}

void Snake::move() {
    if (body.empty()) return;
    
    Point new_head = body.front();
    
    // Calculate new head position based on direction
    switch (direction) {
        case LEFT:
            new_head.x--;
            break;
        case RIGHT:
            new_head.x++;
            break;
        case UP:
            new_head.y--;
            break;
        case DOWN:
            new_head.y++;
            break;
    }
    
    // Add new head at front
    body.push_front(new_head);
    
    // Remove tail to maintain snake length
    // If grow() was called before this, it added a segment,
    // so removing the tail here will result in net growth
    if (body.size() > 1) {
        body.pop_back();
    }
}

void Snake::set_direction(Direction d) {
    // Prevent reversing directly into opposite direction
    if ((direction == LEFT && d == RIGHT) ||
        (direction == RIGHT && d == LEFT) ||
        (direction == UP && d == DOWN) ||
        (direction == DOWN && d == UP)) {
        return;
    }
    direction = d;
}

const std::deque<Point>& Snake::get_body() const {
    return body;
}

const Point& Snake::head() const {
    return body.front();
}

bool Snake::collided_with_self() {
    if (body.empty()) return false;
    
    Point head_pos = body.front();
    
    // Check if head collides with any body segment (starting from index 1)
    for (size_t i = 1; i < body.size(); i++) {
        if (head_pos == body[i]) {
            return true;
        }
    }
    
    return false;
}

// Food implementation
Food::Food() {
    // Initialize with a default position
    pos = {game_width / 2, game_height / 2};
}

void Food::generate_food(Snake snake) {
    // Initialize random seed if not already done
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }
    
    Point new_pos;
    bool valid_pos = false;
    const auto& snake_body = snake.get_body();
    
    // Try to find a valid position that doesn't overlap with snake body
    int attempts = 0;
    const int max_attempts = 1000;
    
    while (!valid_pos && attempts < max_attempts) {
        // Generate random position within boundaries (leave 1 cell border)
        new_pos.x = 1 + (std::rand() % (game_width - 2));
        new_pos.y = 1 + (std::rand() % (game_height - 2));
        
        // Check if position overlaps with snake body
        valid_pos = true;
        for (const auto& segment : snake_body) {
            if (new_pos == segment) {
                valid_pos = false;
                break;
            }
        }
        
        attempts++;
    }
    
    // If we couldn't find a valid position, return a default one
    // (shouldn't happen in normal gameplay)
    if (!valid_pos) {
        new_pos = {game_width / 2, game_height / 2};
    }
    
    pos = new_pos;
}

// Wall collision detection
bool hit_wall(Point snake_head) {
    // Check if head is outside game boundaries
    // Assuming boundaries are 0 to game_width-1 and 0 to game_height-1
    // With 1-cell border: 1 to game_width-2 and 1 to game_height-2
    return snake_head.x <= 0 || snake_head.x >= game_width - 1 ||
           snake_head.y <= 0 || snake_head.y >= game_height - 1;
}

