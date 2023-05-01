#ifndef KDTREE_H
#define KDTREE_H

#include "incl.h"

class KdTree {
public:
    KdTree(const std::vector<Rectangle>& rects);
    ~KdTree();
    bool const contains(float x, float y);
private:
    Node* root_;
};

#endif
