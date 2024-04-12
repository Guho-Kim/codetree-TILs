#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<unordered_map>
#define left 0
#define up 1
#define right 2
#define down 3
int dx[4]={0,-1,0,1};
int dy[4]={-1,0,1,0};

using namespace std;
struct Rabbit{
    int jump_cnt;
    int x;
    int y;
    int pid;
};
struct compare{
    bool operator()(const Rabbit & a, const Rabbit & b){
        if(a.jump_cnt==b.jump_cnt){
            if(a.x+a.y == b.x+b.y){
                if(a.x == b.x){
                    if(a.y == b.y){
                        return a.pid > b.pid;
                    }else{
                        return a.y > b.y;
                    }
                }else{
                    return a.x > b.x;
                }
            }else{
                return a.x+a.y > b.x+b.y;
            }
        }else{
            return a.jump_cnt>b.jump_cnt;
        }
    }
};
struct compare2{
    bool operator()(const Rabbit & a, const Rabbit & b){
            if(a.x+a.y == b.x+b.y){
                if(a.x == b.x){
                    if(a.y == b.y){
                        return a.pid < b.pid;
                    }else{
                        return a.y < b.y;
                    }
                }else{
                    return a.x < b.x;
                }
            }else{
                return a.x+a.y < b.x+b.y;
            }
    }
};
bool compare_vec(const Rabbit a, const Rabbit b){
    if(a.x+a.y == b.x+b.y){
        if(a.x == b.x){
            if(a.y == b.y){
                return a.pid > b.pid;
            }else{
                return a.y > b.y;
            }
        }else{
            return a.x > b.x;
        }
    }else{
        return a.x+a.y > b.x+b.y;
    }
}
priority_queue<Rabbit, vector<Rabbit>, compare> pq;
unordered_map<int, int> dist_info;
unordered_map<int, int> score_info;
int Q, N, M, P, K, S, L, cmd;
long long total_score;

// pid - dist unordered_map으로 초기화.
void init(){
    dist_info.clear();
    score_info.clear();
    cin>>N>>M>>P;  
    for(int i=0; i<P; i++) {
        int pid, d; cin>>pid>>d;
        dist_info[pid]=d;
        pq.push({0, 1, 1, pid});
    }
    total_score=0;
}
void play(){
    unordered_map<int, Rabbit> hist;
    cin>>K>>S;
    for(int k=0; k<K; k++){
        auto rabbit = pq.top();
        pq.pop();
        int d = dist_info[rabbit.pid];
        int pid = rabbit.pid;
        int jump_cnt=rabbit.jump_cnt+1;
        int x= rabbit.x;
        int y= rabbit.y;
        vector<Rabbit> nextRabbit;
        for(int i=0; i<4; i++){
            int nx, ny;
            int dist_x = d%(2*N-2);
            int dist_y = d%(2*M-2);
            nx = x+dist_x*dx[i];
            ny = y+dist_y*dy[i];
            // 상하 처리
            if(nx>N){
                nx = 2*N - nx;
                if(nx<1){
                    nx=2-nx;
                }
            }else if(nx<1){
                nx=2-nx;
                if(nx>N){
                    nx=2*N-nx;
                }
            }
            // 좌우 처리
            if(ny>M){
                ny = 2*M - ny;
                if(ny<1){
                    ny=2-ny;
                }
            }else if(ny<1){
                ny=2-ny;
                if(ny>M){
                    ny=2*M-ny;
                }
            }
            nextRabbit.push_back({jump_cnt, nx, ny, pid});
        }
        sort(nextRabbit.begin(), nextRabbit.end(), compare_vec); 

        Rabbit nRabbit=nextRabbit[0];
        hist[nRabbit.pid]=nRabbit;
        pq.push(nRabbit);

        total_score+=nRabbit.x+nRabbit.y;
        score_info[rabbit.pid]-=nRabbit.x+nRabbit.y;
    }
    vector<Rabbit> bestRabbit;
    for(auto r :hist){
        bestRabbit.push_back(r.second);
    }
    sort(bestRabbit.begin(), bestRabbit.end(), compare_vec);
    int best_pid = bestRabbit[0].pid;
    score_info[best_pid]+=S;
    
}
// pq의 우선 순위에 L은 포함되지 않아서, 단순하기 변경만 하면 됨.
void changeL(){
    int pid; cin>>pid>>L;
    dist_info[pid]*=L;   
}
void getScore(){
    long long max=0;
    for(auto i : score_info){
        if(max<i.second){
            max = i.second;
        }
        // cout<<i.first<<": "<<i.second+total_score<<endl;
    }
    cout<<max+total_score;
    
    while(!pq.empty()) pq.pop();
}
void solve(){
    cin>>Q;
    for(int i=0; i<Q; i++){
        cin>>cmd;
        if(cmd==100) init();
        else if(cmd==200) play();
        else if(cmd==300) changeL();
        else getScore();
    }
}

int main(){
    cin.tie(0); cout.tie(0);ios::sync_with_stdio(0);
    //freopen("test_input.txt", "r", stdin);
    solve();
    cout<<endl;
    return 0;
}