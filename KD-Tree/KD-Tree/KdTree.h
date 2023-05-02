#pragma once

#include "Node.h"

class KdTree {
public:
    KdTree(const std::vector<Rectangle>& rects);
    ~KdTree();
    bool const contains(float x, float y);
private:
    Node* root_;
};
