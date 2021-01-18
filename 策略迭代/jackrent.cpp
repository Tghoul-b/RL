#include<iostream>
#include<cstring>
#include<algorithm>
#include<math.h>
using namespace std;
int rent_income = 10;//租车的收益
int move_cost = 2;//移动车辆的花费
double account = 0.9;//折扣因子
double accuracy = 1e-6;//决定精度的小正数
int max_car_capcity = 20;//最大的存放车的数量
int lambda_rent[2] = { 3,4 };
int lambda_return[2] = { 3,2 };
int max_move_num = 5;//最大移动数量
double tp[2][21][21];//两个停车场的状态转移概率矩阵。
double R[2][21];//两个停车场库存数量为x时的期望收益
int Action[21][21];//动作矩阵,在i，j状态下从停车场0向停车场1转移的车辆数目[-5,5]
double V[21][21] = { 0 };//状态价值函数矩阵
long long factorial(int num)
{
	if (num == 0) //基本情况返回1;
		return 1;
	else
		return num * factorial(num - 1);
}
double prob_calc(int lambda, int x) {//lambda表示参数的值，x表示租车/还车的数量{
	long long a1 = pow(lambda, x);
	long long  b1 = factorial(x);
	double t = exp(-lambda);
	double ans = (a1*t) / b1;
	//cout<<ans<<endl;
	return ans;
}
void transfer_prob(int num, int x) {//num表示停车场的编号，x表示库存数量

	for (int r = 0; r <= max_car_capcity; r++) {//借车的数量
		double p_r = prob_calc(lambda_rent[num], r);
		if (p_r < accuracy)
			continue;
		int rent = min(r, x);//实际租借的数量应该是租借数量和库存数量的较小值
		R[num][x] += p_r * rent*rent_income;//同一个状态不论借出去多少台车，它的动作价值期望相同
		for (int ret = 0; ret <= max_car_capcity; ret++) {//还车的数量
			double p_return = prob_calc(lambda_return[num], ret);
			if (p_return < accuracy)
				continue;
			int s_next = min(x - rent + ret, max_car_capcity);//下一个状态由这个状态的租车和借车动作之后转移
			tp[num][x][s_next] += p_r * p_return;
		}
	}
}
double cal_Action(int i, int j, int a) {//停车场0库存i,停车场1库存j，移动数量a
	if (a > i)//从停车场0移走的数量大于库存数量
		a = i;
	if (-a > j)//从停车场1移走的数量大于库存数量
		a = -j;
	int ii = min(max_car_capcity, i - a);//不能超过最大存放数量
	int jj = min(max_car_capcity, j + a);//不能超过最大存放数量
	double temp_v = -abs(a)*move_cost;//移车的代价
	for (int m = 0; m <= 20; m++) {
		for (int n = 0; n <= 20; n++) {
			temp_v += tp[0][ii][m] * tp[1][jj][n] * (R[0][ii] + R[1][jj] + account * V[m][n]);
		}
	}
	return temp_v;
}
double policy_eval() {
	double delta = 0;
	for (int i = 0; i <= max_car_capcity; i++) {
		for (int j = 0; j <= max_car_capcity; j++) {
			double v = V[i][j];
			int a = Action[i][j];
			V[i][j] = cal_Action(i, j, a);
			delta = max(delta, fabs(V[i][j] - v));
		}
	}
	return delta;
}
int action_greedy(int i, int j) {
	int best_action = 0;
	double best_value = 0;
	for (int a = -max_move_num; a <= max_move_num; a++) {
		if (a > i)
			continue;
		else if (-a > j) {
			continue;
		}
		double val = cal_Action(i, j, a);
		if (val > best_value) {//更换策略的基本条件
			best_value = val;
			best_action = a;
		}
	}
	return best_action;
}
bool policy_improve() {
	bool stable_flag = true;
	for (int i = 0; i <= 20; i++) {
		for (int j = 0; j <= 20; j++) {
			int act_best = action_greedy(i, j);
			if (act_best != Action[i][j]) {  //新的最优的动作与旧动作不相同
				Action[i][j] = act_best;
				stable_flag = false;
			}
		}
	}
	return stable_flag;
}
void print(double s[][21]) {
	for (int i = 0; i <= 20; i++) {
		for (int j = 0; j <= 20; j++) {
			cout << s[i][j] << ' ';
		}
		cout << endl;
	}
}
int main() {
	memset(tp, 0, sizeof tp);
	memset(R, 0, sizeof R);
	memset(Action, 0, sizeof tp);
	memset(V, 0, sizeof R);
	for (int i = 0; i <= 20; i++) {
		transfer_prob(0, i);
		transfer_prob(1, i);
	}
	while (true) {
		//cout << "评测策略中...." << endl;
		while (true) {
			double delta = policy_eval();
			if (delta < accuracy)
				break;
		}
		bool stable = policy_improve();
		if (!stable) {
			//print(V);
		}
		else {
			break;
		}
	}
    print(V);
	system("pause");
}