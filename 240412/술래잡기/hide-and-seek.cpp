#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
#define MAX_N 100
using namespace std;
#define up 0
#define right 1
#define down 2
#define left 3
int dx[4]={-1,0,1,0};
int dy[4]={0,1,0,-1};
/*
    도망자 
        d=1: 좌우로 움직임(초기 오른쪽)
        d=2: 상하로 움직임(초기 아래쪽)
        도망자 관리하기 위한 vector 필요함.
    술래
        달팽이모양으로 움직임
        초기 위치 센터
        1,1에 도달하면 방향 바꿈.
        움직임 직후에 방향 전환 후 시야 3칸 확인. 나무는 스킵. 점수 add
    총 K번의 턴이 있는데 도망자가 동시에 먼저 움직인 이후에 술래가 움직임.


    MAP은 사실 술래를 위한 것임.
*/
struct Runner{
    int x, y;
    int d;
    bool live;
};
struct Hunter{
    int x, y;
    int d;
    int movement;
    int maxMovement;
    int changeDir;
    bool outGoing;
}hunter;

vector<Runner> runners;
int N;
int M;
int H;
int K;
int score;

int tMAP[MAX_N][MAX_N];

int getDistance(int x1, int y1, int x2, int y2){
    return abs(x1-x2) + abs(y1-y2);
}
bool inMAP(int x, int y){
    return x>=1&&x<=N&&y>=1&&y<=N;
}
void init(){
    score=0;
    cin>>N>>M>>H>>K;
    memset(tMAP,0,sizeof(tMAP));
    hunter.x=N/2+1;
    hunter.y=N/2+1;
    hunter.d=up;
    hunter.movement=0;
    hunter.maxMovement=1;
    hunter.changeDir=0;
    hunter.outGoing=true;
    //도망자 위치 set
    for(int i=0; i<M; i++){
        int x, y, d; cin>>x>>y>>d;
        if(d==1)
            runners.push_back({x,y,right,true});
        else
            runners.push_back({x,y,down,true});
    }
    //나무 위치 set
    for(int i=0; i<H; i++){
        int x, y; cin>>x>>y;
        tMAP[x][y] = 1;
    }

}
int changeDir(int d){
    if(d==right) return left;
    else if(d==left) return right;
    else if(d==up) return down;
    else return up;
}

void runnerMove(){
    for(auto& runner : runners){
        if(!runner.live) continue;
        if(getDistance(hunter.x, hunter.y, runner.x, runner.y)>3) continue;
        int x=runner.x;
        int y=runner.y;
        int d=runner.d;

        int nx = x+dx[d];
        int ny = y+dy[d];

        //갈 수 없는 곳이라면 방향 전환
        if(!inMAP(nx, ny)){
            d=changeDir(d);
            runner.d=d;
            nx = x+dx[d];
            ny = y+dy[d];
        }

        //술래가 있다면 Stop
        if(hunter.x==nx && hunter.y==ny) continue;

        //러너 이동!
        runner.x=nx;
        runner.y=ny;
    }
}
void hunterMove(int k){
    int x=hunter.x;
    int y=hunter.y;
    int d=hunter.d;
    //보고 있는 방향을 전진
    int nx=x+dx[d];
    int ny=y+dy[d];
    hunter.movement++;
    hunter.x=nx;
    hunter.y=ny;

    //방향 전환 강제
    if(nx==1&&ny==1){
        hunter.d=down;
        hunter.movement=0;
        hunter.maxMovement=N-1;
        hunter.changeDir=-1;
        hunter.outGoing=false;
    }
    if(nx==N/2+1&&ny==N/2+1){
        hunter.d=up;
        hunter.movement=0;
        hunter.maxMovement=1;
        hunter.changeDir=0;
        hunter.outGoing=true;
    }

    //out going
    //방향 돌아야 하는지 체크.
    if(hunter.outGoing){
        if(hunter.movement==hunter.maxMovement){
            hunter.changeDir++;
            if(hunter.changeDir==2){
                hunter.changeDir=0;
                hunter.maxMovement++;
            }
            hunter.movement=0;
            hunter.d=(hunter.d+1)%4;
        }
    }
    else{
        //in comming
        if(hunter.movement==hunter.maxMovement){
            hunter.changeDir++;
            if(hunter.changeDir==2){
                hunter.changeDir=0;
                hunter.maxMovement--;
            }
            hunter.movement=0;
            hunter.d--;
            if(hunter.d<0) hunter.d=3;
        }
    }
    //러너 잡기.
    int dxs[3]={hunter.x, hunter.x+dx[hunter.d], hunter.x+2*dx[hunter.d]};
    int dys[3]={hunter.y, hunter.y+dy[hunter.d], hunter.y+2*dy[hunter.d]};
    int cnt=0;
    for(auto & runner :runners){
        if(!runner.live) continue;
        int r_x=runner.x;
        int r_y=runner.y;
        for(int i=0; i<3; i++){
            int x=dxs[i];
            int y=dys[i];
            if(x==r_x && y==r_y && !tMAP[x][y]){
                runner.live=false;
                cnt++;
            }
        }
    }
    score+=cnt*k;
}

void solve(){
    for(int k=1; k<=K; k++){
        runnerMove();
        hunterMove(k);
    }
    cout<<score;
}

int main(){
    cin.tie(0);cout.tie(0);ios::sync_with_stdio(0);

    init();
    solve();
    return 0;
}