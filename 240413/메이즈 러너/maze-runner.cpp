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
int start_x, start_y, square_size;
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
void makeSquare(){
    for(int len=2; len<=N; len++){
        for(int x1=1; x1< N-len+2; x1++){
            for(int y1=1; y1<N-len+2; y1++){
                int x2=x1+len-1, y2=y1+len-1;
                if(!(x1<=goal_x && goal_x<=x2&&y1<=goal_y&&goal_y<=y2)) continue;
                for(auto runner : runners){
                    if(!runner.live) continue;
                    if(x1<=runner.x&&runner.x<=x2&&y1<=runner.y&&runner.y<=y2){
                        start_x=x1;
                        start_y=y1;
                        square_size=len;
                        return;
                    }
                }
            }
        }
    }
}
void rotateMaze(){
    makeSquare();

    int tmpMaze[MAX_N][MAX_N];
    for(int i=1; i<=N; i++){
        for(int j=1; j<=N; j++){
            tmpMaze[i][j]=Maze[i][j];
        }
    }

    int ox, oy, nx, ny;
    //내구도 깎아야함.
    for(int i=start_x; i<start_x+square_size; i++){
        for(int j=start_y; j<start_y+square_size; j++){
            ox = i-start_x, oy=j-start_y;
            nx=oy;
            ny=square_size-1-ox;
            nx+=start_x;
            ny+=start_y;
            Maze[nx][ny]=tmpMaze[i][j];
            if(Maze[nx][ny]){
                Maze[nx][ny]--;
            }
        }
    }
    // runner, exit의 위치도 움직여야함.
    //runner & exit move
    ox = goal_x-start_x;
    oy = goal_y-start_y;
    nx = oy;
    ny = square_size -1 -ox;
    nx+=start_x;
    ny+=start_y;
    goal_x = nx;
    goal_y = ny;
    for(auto & runner : runners){
        if(!runner.live) continue;
        if(start_x<=runner.x&&runner.x<start_x+square_size &&start_y<=runner.y&&runner.y<start_y+square_size){
            ox = runner.x-start_x;
            oy = runner.y-start_y;
            nx = oy;
            ny = square_size -1 -ox;
            nx+=start_x;
            ny+=start_y;
            runner.x = nx;
            runner.y = ny;
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