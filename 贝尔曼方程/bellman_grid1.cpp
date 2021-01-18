#include<iostream>
#include<cstring>
#include<algorithm>
#include<math.h>
using namespace std;
const int N = 5;
int dx[4] = { -1,1,0,0 };
int dy[4] = { 0,0,-1,1 };
int A = 0 * N + 1;
int A_prime = 4 * N + 1;
int B = 0 * N + 3;
int B_prime = 2 * N + 3;
double action_prob = 0.25;//动作的概率为等概率
double account = 0.9;//折扣因子
double theta = 1e-4;//截止的条件
pair<int, int> reward(int state, int action) {   //返回下一个状态和动作产生的奖励
	if (state == A) {
		return { A_prime,10 };//如果当前状态是A
	}
	if (state == B) {
		return { B_prime,5 };//如果当前状态是B
	}
	int x = state / N, y = state % N;//得到该状态所在的行号和列号
	int next_state;
	int reward = 0;
	int n_x = x + dx[action], n_y = y + dy[action];
	if (n_x < 0 || n_x >= N || n_y < 0 || n_y >= N) {//出界了
		next_state = state;
		reward = -1;
	}
	else {
		next_state = n_x * N + n_y;
	}
	return { next_state,reward };
}
void print(double state[][N]){
    		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << state[i][j] << ' ';
			}
			cout << endl;
		}
}
void bellman_equation() {
	double state[N][N];//
	memset(state, 0, sizeof state);
	double new_state[N][N];//用来保存递推得到的值
	while (true) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				new_state[i][j] = 0;//清空该数组
			}
		}
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					for (int k = 0; k < 4; k++)//对应的四种动作
					{
						pair<int, int> res = reward(i*N + j, k);
						int next_x = res.first / N, next_y = res.first%N;
						int r = res.second;
						new_state[i][j] += action_prob * (r + account * state[next_x][next_y]);
					}
				}
			}
		double sum = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				sum += fabs(state[i][j] - new_state[i][j]);
			}
		}
		if (sum < theta) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					cout << state[i][j] << ' ';
				}
				cout << endl;
			}
			return;
		}

		memcpy(state, new_state, sizeof state);
    }
}
void bellman_optimal_equation(){
    double state[N][N];//
	memset(state, 0, sizeof state);
	double new_state[N][N];//用来保存递推得到的值
	while (true) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				new_state[i][j] = 0;//清空该数组
			}
		}
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					for (int k = 0; k < 4; k++)//对应的四种动作
					{
						pair<int, int> res = reward(i*N + j, k);
						int next_x = res.first / N, next_y = res.first%N;
						int r = res.second;
						new_state[i][j] = max(new_state[i][j], (r + account * state[next_x][next_y]));
					}
				}
			}
		double sum = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				sum += fabs(state[i][j] - new_state[i][j]);
			}
		}
		if (sum < theta) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					cout << state[i][j] << ' ';
				}
				cout << endl;
			}
			return;
		}

		memcpy(state, new_state, sizeof state);
    }
}
int main() {
	bellman_equation();
    bellman_optimal_equation();
	system("pause");
}