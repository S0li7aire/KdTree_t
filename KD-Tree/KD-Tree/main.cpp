#include "incl.h"

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
        std::cout << (tree.contains(x, y) ? "Dot is contained by a rectangle" : "Dot isn't contained by any rectangle") << std::endl;
        std::cin >> x >> y;
    }
}

int main() {
    std::vector<Rectangle> list = { {1,1,3,3}, {1,4,2,6}, {4,2,8,4}, {2,1,5,3}, {0,8,4,12}, {6,6,12,14}, {6,0,10,4}, {4,8,6,14}, {0,0,1,1}, {15,0,20,5} };//Rectangle array x1, y1, x2, y2 
    KdTree tree(list);
    float inputX = 11.f, inputY = 11.f;
    bool dotDrawn = false;
    std::cout << "Select Resolution:\n1.640x360\n2.854x480\n3.960x540\n4.1024x576\n5.1280x720" << std::endl;
    int width = 854, height = 480, option, multiplier = 10;
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
