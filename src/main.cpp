#include "sciplot/sciplot.hpp"
#include <iostream>
using namespace sciplot;

struct Curve {
    Vec x;
    Vec y;
    std::string name;
};

void draw(const Curve& curve) {
    // Create a Plot object
    Plot2D plot;

    // Set the x and y labels
    plot.xlabel("x");
    plot.ylabel("y");
    // Set the x and y ranges
    plot.xrange(0.0, curve.x.max());
    plot.yrange(0.0, curve.y.max());
    
    // Set the legend to be on the bottom along the horizontal
    plot.legend()
        .atOutsideBottom()
        .displayHorizontal()
        .displayExpandWidthBy(2);

    plot.drawCurve(curve.x, curve.y).label(curve.name);
    plot.drawCurveWithPoints(curve.x, curve.y).label(curve.name);

    // Create figure to hold plot
    Figure fig = { {plot} };
    // Create canvas to hold figure
    Canvas canvas = { {fig} };
    canvas.title("2123132");
    canvas.fontSize(14);
    canvas.size(800, 600);
    canvas.show();
}

int main()
{


    auto  c = Curve{ {1,2,3,4,5,6,7,8}, {1,8,27,3,4,5,6,1}, "a[[a[a[" };
    draw(c);
    char key;
    while(std::cin >> key) {
        if (key == 'c') {
            //canvas.close();
        }
        if (key == 'q') {
            return 0;
        }
    }
}
