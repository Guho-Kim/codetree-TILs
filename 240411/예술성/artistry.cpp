#include<iostream>
#include<algorithm>
#include<queue>
#include<cstring>

using namespace std;
#define MAX_N 30
int N;
int MAP[MAX_N][MAX_N];
int groupMAP[MAX_N][MAX_N];

//groupCnt[group_num] = {other_group_num, cnt}
int groupCnt[900][900];
int group_val[900];
bool visited[MAX_N][MAX_N];
int group_cnt;
vector<int> group_num_cnt;
int score;
int total_score;
int tmpMAP[MAX_N][MAX_N];
/*
    1. 계산 구현하는 함수
        1. 그룹화 시키기 bfs or dfs (visited로 관리)
            - 그룹 점수 sum, 개수 cnt
        2. 그룹끼리 인접한 변 cnt


    2. rotation하는 함수


    3. debug
        view map
*/
void init(){
    score=0;
    group_cnt=0;
    memset(groupMAP, 0, sizeof(groupMAP));
    memset(groupCnt, 0, sizeof(groupCnt));
    memset(group_val, 0, sizeof(group_val));
    group_num_cnt.clear();
}

void viewMAP(){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            cout << MAP[i][j] << " ";
        }
        cout << endl;
    }
    cout<<endl;
}

void viewGroupMAP(){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            cout << groupMAP[i][j] << " ";
        }
        cout << endl;
    }
    cout<<endl;
}

bool inMAP(int x, int y){
    return x >= 0 && y >= 0 && x < N && y < N;
}

int dx[4] = {0, 0, -1, 1};
int dy[4] = {1, -1, 0, 0};

void countContGroup(pair<int,int> pos, int group_num){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            visited[i][j] = false;
        }
    }
    queue<pair<int, int>> q;
    q.push(pos);

    visited[pos.first][pos.second] = true;
    int cnt = 0;

    while(!q.empty()){
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        for(int i=0; i<4; i++){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(!inMAP(nx,ny)) continue;
            if(visited[nx][ny]) continue;
            if(groupMAP[nx][ny] != group_num){
                groupCnt[group_num][groupMAP[nx][ny]]++;
                groupCnt[groupMAP[nx][ny]][group_num]++;
                continue;
            }
            visited[nx][ny]=true;
            q.push({nx, ny});
        }
    }
}
void bfs(pair<int,int> pos){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            visited[i][j] = false;
        }
    }
    queue<pair<int, int>> q;
    q.push(pos);
    visited[pos.first][pos.second] = true;
    int val = MAP[pos.first][pos.second];
    group_cnt++;
    int cnt = 1;
    groupMAP[pos.first][pos.second] = group_cnt;

    while(!q.empty()){
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        for(int i=0; i<4; i++){
            int nx = x + dx[i];
            int ny = y + dy[i];

            if(!inMAP(nx,ny)) continue;
            if(visited[nx][ny]) continue;
            if(MAP[nx][ny] != val){
                // 인접한 변을 카운트 할 수 있는 기회. 그런데 숫자가 mix되어있으면, 잘못하면 엇갈린 카운트도 가능?

                continue;

            }
            
            groupMAP[nx][ny] = group_cnt;
            visited[nx][ny] = true;
            cnt++;
            q.push({nx, ny});
        }
    }
    group_num_cnt.push_back(cnt);
    group_val[group_cnt] = val;
}

void ratate_square(int x, int y){
    int mid = N/2;

    for(int i=0; i<mid; i++){
        for(int j=0; j<mid; j++){
            MAP[x + j][y + i] = tmpMAP[x + mid - i - 1][y + j];
        }
    }
}

void rotation(){
    
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            tmpMAP[i][j] = MAP[i][j];
        }
    }
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            //센터 시계 반대 방향
            if(i==N/2 || j == N/2)
                MAP[i][j] = tmpMAP[j][N-i-1];
        }
    }
    ratate_square(0,0);
    ratate_square(0,N/2+1);
    ratate_square(N/2+1,0);
    ratate_square(N/2+1,N/2+1);

}


void solve(){
    total_score=0;

    int cnt=0;
    while(1){
        cnt++;

        init();
        //viewMAP();
        //set grouping
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                if(!groupMAP[i][j]){
                    bfs({i,j});
                }
            }
        }
        //viewGroupMAP();
        
        bool visitedGroup[900] = {false,};
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                if(!visitedGroup[groupMAP[i][j]]){
                    visitedGroup[groupMAP[i][j]]=true;
                    countContGroup({i,j}, groupMAP[i][j]);
                }
            }
        }

        for(int i=1; i<group_cnt; i++){
            for(int j=i+1; j<=group_cnt; j++){
                groupCnt[i][j]/=2;
                if(groupCnt[i][j]){
                    int tmp=(group_num_cnt[i-1]+group_num_cnt[j-1])*group_val[i]*group_val[j]*groupCnt[i][j];
                    score += tmp;
                }
            }
        }
        total_score+=score;
        if(cnt==4) break;
        rotation();
    }
    cout<<total_score<<endl;

}

int main() {
    cin.tie(0);cout.tie(0);ios::sync_with_stdio(0);
    cin>>N;
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            cin >> MAP[i][j];
        }
    }
    solve();

    return 0;
}