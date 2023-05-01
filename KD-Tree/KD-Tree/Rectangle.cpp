#include "incl.h"

bool Rectangle::contains(float x, float y)
{
    return x1 <= x && x <= x2 && y1 <= y && y <= y2;
}

bool Rectangle::contains(float x, int split_dimensions)
{
    return *this->map[split_dimensions] <= x && x <= *this->map[split_dimensions + 2];
}

float Rectangle::get(bool axis)
{
    return !axis ? x1 + x2 : y1 + y2;//axis == false --> Ox true --> Oy
}

std::vector<float> Rectangle::draw()
{
    std::vector<float> list = { x1, y1, x2, y2 };
    return list;
}