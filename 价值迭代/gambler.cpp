#include<iostream>
#include<cstring>
#include<algorithm>
#include<math.h>
using namespace std;
const int N = 101;//总共101中状态   0-100
double V[N];//状态价值函数
int R[N];
int action[N];//每个状态下选择的最优动作
double account = 1.0;//折扣因子
double p_h = 0.4;//硬币向上的概率
double accuracy = 1e-4;//确定精度的小数
void value_iter() {
	memset(R, 0, sizeof R);
	R[100] = 1;//100这个状态的reward才为1
	memset(V, 0, sizeof V);
	V[100] = 0;
	double new_V[N];
	while (true) {
		memset(new_V, 0, sizeof new_V);
		double delta = 0;//用来衡量精度
		for (int s = 1; s < N-1; s++) {    //当前的赌徒手中的钱
			double tmp = -1e20;//用来记录当前状态下的最大收益
			int i = -1;//用来记录当前最大的收益对应的动作
			for (int a = 1; a <= min(s, N - 1 - s); a++) {   //赌徒下的赌注
				double t = p_h * (R[s + a] + V[s + a]) + (1 - p_h)*(R[s - a] + V[s - a]);
				if (tmp < t) {
					tmp = t;
					i = a;
				}
			}
			action[s] = i;//记录对应的最优动作
			delta = max(delta, fabs(tmp - V[s]));//计算出delta的值
			new_V[s] = tmp;//更新价值函数
		}
		memcpy(V, new_V, sizeof V);
		if (delta < accuracy)
			break;
	}
}
int main() {
	value_iter();
	for (int i = 1; i < N-1; i++) {
		cout << V[i] << ' ';
	}
	cout << endl;
	for (int i = 1; i < N - 1; i++) {
		cout << action[i] << ' ';
	}
	system("pause");
}