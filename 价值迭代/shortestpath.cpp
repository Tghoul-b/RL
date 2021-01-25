#include<iostream>
#include<algorithm>
#include<cstring>
#include<math.h>
using namespace std;
const int N=4;
double V[N][N];
double tmpV[N][N];
int dx[4]={0,0,1,-1};
int dy[4]={-1,1,0,0};
int Action[N][N];
double account=1.0;
double accuracy=1e-4;
pair<int,int> Reward(int x,int y,int i){
      int n_x=x+dx[i];
      int n_y=y+dy[i];
      int next_state=0;
      int reward=-1;
      next_state=n_x*N+n_y;
      if(n_x<0||n_x>=N||n_y<0||n_y>=N)  {
          next_state=x*N+y;
          reward=-1;
      }
      return {next_state,reward};
}
void eval_iteration(){
      
      int cnt=0;
      while(true){
          double delta=0;
          for(int x=0;x<N;x++){
              for(int y=0;y<N;y++){
                  double v=V[x][y];
                  double tmp=-1e18;
                  int best_ac=-1;
                  for(int k=0;k<4;k++){
                      pair<int,int> res=Reward(x,y,k);
                      int n_x=res.first/N,n_y=res.first%N;
                      int R=res.second;
                      double t=(R+account*V[n_x][n_y]);
                      if(t>tmp){
                          tmp=t;
                          best_ac=k;
                      }
                  }
                if(x==0&&y==0)
                    tmp=0;
                  V[x][y]=tmp;
                  Action[x][y]=best_ac;
                  delta=max(delta,fabs(tmp-v));
              }
          }
          if(delta<accuracy){
              for(int i=0;i<N;i++){
                  for(int j=0;j<N;j++)
                     cout<<V[i][j]<<' ';
                  cout<<endl;
              }
              break;
          }
      }
}
int main(){
    eval_iteration();
}