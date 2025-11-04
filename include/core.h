#pragma once
#include<deque>

enum Direction {
    LEFT,    // Default value is 0
    RIGHT,    // Default value is 1
    UP,   // Default value is 2
    DOWN, // Default value is 3
};

struct Point {
    int x, y;
    // point equality
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y; 
    }
};
class Snake {
    std::deque<Point> body;
    Direction direction;
    public:
        Snake(Point screen_size);
        void grow();
        // appends to body at tail
        void move();
        void set_direction(Direction d);
        const std::deque<Point>& get_body() const;
        const Point& head() const;
        bool collided_with_self();
};

class Food {
    Point screen_size;
    public:
        Food(Point screen_size);
        // generate a random point
        Point pos; // saves food position

        // may require snake body as input
        void generate_food(Snake snake); // generate a food position within boundry and don't overlap snake body
};


bool hit_wall(Point snake_head, int max_y, int max_x);