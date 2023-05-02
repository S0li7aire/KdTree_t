#pragma once

#include <map>
#include <vector>

struct Rectangle {
    float x1, y1, x2, y2;
    std::map<int, float*> map = { {0, &x1}, {1, &y1}, {2, &x2}, {3,&y2} };

    bool contains(float x, float y);

    bool contains(float x, int split_dimensions);

    float get(bool axis);

    std::vector<float> draw();
};
