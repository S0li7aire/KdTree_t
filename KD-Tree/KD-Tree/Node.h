#pragma once

#include "Rectangle.h"

struct Node {
    std::vector<Rectangle> rectangles;
    int depth;
    Node* left;
    Node* right;
    int median;
    int split_dimensions;

    Node(const std::vector<Rectangle>& rects, int d = 0);

    ~Node();

    void sort(int split_dimensions);

    float getMedian(const std::vector<Rectangle>& rects, int axis);

    int getIndex(const std::vector<Rectangle>& rectangles, int split_dimensions);
};
