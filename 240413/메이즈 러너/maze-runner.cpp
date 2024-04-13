#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
#define up 0
#define down 1
#define left 2
#define right 3
int dx[4] = {-1,1,0,0};
int dy[4] = {0,0,-1,1};
using namespace std;
#define MAX_N 11
int N, M, K, goal_x, goal_y, sum_of_movement;
int Maze[MAX_N][MAX_N];

struct Runner{
    int x, y;
    bool live;
};
vector<Runner> runners;
void init(){
    cin>>N>>M>>K;
    for(int i=1; i<=N; i++){
        for(int j=1; j<=N; j++){
            cin>>Maze[i][j];
        }
    }
    for(int i=0; i<M; i++){
        int x, y; cin>>x>>y;
        runners.push_back({x,y,true});
    }
    cin>>goal_x>>goal_y;
    sum_of_movement=0;
}
int getDist(int x1, int y1, int x2, int y2){
    return abs(x1-x2)+abs(y1-y2);
}
void view(){
    for(int i=1; i<=N; i++){
        for(int j=1; j<=N; j++){
            cout<<Maze[i][j]<<" ";
        }
        cout<<endl;
    }
   
    cout<<"exit: "<<goal_x<<", "<<goal_y<<endl;
    cout<<endl;
}
bool inMaze(int x, int y){
    return x>=1&&x<=N&&y>=1&&y<=N;
}
void rotateMaze(){
    //start 위치 구하기.
    // runner idx에 매칭되는 dist 저장 필요. row랑 col 합산이 아니라 따로 따로 길이만 있으면 됨
    vector<pair<int, pair<int,int>>> square_dist;
    int cnt=0;
    for(auto runner : runners){
        int dist = max(abs(goal_x-runner.x), abs(goal_y-runner.y));
        int dir;
        //4분류 하면 안됨................. 훨씬 많음.
        if(runner.x<=goal_x && runner.y<=goal_y) dir =0;
        else if(runner.x<=goal_x && runner.y>=goal_y) dir =1;
        else if(runner.x>=goal_x && runner.y<=goal_y) dir =2;
        else if(runner.x>=goal_x && runner.y>=goal_y) dir =3;

        square_dist.push_back({dist, {dir, cnt++}});
    }
    sort(square_dist.begin(), square_dist.end());
    int min_dist;
    int min_dir;
    for(int i=0; i<square_dist.size(); i++){
        int runner_idx = square_dist[i].second.second;
        if(runners[runner_idx].live){
            min_dist = square_dist[i].first;
            min_dir = square_dist[i].second.first;
            break;
        }
    }

    // 가능한 최소 사각형 먼저 구하기. 그걸 r값 작은 -> 같으면 c값 작은 걸로 우선순위로 정렬한 뒤에 선택
    int start_x, start_y;
    if(min_dir==0){
        start_x=goal_x-min_dist;
        start_y=goal_y-min_dist;
    }else if(min_dir==1){
        start_x=goal_x-min_dist;
        start_y=goal_y;
    }else if(min_dir==2){
        start_x=goal_x;
        start_y=goal_y-min_dist;
    }else if(min_dir==3){
        start_x=goal_x;
        start_y=goal_y;
    }
    if(start_x<1){
        start_x=1;
    }
    if(start_y<1){
        start_y=1;
    }
    if(start_x+min_dist>N){
        int tmp = start_x+min_dist -N;
        start_x-=tmp;
    }
    if(start_y+min_dist>N){
        int tmp = start_y+min_dist -N;
        start_y-=tmp;
    }
    


    //Maze move
    int tmpMap[MAX_N][MAX_N];
    for(int i=1; i<=N; i++){
        for(int j=1; j<=N; j++){
            tmpMap[i][j]=Maze[i][j];
        }
    }

    int size = min_dist+1;
    //내구도 깎아야함.
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            int tmp = tmpMap[start_x+size-1-j][start_y+i];
            if(tmp>0) tmp--;
            Maze[start_x+i][start_y+j] = tmp;
        }
    }
    // runner, exit의 위치도 움직여야함.
    //runner & exit move
    int j_ = start_x+size-1-goal_x;
    int i_ = goal_y-start_y;
    goal_x = start_x+i_;
    goal_y=start_y+j_;

    for(int i=0; i<square_dist.size(); i++){
        int runner_idx = square_dist[i].second.second;
        if(runners[runner_idx].live && min_dist == square_dist[i].first && min_dir==square_dist[i].second.first){
            j_ = start_x+size-1-runners[runner_idx].x;
            i_ = runners[runner_idx].y-start_y;
            runners[runner_idx].x = start_x+i_;
            runners[runner_idx].y=start_y+j_;

        }
    }
}
void moveRunner(){
    for(auto & runner : runners){
        if(!runner.live) continue;
        int cur_dist = getDist(runner.x, runner.y, goal_x, goal_y);
        for(int i=0; i<4; i++){
            int nx=runner.x+dx[i], ny=runner.y+dy[i];
            if(!inMaze(nx,ny) || Maze[nx][ny]) continue;

            int nxt_dist = getDist(nx, ny, goal_x, goal_y);
            if(nxt_dist < cur_dist){
                //move
                runner.x=nx;
                runner.y=ny;
                sum_of_movement++;
                if(nxt_dist==0) runner.live=false;
                break;
            }
        }
    }
}
void solve(){
    for(int k=0; k<K; k++){
        moveRunner();
        rotateMaze();
    }
    cout<<sum_of_movement<<endl;
    cout<<goal_x<<" "<<goal_y;
}

int main(){
    cin.tie(0);cout.tie(0);ios::sync_with_stdio(0);
    //freopen("test_input.txt", "r", stdin);
    
    init();
    solve();

    return 0;
}