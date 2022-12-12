#include <iostream>
#include <clocale>
#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>

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
    void draw(bool show = true)
    {
        Plot2D plot;
        plot.xlabel("x");
        plot.ylabel("y");
        plot.legend().atOutsideRight().displayVertical().displayExpandHeightBy(2);
        drawFunctions(plot);
        Figure fig = {{plot}};
        Canvas canvas = {{fig}};
        canvasFunctions(canvas);
        canvas.size(1200, 720);
        if (show)
        {
            canvas.show();
        }
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
        [&](Plot2D &plot)
        {
            plot.drawCurve(X, Y).label("Оригинальная функция");
            plot.drawCurve(X, new_Y).label("Итог Интерполяции");
            plot.drawPoints(x_values, y_values).label("Точки интерполяции").pointSize(1).pointType(7);
        },
        [](Canvas &canvas)
        {
            canvas.title("Интерполяция Ньютона");
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
        [&](Plot2D &plot)
        {
            plot.drawCurve(X, Y).label("Оригинальная функция");
            plot.drawCurve(X, new_Y).label("Итог Интерполяции");
            plot.drawPoints(x_values, y_values).label("Точки интерполяции").pointSize(1).pointType(7);
        },
        [](Canvas &canvas)
        {
            canvas.title("Интерполяция Лагранжа");
        }};
    d.draw();
}

void approx(size_t K)
{
    std::vector<double> a, b, x, y;
    std::vector<std::vector<double>> sums;

    x = {X.front(), X[25], X[50], X[75], X[85], X[100], X[125], X[135], X[145], X[150], X[165], X[175], X.back()};
    y = {Y.front(), Y[25], Y[50], Y[75], Y[85], Y[100], Y[125], Y[135], Y[145], Y[150], Y[165], Y[175], Y.back()};
    size_t N = x.size();
    {
        // allocate memory for matrixes
        a = std::vector<double>(K + 1, 0);
        b = std::vector<double>(K + 1, 0);
        sums = std::vector<std::vector<double>>(K + 1, std::vector<double>(K + 1, 0));
    }
    {
        int i = 0, j = 0, k = 0;
        // init square sums matrix
        for (i = 0; i < K + 1; i++)
        {
            for (j = 0; j < K + 1; j++)
            {
                sums[i][j] = 0;
                for (k = 0; k < N; k++)
                {
                    sums[i][j] += powl(x[k], static_cast<long double>(i + j));
                }
            }
        }
        // init free coefficients column
        for (i = 0; i < K + 1; i++)
        {
            for (k = 0; k < N; k++)
            {
                b[i] += powl(x[k], static_cast<long double>(i)) * y[k];
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
                    std::wcout << L"Нет решений матрицы!" << std::endl;
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

        for (i = static_cast<int>(K + 1) - 1; i >= 0; i--)
        {
            double s = 0;
            for (j = i; j < K + 1; j++)
            {
                s = s + sums[i][j] * a[j];
            }
            a[i] = (b[i] - s) / sums[i][i];
        }
    }

    double error2 = 0.f;
    std::vector<double> new_Y;
    for (size_t i = 0; i < X.size(); i++)
    {
        auto x = X[i];
        auto y = Y[i];
        // a[0] + a[1] * x + a[2] * x * x+a[3]*x*x*x
        double new_y = 1.f, pow_x = 1.f;
        for (size_t i = 0; i <= K; i++)
        {
            new_y += a[i] * pow_x;
            pow_x *= x;
        }
        new_Y.push_back(new_y);
        error2 += (new_y - y) * (new_y - y);
    }

    double error = sqrtl(error2 / static_cast<double>(X.size() - 1));
    std::wcout << L"Среднеквадратическое отклонение равно: " << error << std::endl;

    Drawer d{
        [&](Plot2D &plot)
        {
            plot.drawCurve(X, Y).label("Оригинальная функция");
            plot.drawCurve(X, new_Y).label("Итог Аппроксимации");
        },
        [](Canvas &canvas)
        {
            canvas.title("Аппроксимация методом наименьших квадратов");
        }};
    d.draw();
}

struct spline_attributes
{
    double a, b, c, d, x;
};

std::vector<spline_attributes>
buildSpline(std::vector<double> x, std::vector<double> y, size_t n)
{
    auto splines = std::vector<spline_attributes>(n);
    for (size_t i = 0; i < n; i++)
    {
        splines[i].x = x[i];
        splines[i].a = y[i];
    }
    splines[0].c = splines[n - 1].c = 0.f;
    auto alpha = std::vector<double>(n - 1);
    auto beta = std::vector<double>(n - 1);
    for (size_t i = 1; i < n - 1; i++)
    {
        auto hi = x[i] - x[i - 1];
        auto hi1 = x[i + 1] - x[i];
        auto A = hi;
        auto C = 2.0 * (hi + hi1);
        auto B = hi1;
        auto F = 6.0 * ((y[i + 1] - y[i]) / hi1 - (y[i] - y[i - 1]) / hi);
        auto z = (A * alpha[i - 1] + C);
        alpha[i] = -B / z;
        beta[i] = (F - A * beta[i - 1]) / z;
    }
    for (size_t i = n - 2; i > 0; i--)
    {
        splines[i].c = alpha[i] * splines[i + 1].c + beta[i];
    }
    for (size_t i = n - 1; i > 0; i--)
    {
        auto hi = x[i] - x[i - 1];
        splines[i].d = (splines[i].c - splines[i - 1].c) / hi;
        splines[i].b = hi * (2.0 * splines[i].c + splines[i - 1].c) / 6.0 + (y[i] - y[i - 1]) / hi;
    }
    return splines;
}

double interpolate(std::vector<spline_attributes> splines, double x)
{
    auto n = splines.size();
    auto s = spline_attributes{};
    if (x <= splines[0].x)
    {
        s = splines[0];
    }
    else if (x >= splines[n - 1].x)
    {
        s = splines[n - 1];
    }
    else
    {
        size_t i = 0;
        auto j = n - 1;
        while (i + 1 < j)
        {
            auto k = i + (j - i) / 2;
            if (x <= splines[k].x)
            {
                j = k;
            }
            else
            {
                i = k;
            }
        }
        s = splines[j];
    }
    auto dx = x - s.x;
    return s.a + (s.b + (s.c / 2.0 + (s.d * dx) / 6.0) * dx) * dx;
}

void spline()
{
    std::vector<double> x_values;
    std::vector<double> y_values;

    x_values = {X.front(), X[25], X[50], X[75], X[100], X[125], X[150], X.back()};
    y_values = {Y.front(), Y[25], Y[50], Y[75], Y[100], Y[125], Y[150], Y.back()};

    auto spline = buildSpline(x_values, y_values, x_values.size());

    auto larganzhFunc = create_Lagrange_polynomial(x_values, y_values);
    std::vector<double> new_Y;
    for (auto x : X)
    {
        new_Y.push_back(interpolate(spline, x));
    }

    Drawer d{
        [&](Plot2D &plot)
        {
            plot.drawCurve(X, Y).label("Оригинальная функция");
            plot.drawCurve(X, new_Y).label("Итог Интерполяции");
            plot.drawPoints(x_values, y_values).label("Точки интерполяции").pointSize(1).pointType(7);
        },
        [](Canvas &canvas)
        {
            canvas.title("Интерполяция Сплайном");
        }};
    d.draw();
}

void diff()
{
    double delta = 1;
    std::vector<double> righty;
    std::vector<double> rightx;
    std::vector<double> lefty;
    std::vector<double> leftx;
    std::vector<double> centrey;
    std::vector<double> centrex;
    std::vector<double> diff2y;
    std::vector<double> diff2x;

    for (size_t i = 0; i < X.size() - 1; i++)
    {
        double temp = (Y[i + static_cast<int64_t>(delta)] - Y[i]) / (delta * 2.0);
        righty.push_back(temp);
        rightx.push_back(X[i]);
    }
    for (size_t i = 1; i < X.size(); i++)
    {
        double temp = (Y[i] - Y[i - static_cast<int64_t>(delta)]) / (delta * 2.0);
        lefty.push_back(temp);
        leftx.push_back(X[i]);
    }
    for (size_t i = 1; i < X.size() - 1; i++)
    {
        double temp = (Y[i + static_cast<int64_t>(delta)] - Y[i - static_cast<int64_t>(delta)]) / (delta * 2.0);
        centrey.push_back(temp);
        centrex.push_back(X[i]);
    }

    for (size_t i = 2; i < X.size() - 2; i++)
    {
        auto k = i / 2;
        double temp = (centrey[k + 1] - 2 * centrey[k] + centrey[k - 1]) / ((2 * delta * 2) * (2 * delta * 2));
        diff2y.push_back(temp);
        diff2x.push_back(X[i]);
    }

    {
        Drawer d{
            [&](Plot2D &plot)
            {
                plot.drawCurve(rightx, righty).label("Разностное отношение вперёд");
            },
            [](Canvas &canvas)
            {
                canvas.title("Разностное отношение вперёд");
            }};
        d.draw();
    }
    {
        Drawer d{
            [&](Plot2D &plot)
            {
                plot.drawCurve(leftx, lefty).label("Разностное отношение назад");
            },
            [](Canvas &canvas)
            {
                canvas.title("Разностное отношение назад");
            }};
        d.draw();
    }
    {
        Drawer d{
            [&](Plot2D &plot)
            {
                plot.drawCurve(centrex, centrey).label("Центральное разностное отношение");
            },
            [](Canvas &canvas)
            {
                canvas.title("Центральное разностное отношение");
            }};
        d.draw();
    }
    {
        Drawer d{
            [&](Plot2D &plot)
            {
                plot.drawCurve(diff2x, diff2y).label("Вторая производная");
            },
            [](Canvas &canvas)
            {
                canvas.title("Вторая производная");
            }};
        d.draw();
    }
    {
        Drawer d{
            [&](Plot2D &plot)
            {
                plot.drawCurve(rightx, righty).label("Разностное отношение вперёд");
                plot.drawCurve(leftx, lefty).label("Разностное отношение назад");
                plot.drawCurve(centrex, centrey).label("Центральное разностное отношение");
            },
            [](Canvas &canvas)
            {
                canvas.title("Сравнение разностных отношений");
            }};
        d.draw();
    }
}

void integration()
{
    double h = 1.f;
    size_t n = Y.size() - 1;

    double ff = std::accumulate(Y.begin(), Y.end(), static_cast<long double>(0));
    double k1 = Y.front();
    double k2 = Y.back();
    double left = h * (ff - k2);

    std::wcout << L"Метод левых прямоугольников:\t" << left << std::endl;

    double right = h * (ff - k1);
    std::wcout << L"метод правых прямоугольников:\t" << right << std::endl;

    std::vector<double> dx{};
    for (size_t i = 1; i < n - 1; i++)
    {
        dx.push_back((Y[i + 1] - Y[i - 1]) / h);
    }
    std::transform(dx.begin(), dx.end(),
                   dx.begin(), // write to the same location
                   [](double d)
                   { return d > 0 ? d : d * -1.f; });
    auto mm = std::max_element(dx.begin(), dx.end());
    auto pogrechnost = *mm * (h * h * (n - 1) / 2);

    std::wcout << L"Абсолютная погрешность метода левых и правых прямоугольников:\t" << pogrechnost << std::endl;

    double h4 = 1.f;
    // метод центральных прямоугольников
    double summ = 0.f;
    for (size_t i = 0; i < n - 3; i++)
    {
        summ = summ + Y[(i + 2 + i) / 2];
    }
    auto sredn = h4 * summ + Y[n - 1] * 2;
    std::wcout << L"метод центральных прямоугольников:\t" << sredn << std::endl;

    std::vector<double> dx2{};
    for (size_t i = 3; i < n - 2; i++)
    {
        //% вторая производная
        auto k = i / 2;
        dx2.push_back((dx[k + 1] - 2 * dx[k] + dx[k - 1]) / ((2 * h) * (2 * h)));
    }
    std::transform(dx2.begin(), dx2.end(),
                   dx2.begin(), // write to the same location
                   [](double d)
                   { return d > 0 ? d : d * -1.f; });
    auto mm2 = std::max_element(dx2.begin(), dx2.end());

    auto pogrechnost2 = *mm2 * (h * h * h * (n - 1) / 24);
    //% погрешность метода
    std::wcout << L"Абсолютная погрешность метода средних прямоугольников:\t" << pogrechnost2 << std::endl;

    auto trap = h * ((k1 + k2) / 2 + (ff - (k1 + k2)));
    //% метод трапеции
    std::wcout << L"метод трапеции" << trap << std::endl;
    auto pogrechnost3 = *mm2 * (h * h * h * (n - 1) / 12);
    // % погрешность метода
    std::wcout << L"Абсолютная погрешность метода трапеции:\t" << pogrechnost3 << std::endl;

    double summ1 = 0;
    //% вычисление сумм для метода Симпсона
    double summ2 = 0;
    for (size_t i = 2; i < n - 2; i++)
    {
        summ1 = summ1 + Y[i];
    }
    for (size_t i = 3; i < n - 1; i++)
    {
        summ2 = summ2 + Y[i];
    }

    auto h3 = h / 6.f;
    auto simpson = h3 * (k1 + k2 + 4.f * summ1 + 2.f * summ2);
    //% метод Симпсона
    std::wcout << L"метод Симпсона:\t" << simpson << std::endl;

    std::vector<double> dx3{};

    for (size_t i = 2; i < n - 2; i++)
    {
        dx3.push_back(Y[i - 2] - 4.f * Y[i - 1] + 6.f * Y[i] - 4.f * Y[i - 1] - Y[i - 2] / (h * h * h * h));
    }
    std::transform(dx3.begin(), dx3.end(),
                   dx3.begin(), // write to the same location
                   [](double d)
                   { return d > 0 ? d : d * -1.f; });
    auto mm3 = std::max_element(dx3.begin(), dx3.end());

    auto a = (n - 1) * h;
    auto pogrechnost4 = (*mm3 * a * a * a * a * a) / (2880.f * (n - 1) * (n - 1) * (n - 1) * (n - 1));
    //% погрешность метода
    std::wcout << L"Абсолютная погрешность метода Симпсона:" << pogrechnost4 << std::endl;
}

void mathModel()
{
    std::vector<double> x_values;
    std::vector<double> y_values;

    x_values = {X.front(), X[15], X[25], X[35], X[50], X[60], X[70], X[75], X[90], X[100], X[125], X[145], X[160], X[170], X[175], X.back()};
    y_values = {Y.front(), Y[15], Y[25], Y[35], Y[50], Y[60], Y[70], Y[75], Y[90], Y[100], Y[125], Y[145], Y[160], Y[170], Y[175], Y.back()};

    auto spline = buildSpline(x_values, y_values, x_values.size());

    auto larganzhFunc = create_Lagrange_polynomial(x_values, y_values);
    std::vector<double> new_Y;
    for (auto x : X)
    {
        new_Y.push_back(interpolate(spline, x));
    }

    double x_0;
    std::wcout << L"Введите Xo = " << std::endl;
    std::wcin >> x_0;

    // Скорость
    auto x_m = X;
    auto y_m = new_Y;
    size_t i0 = 0;
    while (x_m[i0] < x_0)
    {
        i0++;
    }
    double y_0 = y_m[i0];
    double C = 9.8f * y_0;
    auto tg = std::vector<double>(y_m.size(), 0);
    tg[0] = y_m[1] / x_m[1];

    for (size_t i = 1; i < y_m.size(); i++)
    {
        tg[i] = (y_m[i] - y_m[i - 1]) / (x_m[i] - x_m[i - 1]);
    }

    auto Y_x = std::vector<double>(y_m.size(), 0);
    for (size_t i = 0; i < y_m.size(); i++)
    {
        Y_x[i] = std::abs(std::sqrtl(2.f * (C - 9.8f * y_m[i]) / (1.f + (tg[i]) * (tg[i]))));
    }

    auto delta_t = std::vector<double>(Y_x.size(), 0);
    for (size_t i = 0; i < Y_x.size(); i++)
    {
        delta_t[i] = 0.5f / Y_x[i];
    }

    auto YY = std::vector<double>(x_m.size(), 0);
    for (size_t i = i0; i < x_m.size(); i++)
    {
        if (y_m[i] <= y_0)
        {
            YY[i] = Y_x[i];
        }
    }

    {
        Drawer d{
            [&](Plot2D &plot)
            {
                plot.drawCurve(X, Y).label("Оригинальная функция");
                plot.drawCurve(x_m, YY).label("Функция Скорости");
            },
            [](Canvas &canvas)
            {
                canvas.title("Скорость");
            }};
        d.draw();
    }

    {
        std::system("del *.gif  > nul 2>&1");
        std::system("del *.png  > nul 2>&1");
        std::system("del *.plt  > nul 2>&1");
        int flag, a;

        if (tg[i0] < 0.f)
        {
            flag = 1;
            a = 1;
        }
        else
        {
            flag = 0;
            a = -1;
        }

        int j = 0;
        int i = static_cast<int>(i0) + a;

        while (j < 2)
        {
            while (y_0 > y_m[i])
            {

                Drawer d{
                    [&](Plot2D &plot)
                    {
                        std::vector<double> XX = {X[i]};
                        std::vector<double> YY = {Y[i]};
                        plot.drawCurve(X, Y).label("Оригинальная функция");
                        plot.drawPoints(XX, YY).label("Материальная точка").pointSize(1).pointType(7);
                    },
                    [&](Canvas &canvas)
                    {
                        std::string name = "pic";
                        static int ii = 1;
                        std::wcout << L"Конвертация модели в изображения, шаг:  \t" << ii << std::endl;
                        name = name + std::to_string(ii++) + ".png";
                        canvas.title("Материальная точка");
                        canvas.size(1200, 720);
                        canvas.save(name);
                    }};
                d.draw(false);
                i = i + a;
            }
            if (flag == 1)
            {
                flag = 0;
                a = -1;
            }
            else
            {
                flag = 1;
                a = 1;
            }
            i = i + a;
            j = j + 1;
        }
        std::system("ffmpeg -f image2 -i pic%d.png model.gif > nul 2>&1");
        std::system("del *.png  > nul 2>&1");
        std::system("del *.plt  > nul 2>&1");
    }
}

int main()
{
    setlocale(LC_ALL, ".UTF8");

    std::wstring input;
    std::map<std::wstring, std::pair<std::wstring, std::function<void(void)>>> menu = {
        {{L"O"}, {L"Оригинальная функция", []()
                  { origin(); }}},
        {{L"A2"}, {L"Аппроксимация методом наименьших квадратов для параболы", []()
                   { approx(2); }}},
        {{L"A3"}, {L"Аппроксимация методом наименьших квадратов для кубики", []()
                   { approx(3); }}},
        {{L"A5"}, {L"Аппроксимация методом наименьших квадратов для полинома пятой степени", []()
                   { approx(5); }}},
        {{L"S"}, {L"Аппроксимация Сплайнами", []()
                  { spline(); }}},
        {{L"D"}, {L"Дифференцирование", []()
                  { diff(); }}},
        {{L"I"}, {L"Интегрирование", []()
                  { integration(); }}},
        {{L"M"}, {L"Математическая модель", []()
                  { mathModel(); }}},
        {{L"L3"}, {L"Интерполяция Лагранжа по 3 точкам", []()
                   { lagranzh(3); }}},
        {{L"L5"}, {L"Интерполяция Лагранжа по 5 точкам", []()
                   { lagranzh(5); }}},
        {{L"L8"}, {L"Интерполяция Лагранжа по 8 точкам", []()
                   { lagranzh(8); }}},
        {{L"N3"}, {L"Интерполяция Ньютона по 3 точкам", []()
                   { newton(3); }}},
        {{L"N5"}, {L"Интерполяция Ньютона по 5 точкам", []()
                   { newton(5); }}},
        {{L"N8"}, {L"Интерполяция Ньютона по 8 точкам", []()
                   { newton(8); }}},
        {{L"q"}, {L"Выход", []()
                  { exit(0); }}},
        //{}
    };
    for (;;)
    {
        std::wcout << L"Ввод:\t Команда" << std::endl;
        for (auto &[cmd, value] : menu)
        {
            std::wcout << cmd << L":\t " << value.first << std::endl;
        }

        std::wcin >> input;
        if (menu[input].first.size() > 0)
        {
            menu[input].second();
        }
        else
        {
            std::wcout << L"Неверная команда!" << std::endl;
        }
    }
}
