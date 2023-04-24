#include <iostream>
#include <vector>
using namespace std;
const double k = 1.0;
struct Point
{
    double x, y;   // 位置
    double vx, vy; // 速度
    double m;      // 质量
    double fx, fy;
};
void force(Point p1, Point &p2, double initialx)
{ // 注意这里Piont p2必须要加&表示应用，否则函数无效
    if (p1.x > p2.x)
    {
        p2.fx += ((p1.x - p2.x) - initialx) * k;
    }
    if (p1.x < p2.x)
    {
        p2.fx += (initialx - (p2.x - p1.x)) * k;
    }
    if (p1.y > p2.y)
    {
        p2.fy += ((p1.y - p2.y) - initialx) * k;
    }
    if (p1.y < p2.y)
    {
        p2.fy += (initialx - (p2.y - p1.y)) * k;
    }
}
int main()
{
    const int num = 3;
    const double dt = 0.01;
    const double steps = 100;
    const double initialx = 2;
    vector<vector<Point>> A(num, vector<Point>(num));
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            A[i][j].fx = 0;
            A[i][j].fy = 0;
            A[i][j].m = 1.0;
            A[i][j].vx = 0.0;
            A[i][j].vy = 0.0;
            A[i][j].x = i;
            A[i][j].y = j;
        }
    }
    for (double t = 0; t < steps * dt; t += dt)
    {
        cout << "时间：" << t << " " << endl;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                A[i][j].fx = 0;
                A[i][j].fy = 0;
                if (i < num - 1)
                {
                    force(A[i + 1][j], A[i][j], initialx);
                }
                if (i > 0)
                {
                    force(A[i - 1][j], A[i][j], initialx);
                }
                if (j < num - 1)
                {
                    force(A[i][j + 1], A[i][j], initialx);
                }
                if (i > 0)
                {
                    force(A[i][j - 1], A[i][j], initialx);
                }
                double ax = A[i][j].fx / A[i][j].m;
                double ay = A[i][j].fy / A[i][j].m;
                A[i][j].vx += ax * dt;
                A[i][j].vy += ay * dt;
                A[i][j].x += A[i][j].vx * dt;
                A[i][j].y += A[i][j].vy * dt;
                cout << "第" << i + 1 << "行"
                     << "第" << j + 1 << "列"
                     << " "
                     << "位置：" << A[i][j].x << "," << A[i][j].y << " "
                     << "速度：" << A[i][j].vx << "," << A[i][j].vy << "   ";
            }
            cout << endl;
        }
        cout << endl;
    }
    return 0;
}