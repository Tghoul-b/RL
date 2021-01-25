/*
常蒙特卡罗方法可以粗略地分成两类：一类是所求解的问题本身具有内在的随机性，借助计算机的运算能力可以直接模拟这种随机的过程。
例如在核物理研究中，分析中子在反应堆中的传输过程。
中子与原子核作用受到量子力学规律的制约，人们只能知道它们相互作用发生的概率，却无法准确获得中子与原子核作用时的位置以及裂变产生的新中子的行进速率和方向。
科学家依据其概率进行随机抽样得到裂变位置、速度和方向，这样模拟大量中子的行为后，经过统计就能获得中子传输的范围，作为反应堆设计的依据。
另一种类型是所求解问题可以转化为某种随机分布的特征数，比如随机事件出现的概率，或者随机变量的期望值。通过随机抽样的方法，以随机事件出现的频率估计其概率，或者以抽样的数字特征估算随机变量的数字特征，并将其作为问题的解。
这种方法多用于求解复杂的多维积分问题。
*/
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cmath>
#include<time.h>
using namespace std;
const int N=1000100;//取样次数

double g(double x){//g(x)=x^2
    return x*x;
}
double definite(double l,double r){//计算 x^2在[l,r]之间的积分
    double sum=0;
    for(int i=0;i<N;i++){
        double rnd = (double) rand()/RAND_MAX * (r-l) + l;
        sum+=(r-l)*g(rnd);
    }
    return sum/N;
}
double calPI(double r){//计算PI的值
    double sum=0;
    double xmin=-r,xmax=r;
    double ymin=-r,ymax=r;
    for(int i=0;i<N;i++){
        double x=(double) rand()/RAND_MAX * (xmax-xmin) + xmin;
        double y=(double) rand()/RAND_MAX * (ymax-ymin) + ymin;
        if(x*x+y*y<r*r)
           sum+=1;
    }
    return sum/N*4;
}
int main(){
    double l=-1,r=1;
    srand((unsigned)time(0));
    printf("定积分x^2在[%llf,%llf]区间中的定积分值为:%llf\n",l,r,definite(l,r));
    printf("圆周率的值大约为:%llf",calPI(1));
}