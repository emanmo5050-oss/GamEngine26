//Vec2.h
#pragma once

struct Vec2 {
    int x, y;
    Vec2(int x = 0, int y = 0) : x(x), y(y) {}

    Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
    Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
    Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
    Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
    bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
};