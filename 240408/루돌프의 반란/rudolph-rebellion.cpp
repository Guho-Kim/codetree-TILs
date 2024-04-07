#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#define MAX_N 55
#define MAX_P 35
using namespace std;

struct Santa
{
    pair<int, int> pos;
    int score;
    int stun;
    bool life;
};
int N, M, P, C, D;
vector<Santa> santa(MAX_P);
pair<int, int> deer;

bool compare(const pair<int, int> &a, const pair<int, int> &b)
{
    if (a.first == b.first)
    {
        if (santa[a.second].pos.first == santa[b.second].pos.first)
            return santa[a.second].pos.second > santa[b.second].pos.second;
        return santa[a.second].pos.first > santa[b.second].pos.first;
    }
    return a.first < b.first;
}
int MAP[MAX_N][MAX_N];

int getDistance(pair<int, int> a, pair<int, int> b)
{
    return pow(a.first - b.first, 2) + pow(a.second - b.second, 2);
}
bool inMAP(pair<int, int> a){
    int r = a.first;
    int c = a.second;
    return r>=1 && r<=N && c>=1 && c<=N;
}
int santaDx[4]={-1,0,1,0};
int santaDy[4]={0,1,0,-1};

void init()
{
    for (int i = 0; i < MAX_N; i++)
    {
        for (int j = 0; j < MAX_N; j++)
        {
            MAP[i][j] = 0;
        }
    }
    for (int i = 0; i < MAX_P; i++)
    {
        santa[i].pos = make_pair(0, 0);
        santa[i].score = 0;
        santa[i].stun = -1;
        santa[i].life = false;
    }
    cin >> N >> M >> P >> C >> D;
    cin >> deer.first >> deer.second;
    MAP[deer.first][deer.second] = -1;
    for (int i = 0; i < P; i++)
    {
        int p;
        cin >> p;
        cin >> santa[p].pos.first >> santa[p].pos.second;
        MAP[santa[p].pos.first][santa[p].pos.second] = p;
        santa[p].life = true;
    }
}
void view(int k){
    cout<<"round "<<k<<endl;
    for(int i=1; i<=N; i++){
        for(int j=1; j<=N; j++){
            cout << MAP[i][j] << " ";
        }
        cout << endl;
    }
    cout<<endl;
    for(int i=1; i<=P; i++){
        cout<<"santa "<<i<<" : ";
        cout <<"("<< santa[i].pos.first << "," << santa[i].pos.second << ")";
        cout<<"score: "<<santa[i].score << ", life : " << santa[i].life << " " << santa[i].stun << endl;

    }
    cout<<endl;
}
void solve(int K)
{
    int move_to_r;
    int move_to_c;
    // view(0);
    for(int k=1; k<=K; k++){
        vector<pair<int, int>> dist;
        for (int i = 1; i <= P; i++)
        {
            if(santa[i].life==false) continue;
            dist.push_back(make_pair(getDistance(deer, santa[i].pos), i));
        }
        if(dist.size()==0) break;

        sort(dist.begin(), dist.end(), compare);

        int targetSantaIdx = dist[0].second;
        Santa *targetSanta = &santa[targetSantaIdx];

        //deer move
        move_to_r=0;
        move_to_c=0;
        if (deer.first < targetSanta->pos.first){
            move_to_r++;
        }else if (deer.first > targetSanta->pos.first){
            move_to_r--;
        }if (deer.second < targetSanta->pos.second){
            move_to_c++;
        }else if (deer.second > targetSanta->pos.second){
            move_to_c--;
        }

        MAP[deer.first][deer.second]=0;

        deer.first+=move_to_r;
        deer.second+=move_to_c;

        // deer -> santa 충돌
        if(deer == targetSanta->pos){
            targetSanta->stun=k+1;
            targetSanta->score+=C;

            //targetSanta out from map
            MAP[targetSanta->pos.first][targetSanta->pos.second]=0;

            //targetSanta index change
            targetSanta->pos.first += move_to_r*C;
            targetSanta->pos.second += move_to_c*C;

            // check 2. 상호작용
            int nextSanta;
            while(1){
                // check 1. 경계
                if(!inMAP(targetSanta->pos)){
                    targetSanta->life = false;
                    MAP[targetSanta->pos.first][targetSanta->pos.second]=0;
                    break;
                }
                // nextSanta out from map
                nextSanta = MAP[targetSanta->pos.first][targetSanta->pos.second];

                // targetSanta in map
                MAP[targetSanta->pos.first][targetSanta->pos.second]=targetSantaIdx;

                // just move and done
                if(nextSanta == 0){
                    break;
                }

                // else, nextSanta move
                santa[nextSanta].pos.first+=move_to_r;
                santa[nextSanta].pos.second+=move_to_c;
                targetSanta = &santa[nextSanta];
                targetSantaIdx = nextSanta;
            }
        }

        MAP[deer.first][deer.second]=-1;
        
        // cout<<"after deer move "<<endl;view(k);


        // 산타 움직임
        for(int i=1; i<=P; i++){
            if(santa[i].life==false) continue;
            if(santa[i].stun >= k) continue;


            // dist 최소가 되는 곳으로 이동할 건데, 조회하는 순서만 있는 것임
            int curr_dist = getDistance(deer, santa[i].pos);
            int minDist = curr_dist;
            int n_r, n_c;
            n_r=n_c=0;
            for(int j=0; j<4; j++){
                int next_r = santa[i].pos.first + santaDx[j];
                int next_c = santa[i].pos.second + santaDy[j];
                if(!inMAP(make_pair(next_r, next_c))) continue;
                
                int next_dist = getDistance(deer, make_pair(next_r, next_c));
                if(MAP[next_r][next_c]<=0 && next_dist < minDist){
                    minDist = next_dist;
                    n_r = next_r;
                    n_c = next_c;
                }
            }
            // 갈 곳이 없네,,
            if(minDist == curr_dist){
                continue;
            }
            move_to_r=santa[i].pos.first - n_r;
            move_to_c=santa[i].pos.second - n_c;

            // santa move
            MAP[santa[i].pos.first][santa[i].pos.second]=0;
            santa[i].pos.first = n_r;
            santa[i].pos.second = n_c;
            MAP[santa[i].pos.first][santa[i].pos.second]=i;
        

            // santa -> deer 충돌
            if(deer == santa[i].pos){
                santa[i].score+=D;
                santa[i].stun=k+1;
                targetSanta = &santa[i];
                targetSantaIdx = i;


                //cp
                MAP[targetSanta->pos.first][targetSanta->pos.second]=0;

                targetSanta->pos.first += move_to_r*D;
                targetSanta->pos.second += move_to_c*D;
                
                int nextSanta;
                while(1){
                    // check 1. 경계
                    if(!inMAP(targetSanta->pos)){
                        targetSanta->life = false;
                        MAP[targetSanta->pos.first][targetSanta->pos.second]=0;
                        break;
                    }
                    // nextSanta out from map
                    nextSanta = MAP[targetSanta->pos.first][targetSanta->pos.second];

                    // targetSanta in map
                    MAP[targetSanta->pos.first][targetSanta->pos.second]=targetSantaIdx;

                    // just move and done
                    if(nextSanta == 0){
                        break;
                    }

                    // else, nextSanta move
                    santa[nextSanta].pos.first+=move_to_r;
                    santa[nextSanta].pos.second+=move_to_c;
                    targetSanta = &santa[nextSanta];
                    targetSantaIdx = nextSanta;
                }
            }
            MAP[deer.first][deer.second]=-1;


        }

        // 살아남은 산타 +1점!
        for (int i = 1; i <= P; i++)
        {
            if(santa[i].life==false) continue;
            santa[i].score++;
        }
        // cout<<"after santa move"<<endl;view(k);
    }
    for(int i=1; i<=P; i++){
        cout<<santa[i].score<<" ";
    }
}

int main()
{
    cin.tie(0);
    cout.tie(0);
    ios::sync_with_stdio(0);
    init();
    solve(M);
    return 0;
}