#include<iostream>
#include<vector>
using namespace std;
const double k = 1.0;//弹性系数
class point
{
public:
    double x; //位置
    double v; //速度
    double m; //质量
    double k; //弹性系数

    point(double initialx,double m, double k):x(initialx),v(0),m(m),k(k){}

    void update(double dt){
        double f = -k * x;
        double a = f/m;
        x += v * dt;
        v += a * dt;
    }
};
int main(){
    double m = 1.0;
    double k = 1.0;
    double initialx = 1.0;
    double timestep = 0.01;
    int steps = 500;

    point one(initialx, m, k);

    for(int i = 0; i < steps; i++){
        one.update(timestep);
        cout<<"时间："<<i * timestep<<"，位置"<<one.x<<"，速度"<<one.v<<endl;
    }
    return 0;
}