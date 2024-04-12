#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<map>
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
priority_queue<Rabbit, vector<Rabbit>, compare> pq;
map<int, int> dist_info;
map<int, int> score_info;
int Q, N, M, P, K, S, L, cmd, total_score;

// pid - dist map으로 초기화.
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
    map<int, Rabbit> hist;
    cin>>K>>S;
    for(int k=0; k<K; k++){
        auto rabbit = pq.top();
        pq.pop();
        int d = dist_info[rabbit.pid];
        int pid = rabbit.pid;
        int jump_cnt=rabbit.jump_cnt+1;
        int x= rabbit.x;
        int y= rabbit.y;
        priority_queue<Rabbit, vector<Rabbit>, compare2> pq2;
        for(int i=0; i<4; i++){
            int nx=x;
            int ny=y;
            int dx_=dx[i];
            int dy_=dy[i];
            for(int j=0; j<d; j++){
                nx+=dx_;
                ny+=dy_;
                if(!(nx>=1&&nx<=N)){
                    dx_*=-1;
                    nx+=2*dx_;
                }
                if(!(ny>=1&&ny<=M)){
                    dy_*=-1;
                    ny+=2*dy_;
                }
            }
            pq2.push({jump_cnt, nx, ny, pid});
        }
        Rabbit nRabbit=pq2.top();
        hist[nRabbit.pid]=nRabbit;
        while(!pq2.empty()) pq2.pop();
        pq.push(nRabbit);

        total_score+=nRabbit.x+nRabbit.y;
        score_info[rabbit.pid]-=nRabbit.x+nRabbit.y;
    }
    priority_queue<Rabbit, vector<Rabbit>, compare2> pq3;
    for(auto r :hist){
        pq3.push(r.second);
    }
    int best_pid = pq3.top().pid;
    score_info[best_pid]+=S;
    while(!pq3.empty()) pq3.pop();

    
}
// pq의 우선 순위에 L은 포함되지 않아서, 단순하기 변경만 하면 됨.
void changeL(){
    int pid; cin>>pid>>L;
    dist_info[pid]*=L;   
}
void getScore(){
    int max=0;
    for(auto i : score_info){
        if(max<i.second+total_score){
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
    return 0;
}