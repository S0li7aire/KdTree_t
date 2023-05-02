#include "KdTree.h"

KdTree::KdTree(const std::vector<Rectangle>& rects)
{
    root_ = new Node(rects);
}

KdTree::~KdTree()
{
    delete root_;
}

bool const KdTree::contains(float x, float y) {
    Node* node = root_;
    while (node != nullptr) {
        if (node->rectangles.empty()) {
            return false;
        }
        if (node->rectangles.size() == 1) {
            return node->rectangles[0].contains(x, y);
        }
        if (node->split_dimensions == 0) {
            if (x <= node->left->rectangles.back().x2) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        else {
            if (y <= node->left->rectangles.back().y2) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
    }
    return false;
}
