#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <SFML\Graphics.hpp>

struct Rectangle {
    float x1, y1, x2, y2;
    std::map<int, float*> map = { {0, &x1}, {1, &y1}, {2, &x2}, {3,&y2} };

    bool contains(float x, float y)
    {
        return x1 <= x && x <= x2 && y1 <= y && y <= y2;
    }

    bool contains(float x, int split_dimensions)
    {
        return *this->map[split_dimensions] <= x && x <= *this->map[split_dimensions + 2];
    }

    float get(bool axis)
    {
        return !axis ? x1 + x2 : y1 + y2;//axis == false --> Ox true --> Oy
    }

    std::vector<float> draw()
    {
        std::vector<float> list = { x1, y1, x2, y2 };
        return list;
    }
};

struct Node {
    std::vector<Rectangle> rectangles;
    int depth;
    Node* left;
    Node* right;
    int median;
    int split_dimensions;//split_dimensions == false --> Ox true --> Oy
    /*
    Node(const std::vector<Rectangle>& rects, int d = 0) {
        rectangles = rects;
        depth = d;
        left = nullptr;
        right = nullptr;
        if (rectangles.size() > 1) {
            std::priority_queue<Node*, std::vector<Node*>, NodeComparator> q;
            q.push(this);
            while (!q.empty()) {
                Node* current_node = q.top();
                q.pop();
                if (current_node->rectangles.size() <= 1) {
                    continue;
                }
                current_node->split_dimensions = current_node->depth % 2;
                std::sort(current_node->rectangles.begin(), current_node->rectangles.end(),
                    [current_node](const Rectangle& r1, const Rectangle& r2) {
                        if (current_node->split_dimensions == 0) {
                            return r1.x1 < r2.x1;
                        }
                        else {
                            return r1.y1 < r2.y1;
                        }
                    }
                );
                int median_idx = getMedian(current_node->rectangles, current_node->split_dimensions);
                Rectangle median_rectangle = current_node->rectangles[median_idx];
                std::vector<Rectangle> left_rectangles(current_node->rectangles.begin(), current_node->rectangles.begin() + median_idx);
                std::vector<Rectangle> right_rectangles(current_node->rectangles.begin() + median_idx, current_node->rectangles.end());
                current_node->left = new Node(left_rectangles, current_node->depth + 1);
                current_node->right = new Node(right_rectangles, current_node->depth + 1);
                q.push(current_node->left);
                q.push(current_node->right);
            }
        }
    }*/

    Node(const std::vector<Rectangle>& rects, int d = 0)
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
                //index = getIndex(rectangles, split_dimensions);
                std::vector<Rectangle> left_rectangles(rectangles.begin(), rectangles.begin() + (rectangles.size() == 2 ? 1 :(index + 1)));
                std::vector<Rectangle> right_rectangles(rectangles.begin() + (rectangles.size() == 2 ? 1 : (index + 1)), rectangles.end());
                left = new Node(left_rectangles, depth + 1);
                right = new Node(right_rectangles, depth + 1);
            }
    }

    ~Node()
    {
        delete left;
        delete right;
    }
    
    void sort(int split_dimensions) {
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

    float getMedian(const std::vector<Rectangle>& rects, int axis) {
        float sum = 0, iter = 0;
        for (auto x : rects)
        {
            sum += x.get(axis);
            iter += 2;
        }
        return ceil(sum / iter);
    }

    int getIndex(const std::vector<Rectangle>& rectangles, int split_dimensions)
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
};

class KdTree {
public:
    KdTree(const std::vector<Rectangle>& rects)
    {
        root_ = new Node(rects);
    }

    ~KdTree()
    {
        delete root_;
    }

    bool contains(float x, float y) const
    {
        Node* node = root_;
        while (node != nullptr)
        {
            if (node->rectangles.empty())
            {
                return false;
            }
            if (node->rectangles.size() == 1)
            {
                return node->rectangles[0].contains(x, y);
            }
            if (node->split_dimensions == 0)
            {
                if (x <= node->left->rectangles.back().x2)
                {
                    node = node->left;
                }
                else
                {
                    node = node->right;
                }
            }
            else
            {
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
private:
    Node* root_;
};

//-----------------------Create list of rectangle shapes-----------------------
std::vector<sf::ConvexShape> makeList(const std::vector<Rectangle>& fillList, std::vector<sf::ConvexShape>& outlineList, int mult) {
    std::vector<Rectangle> list = fillList;
    std::vector<sf::ConvexShape> lis;

    for (int i = 0; i < list.size(); i++)
    {
        sf::ConvexShape convex;
        convex.setPointCount(4);
        std::vector<float> coord = list[i].draw();
        float x1 = coord[0],
            y1 = coord[1],
            x2 = coord[2],
            y2 = coord[3];
        x1 *= mult;
        y1 *= mult;
        x2 *= mult;
        y2 *= mult;
        convex.setPoint(0, sf::Vector2f(x1, y1));
        convex.setPoint(1, sf::Vector2f(x1, y2));
        convex.setPoint(2, sf::Vector2f(x2, y2));
        convex.setPoint(3, sf::Vector2f(x2, y1));
        convex.setFillColor(sf::Color::Green);

        sf::ConvexShape outlineConvex(convex);
        outlineConvex.setFillColor(sf::Color::Transparent);
        outlineConvex.setOutlineColor(sf::Color::Red);
        outlineConvex.setOutlineThickness(-1.f);
        lis.push_back(convex);
        outlineList.push_back(outlineConvex);
    }
    return lis;
}

//-----------------------Sets circle/dot on coordinates-----------------------
bool drawDot(sf::CircleShape& circle, const float& inputX, const float& inputY, int mult) {
    if (inputX != NULL && inputY != NULL)
    {
        circle.setRadius(2.f);
        circle.setPointCount(100);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(inputX * mult, inputY * mult);
        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------Input Handler Gets Coordinates From CLI-----------------------
void inputHandler(sf::RenderWindow& window, float& x, float& y, KdTree& tree, bool dotDrawn) {
    while (window.isOpen())
    {
        system("cls");
        std::cout << "x = " << x << std::endl << "y = " << y << std::endl;
        std::cout << tree.contains(x, y) << std::endl;
        std::cin >> x >> y;
    }
}

int main() {
    std::vector<Rectangle> list = { {10,10,30,30}, {10,40,20,60}, {40,20,80,40}, {20,10,50,30}, {0,80,40,120}, {60,60,120,140}, {60,0,100,40}, {40,80,60,140}, {0,0,10,10}, {150,0,200,50} };//Rectangle array x1, y1, x2, y2 
    KdTree tree(list);
    float inputX = 11.f, inputY = 11.f;
    bool dotDrawn = false;
    std::cout << "Select Resolution:\n1.640x360\n2.854x480\n3.960x540\n4.1024x576\n5.1280x720" << std::endl;
    int width = 320, height = 180, option, multiplier = 1;
    std::cin >> option;
    switch (option) {
    case 1:
        width = 640;
        height = 360;
        break;
    case 2:
        width = 854;
        height = 480;
        break;
    case 3:
        width = 960;
        height = 540;
        break;
    case 4:
        width = 1024;
        height = 576;
        break;
    case 5:
        width = 1280;
        height = 720;
        break;
    default:
        width = 480;
        height = 270;
        break;
    }
    sf::RenderWindow window(sf::VideoMode(width, height), "");
    std::vector<sf::ConvexShape> outlineList;
    std::vector<sf::ConvexShape> filList = makeList(list, outlineList, multiplier);
    std::thread inputThread(inputHandler, std::ref(window), std::ref(inputX), std::ref(inputY), std::ref(tree), dotDrawn);
    sf::CircleShape circle;
    while (window.isOpen())
    {
        bool dot = false;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                inputThread.join();
                return 0;
            }
        }
        window.clear();
        for (int i = 0; i < filList.size(); i++)
            window.draw(filList[i]);
        for (int i = 0; i < outlineList.size(); i++)
            window.draw(outlineList[i]);
        dotDrawn = drawDot(circle, inputX, inputY, multiplier);
        window.draw(circle);
        window.display();
    }
    inputThread.join();
    return 0;
}
