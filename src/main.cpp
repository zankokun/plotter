#include <iostream>
#include <clocale>
#include <cmath>
#include <vector>

#include "sciplot/Plot2D.hpp"
#include "sciplot/sciplot.hpp"
using namespace sciplot;

// Массив точек оригинально графика
static const std::vector<double> X = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180};
static const std::vector<double> Y = {260, 260, 260, 260, 259, 259, 258, 258, 257, 256, 255, 254, 253, 251, 249, 247, 245, 243, 240, 235, 229, 223, 220, 215, 213, 212, 211, 210, 209, 208, 208, 208, 207, 207, 207, 206, 206, 205, 204, 204, 204, 203, 203, 202, 201, 200, 199, 198, 196, 194, 193, 192, 190, 188, 185, 181, 178, 175, 170, 165, 161, 155, 150, 140, 135, 127, 120, 115, 110, 106, 103, 100, 97, 93, 90, 89, 87, 85, 83, 81, 79, 77, 75, 73, 72, 71, 69, 68, 67, 66, 65, 64, 63, 62, 60, 59, 58, 58, 57, 56, 55, 55, 54, 53, 53, 52, 51, 50, 49, 49, 49, 48, 48, 47, 47, 46, 46, 46, 46, 45, 45, 45, 45, 45, 45, 45, 45, 44, 45, 45, 45, 45, 45, 45, 46, 46, 47, 48, 49, 50, 52, 53, 54, 55, 57, 59, 61, 63, 65, 67, 69, 71, 73, 76, 78, 80, 83, 85, 88, 91, 93, 95, 98, 101, 105, 109, 113, 118, 120, 125, 133, 140, 145, 152, 158, 170, 180, 190, 220, 230, 260};

template <typename CustomDrawFunction, typename CustomCanvasFunction>
struct Drawer
{
    CustomDrawFunction drawFunctions;
    CustomCanvasFunction canvasFunctions;
    Drawer(CustomDrawFunction drawFunctions,
           CustomCanvasFunction canvasFunctions) : drawFunctions{drawFunctions},
                                                   canvasFunctions{canvasFunctions} {}
    void draw()
    {
        Plot2D plot;
        plot.xlabel("x");
        plot.ylabel("y");
        plot.legend()
            .atOutsideBottom()
            .displayHorizontal()
            .displayExpandWidthBy(2);
        drawFunctions(plot);
        Figure fig = {{plot}};
        Canvas canvas = {{fig}};
        canvasFunctions(canvas);
        canvas.size(800, 600);
        canvas.show();
    }
};

void origin()
{
    Drawer d{
        [](Plot2D &plot)
        {
            plot.drawCurve(X, Y).label("Оригинальный график");
        },
        [](Canvas &canvas)
        {
            canvas.title("График по точкам");
        }};
    d.draw();
}

double divided_differences(std::vector<double> x_values, std::vector<double> y_values, size_t k)
{
    double result = 0.f;
    for (size_t j = 0; j < k + 1; j++)
    {
        double mul = 1.f;
        for (size_t i = 0; i < k + 1; i++)
        {
            if (i != j)
            {
                mul *= x_values[j] - x_values[i];
            }
        }
        result += y_values[j] / mul;
    }
    return result;
}

auto create_Newton_polynomial(std::vector<double> x_values, std::vector<double> y_values)
{
    std::vector<double> div_diff;
    for (size_t i = 1; i < x_values.size(); i++)
    {
        div_diff.push_back(divided_differences(x_values, y_values, i));
    }
    auto resultFunc = [div_diff, x_values, y_values](double x)
    {
        double result = y_values[0];
        for (size_t k = 1; k < y_values.size(); k++)
        {
            double mul = 1.f;
            for (size_t j = 0; j < k; j++)
            {
                mul *= x - x_values[j];
            }
            result += div_diff[k - 1] * mul;
        }
        return result;
    };
    return resultFunc;
}

void newton(size_t k)
{
    std::vector<double> x_values;
    std::vector<double> y_values;
    if (k == 3)
    {
        x_values = {X.front(), X[90], X.back()};
        y_values = {Y.front(), Y[90], Y.back()};
    }
    if (k == 5)
    {
        x_values = {X.front(), X[50], X[100], X[150], X.back()};
        y_values = {Y.front(), Y[50], Y[100], Y[150], Y.back()};
    }
    if (k == 8)
    {
        x_values = {X.front(), X[25], X[50], X[75], X[100], X[125], X[150], X.back()};
        y_values = {Y.front(), Y[25], Y[50], Y[75], Y[100], Y[125], Y[150], Y.back()};
    }
    auto newtonFunc = create_Newton_polynomial(x_values, y_values);
    std::vector<double> new_Y;
    for (auto x : X)
    {
        new_Y.push_back(newtonFunc(x));
    }

    Drawer d{
        [new_Y](Plot2D &plot)
        {
            plot.drawCurve(X, Y).label("Оригинальная функция");
            plot.drawCurve(X, new_Y).label("Интерполяция Ньютона");
        },
        [](Canvas &canvas) {

        }};
    d.draw();
}

auto create_basic_polynomial(std::vector<double> x_values, size_t i)
{
    return [x_values, i](double x)
    {
        double divider = 1.f;
        double result = 1.f;
        for (size_t j = 0; j < x_values.size(); j++)
        {
            if (j != i)
            {
                result *= x - x_values[j];
                divider *= x_values[i] - x_values[j];
            }
        }
        return result / divider;
    };
}

auto create_Lagrange_polynomial(std::vector<double> x_values, std::vector<double> y_values)
{
    std::vector<std::function<double(double x)>> basic_polynomials{};
    for (size_t i = 0; i < x_values.size(); i++)
    {
        basic_polynomials.push_back(create_basic_polynomial(x_values, i));
    }

    return [y_values, basic_polynomials](double x)
    {
        double result = 0.f;
        for (size_t i = 0; i < y_values.size(); i++)
        {
            result += y_values[i] * basic_polynomials[i](x);
        }
        return result;
    };
}

void lagranzh(size_t k)
{
    std::vector<double> x_values;
    std::vector<double> y_values;
    if (k == 3)
    {
        x_values = {X.front(), X[90], X.back()};
        y_values = {Y.front(), Y[90], Y.back()};
    }
    if (k == 5)
    {
        x_values = {X.front(), X[50], X[100], X[150], X.back()};
        y_values = {Y.front(), Y[50], Y[100], Y[150], Y.back()};
    }
    if (k == 8)
    {
        x_values = {X.front(), X[25], X[50], X[75], X[100], X[125], X[150], X.back()};
        y_values = {Y.front(), Y[25], Y[50], Y[75], Y[100], Y[125], Y[150], Y.back()};
    }
    auto larganzhFunc = create_Lagrange_polynomial(x_values, y_values);
    std::vector<double> new_Y;
    for (auto x : X)
    {
        new_Y.push_back(larganzhFunc(x));
    }

    Drawer d{
        [new_Y](Plot2D &plot)
        {
            plot.drawCurve(X, Y).label("Оригинальная функция");
            plot.drawCurve(X, new_Y).label("Интерполяция Лагранжа");
        },
        [](Canvas &canvas) {

        }};
    d.draw();
}

void approx()
{
    std::vector<double> a, b, x, y;
    std::vector<std::vector<double>> sums;
    int N = 10, K = 3;
    {
        // allocate memory for matrixes
        a = std::vector<double>(K + 1, 0);
        b = std::vector<double>(K + 1, 0);
        x = std::vector<double>(N, 0);
        y = std::vector<double>(N, 0);
        sums = std::vector<std::vector<double>>(K + 1, std::vector<double>(K + 1, 0));
    }
    {
        int i = 0, j = 0, k = 0;
        // read x, y matrixes from input file
        for (k = 0; k < N; k++)
        {
            x[k] = X[((x.size()) / N) * k];
            y[k] = Y[((x.size()) / N) * k];
        }
        // init square sums matrix
        for (i = 0; i < K + 1; i++)
        {
            for (j = 0; j < K + 1; j++)
            {
                sums[i][j] = 0;
                for (k = 0; k < N; k++)
                {
                    sums[i][j] += powl(x[k], i + j);
                }
            }
        }
        // init free coefficients column
        for (i = 0; i < K + 1; i++)
        {
            for (k = 0; k < N; k++)
            {
                b[i] += powl(x[k], i) * y[k];
            }
        }
    }

    {
        int i, j, k;
        double temp = 0;
        for (i = 0; i < K + 1; i++)
        {
            if (sums[i][i] == 0)
            {
                for (j = 0; j < K + 1; j++)
                {
                    if (j == i)
                        continue;
                    if (sums[j][i] != 0 && sums[i][j] != 0)
                    {
                        for (k = 0; k < K + 1; k++)
                        {
                            temp = sums[j][k];
                            sums[j][k] = sums[i][k];
                            sums[i][k] = temp;
                        }
                        temp = b[j];
                        b[j] = b[i];
                        b[i] = temp;
                        break;
                    }
                }
            }
        }
    }
    {
        int i = 0, j = 0, k = 0;
        // process rows
        for (k = 0; k < K + 1; k++)
        {
            for (i = k + 1; i < K + 1; i++)
            {
                if (sums[k][k] == 0)
                {
                    printf("\nSolution is not exist.\n");
                    return;
                }
                double M = sums[i][k] / sums[k][k];
                for (j = k; j < K + 1; j++)
                {
                    sums[i][j] -= M * sums[k][j];
                }
                b[i] -= M * b[k];
            }
        }

        for (i = (K + 1) - 1; i >= 0; i--)
        {
            double s = 0;
            for (j = i; j < K + 1; j++)
            {
                s = s + sums[i][j] * a[j];
            }
            a[i] = (b[i] - s) / sums[i][i];
        }
    }
}

int main()
{
    // #if defined(_WIN32) || defined(_WIN64)
    //setlocale(LC_ALL, "Russian");
    // #endif

    std::string input;
    std::map<std::string, std::pair<std::string, std::function<void(void)>>> menu = {
        {{"O"}, {"Оригинальная функция", []()
                 { origin(); }}},
        {{"A"}, {"Аппроксимация методом наименьших квадратов", []()
                 { approx(); }}},
        {{"L3"}, {"Интерполяция Лагранжа по 3 точкам", []()
                  { lagranzh(3); }}},
        {{"L5"}, {"Интерполяция Лагранжа по 5 точкам", []()
                  { lagranzh(5); }}},
        {{"L8"}, {"Интерполяция Лагранжа по 8 точкам", []()
                  { lagranzh(8); }}},
        {{"N3"}, {"Интерполяция Ньютона по 3 точкам", []()
                  { newton(3); }}},
        {{"N5"}, {"Интерполяция Ньютона по 5 точкам", []()
                  { newton(5); }}},
        {{"N8"}, {"Интерполяция Ньютона по 8 точкам", []()
                  { newton(8); }}},
        //{}
    };
    for (;;)
    {
        for (auto &[cmd, value] : menu)
        {
            std::cout << cmd << ": " << value.first << std::endl;
        }
        /*if (input == "O"){
            origin();
        }
        else if (input == "N3")
        {
            newton(3);
        }
        else if (input == "N5")
        {
            newton(5);
        }
        else if (input == "N8")
        {
            newton(8);
        }
        else if (input == "L3"){
            lagranzh(3);
        }
        else if (input == "L5"){
            lagranzh(5);
        }
        else if (input == "L8"){
            lagranzh(8);
        }
        */
        std::getline(std::cin, input);
        if (menu[input].first.size() > 0)
        {
            menu[input].second();
        }
        else if (input == "q")
        {
            return 0;
        }
        else
        {
            std::cout << "Неверная команда!" << std::endl;
        }
    }
}
