#include "Node.h"
#include <algorithm>

Node::Node(const std::vector<Rectangle>& rects, int d)
{
    rectangles = rects;
    depth = d;
    left = nullptr;
    right = nullptr;
    if (rectangles.size() > 1)
        if (!rectangles.empty())
        {
            split_dimensions = depth % 2;

            sort(split_dimensions);

            int index = getIndex(rectangles, split_dimensions);
            median = getMedian(rectangles, split_dimensions);
            int j = 0;
            for (int i = 0; i < rectangles.size() - j; i++) {
                if (rectangles[i].contains(median, split_dimensions)) {
                    if (split_dimensions == 0) {
                        auto rectBuff_1 = rectangles[i];
                        auto rectBuff_2 = rectangles[i];
                        rectBuff_1.x2 = median;
                        rectBuff_2.x1 = median;
                        if (rectBuff_1.x1 != rectBuff_1.x2) {
                            if (rectBuff_2.x1 != rectBuff_2.x2) {
                                rectangles[i] = rectBuff_1;
                                rectangles.push_back(rectBuff_2);
                                j++;
                                index = i;
                            }
                        }
                    }
                    else {
                        auto rectBuff_1 = rectangles[i];
                        auto rectBuff_2 = rectangles[i];
                        rectBuff_1.y2 = median;
                        rectBuff_2.y1 = median;
                        if (rectBuff_1.y1 != rectBuff_1.y2) {
                            if (rectBuff_2.y1 != rectBuff_2.y2) {
                                rectangles[i] = rectBuff_1;
                                rectangles.push_back(rectBuff_2);
                                j++;
                                index = i;
                            }
                        }
                    }

                }
            }
            sort(split_dimensions);
            std::vector<Rectangle> left_rectangles(rectangles.begin(), rectangles.begin() + (rectangles.size() == 2 ? 1 : (index + 1)));
            std::vector<Rectangle> right_rectangles(rectangles.begin() + (rectangles.size() == 2 ? 1 : (index + 1)), rectangles.end());
            left = new Node(left_rectangles, depth + 1);
            right = new Node(right_rectangles, depth + 1);
        }
}

Node::~Node()
{
    delete left;
    delete right;
}

void Node::sort(int split_dimensions) {
    if (split_dimensions == 0)
    {
        std::sort(rectangles.begin(), rectangles.end(),
            [this](const Rectangle& r1, const Rectangle& r2) {
                return r1.x2 < r2.x2;
            }
        );
    }
    else
    {
        std::sort(rectangles.begin(), rectangles.end(),
            [this](const Rectangle& r1, const Rectangle& r2) {
                return r1.y2 < r2.y2;
            });
    }
}

float Node::getMedian(const std::vector<Rectangle>& rects, int axis) {
    float sum = 0, iter = 0;
    for (auto x : rects)
    {
        sum += x.get(axis);
        iter += 2;
    }
    return ceil(sum / iter);
}

int Node::getIndex(const std::vector<Rectangle>& rectangles, int split_dimensions)
{
    if (rectangles.size() == 2 || rectangles.size() == 3) {
        return 1;
    }
    int axis = split_dimensions;
    auto rects = rectangles;
    float median = getMedian(rects, axis);
    auto large = std::max_element(rects.begin(), rects.end(),
        [this, axis](Rectangle& r1, Rectangle& r2) -> bool {
            return r1.get(axis) < r2.get(axis);
        });

    float largest = *large[0].map[axis] > *large[0].map[axis + 2] ? *large[0].map[axis] : *large[0].map[axis + 2];
    int index;
    for (int i = 0; i < rects.size(); i++)
        if (*rects[i].map[axis] >= median && *rects[i].map[axis] <= largest)
        {
            largest = *rects[i].map[axis];
            index = i;
        }
        else if (*rects[i].map[axis + 2] >= median && *rects[i].map[axis + 2] <= largest)
        {
            largest = *rects[i].map[axis + 2];
            index = i;
        }
    return index == 0 ? index + 1 : index;
}
