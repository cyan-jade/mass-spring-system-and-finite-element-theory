#include <iostream>
#include <vector>
using namespace std;
const double p = 77;     
const double q = 77;         //StVK模型参数，由物体材质决定
struct Point
{
    double x, y, z;    // 坐标
    double vx, vy, vz; // 速度
    double m;          // 质量
    double fx, fy, fz; // 受力
};
typedef vector<vector<double>> Matrix;

double determinant(const Matrix &matrix)
{ // 矩阵行列式
    double det = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
                 matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
                 matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    return det;
}

double trace(const Matrix &a)
{ // 矩阵的迹
    double tra = 0.0;
    for (int i = 0; i < 3; i++)
    {
        tra += a[i][i];
    }
    return tra;
}

Matrix transposition(const Matrix &matrix)
{ // 矩阵转置
    Matrix trans(3, vector<double>(3));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            trans[i][j] = matrix[j][i];
        }
    }
    return trans;
}

Matrix subtract(const Matrix &a, const Matrix &b)
{ // 矩阵减法
    Matrix sub(3, vector<double>(3));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            sub[i][j] = a[i][j] - b[i][j];
        }
    }
    return sub;
}

Matrix sum(const Matrix &a, const Matrix &b)
{ // 矩阵加法
    Matrix s(3, vector<double>(3));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            s[i][j] = a[i][j] + b[i][j];
        }
    }
    return s;
}

Matrix inverse(const Matrix &matrix)
{ // 矩阵求逆
    double det = determinant(matrix);
    Matrix inv(3, vector<double>(3));
    inv[0][0] = (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) / det;
    inv[0][1] = (matrix[0][2] * matrix[2][1] - matrix[0][1] * matrix[2][2]) / det;
    inv[0][2] = (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]) / det;
    inv[1][0] = (matrix[1][2] * matrix[2][0] - matrix[1][0] * matrix[2][2]) / det;
    inv[1][1] = (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]) / det;
    inv[1][2] = (matrix[0][2] * matrix[1][0] - matrix[0][0] * matrix[1][2]) / det;
    inv[2][0] = (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]) / det;
    inv[2][1] = (matrix[0][1] * matrix[2][0] - matrix[0][0] * matrix[2][1]) / det;
    inv[2][2] = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) / det;
    return inv;
}

Matrix multiply(const Matrix &a, const Matrix &b)
{ // 矩阵乘法

    Matrix result(3, vector<double>(3, 0));

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

Matrix nummultiply(const Matrix &a, double k)
{ // 矩阵数乘
    Matrix num(3, vector<double>(3));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            num[i][j] = a[i][j] * k;
        }
    }
    return num;
}

Matrix composite(const vector<double> &a_1, const vector<double> &a_2, const vector<double> &a_3)
{ // 向量结合成矩阵
    vector<vector<double>> A(3, vector<double>(3));
    for (int i = 0; i < 3; ++i)
    {
        A[i][0] = a_1[i];
        A[i][1] = a_2[i];
        A[i][2] = a_3[i];
    }
    return A;
}

Matrix stress(const vector<double> &X_10, const vector<double> &X_20, const vector<double> &X_30, const Matrix &x)
{ // StVK求应力
    Matrix D = composite(X_10, X_20, X_30);
    Matrix D1 = inverse(D);
    Matrix D2 = transposition(D1);
    Matrix F = multiply(x, D1);
    Matrix FT = transposition(F);
    Matrix I = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Matrix F1 = multiply(FT, F);
    Matrix G0 = subtract(F1, I);
    Matrix G = nummultiply(G0, 0.5);
    Matrix dW1 = nummultiply(G, 2 * p);
    double tG = trace(G);
    Matrix dW2 = nummultiply(I, q * tG);
    Matrix dW = sum(dW1, dW2);
    Matrix P = multiply(F, dW);
    Matrix f = nummultiply(multiply(P, D2), 1 / (determinant(D1) * 6));
    return f;
}
int main()
{
    const double dt = 0.01;      //时间步长
    const double steps = 100;
    vector<Point> A(4);
    for (int i = 0; i < 4; i++)
    {
        A[i].fx = 0;
        A[i].fy = 0;
        A[i].fz = 0;
        A[i].m = 1.0;
        A[i].vx = 0.0;
        A[i].vy = 0.0;
        A[i].vz = 0.0;
        A[i].x = 0.0;
        A[i].y = 0.0;
        A[i].z = 0.0;
    }
    A[1].x = 1.0;
    A[2].y = 1.0;
    A[3].z = 1.0;
    Matrix f;
    const Matrix x = {{2.0, 0, 0}, {0, 2.0, 0}, {0, 0, 2.0}};
    vector<double> x_10(3), x_20(3), x_30(3); // 这里要注意直接分配内存空间
    for (double t = 0; t < steps * dt; t += dt)
    {
        cout << "时间：" << t << " " << endl;
        x_10[0] = A[1].x - A[0].x;
        x_10[1] = A[1].y - A[0].y;
        x_10[2] = A[1].z - A[0].z;
        x_20[0] = A[2].x - A[0].x;
        x_20[1] = A[2].y - A[0].y;
        x_20[2] = A[2].z - A[0].z;
        x_30[0] = A[3].x - A[0].x;
        x_30[1] = A[3].y - A[0].y;
        x_30[2] = A[3].z - A[0].z;
        f = stress(x_10, x_20, x_30, x);
        for (int i = 1; i < 4; i++)
        {
            A[i].fx = f[0][i - 1];
            A[i].fy = f[1][i - 1];
            A[i].fz = f[2][i - 1];
        }
        A[0].fx = -A[1].fx - A[2].fx - A[3].fx;
        A[0].fy = -A[1].fy - A[2].fy - A[3].fy;
        A[0].fz = -A[1].fz - A[2].fz - A[3].fz;
        for (int i = 0; i < 4; i++)
        {
            A[i].vx += dt * A[i].fx / A[i].m;
            A[i].vy += dt * A[i].fy / A[i].m;
            A[i].vz += dt * A[i].fz / A[i].m;
            A[i].x += dt * A[i].vx;
            A[i].y += dt * A[i].vy;
            A[i].z += dt * A[i].vz;
            cout << "第" << i + 1 << "个"
                 << " "
                 << "位置：" << A[i].x << "," << A[i].y << "," << A[i].z << " "
                 << "速度：" << A[i].vx << "," << A[i].vy << "," << A[i].vz << "   ";
        }
        cout << endl;
    }
    return 0;
}