#include <iostream>
#include <vector>
using namespace std;
const double k = 1.0;
struct Point
{
    double x; // 位置
    double v; // 速度
    double m; // 质量
};

double force(Point p1, Point p2, double initialx)
{
    double f = 0.0;
    if (p1.x > p2.x)
    {
        f = ((p1.x - p2.x) - initialx) * k;
    }
    else
    {
        f = (initialx - (p2.x - p1.x)) * k;
    }
    return f;
}
int main()
{
    const int num = 3;
    const double dt = 0.01;
    const double steps = 500;
    const double initialx = 2;
    vector<Point> A;
    A.push_back({0.0, 0.0, 1.0});
    A.push_back({1.0, 0.0, 1.0});
    A.push_back({2.0, 0.0, 1.0});
    for (double t = 0; t < steps * dt; t += dt)
    {
        cout << "时间：" << t << " ";
        for (int i = 0; i < num; i++)
        {
            double f = 0.0;
            if (i < num - 1)
            {
                f += force(A[i + 1], A[i], initialx);
            }
            if (i > 0)
            {
                f += force(A[i - 1], A[i], initialx);
            }
            double a = f / A[i].m;
            A[i].v = A[i].v + a * dt;
            A[i].x = A[i].x + A[i].v * dt;
            cout << "第" << i << "个"
                 << " "
                 << "位置:" << A[i].x << " "
                 << "速度：" << A[i].v;
        }
        cout << endl;
    }
    return 0;
}