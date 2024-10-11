#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#define north 0
#define east 1
#define south 2
#define west 3

using namespace std;

long long ret=0;
int R, C, K;
typedef struct Monster{
	int r;
	int c;
	int d;
}Monster;
Monster monsters[1005];

int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1};
int startM, endM;
int map[80][80]={0,};
bool isExit[80][80]={false,};
bool isInForest(int r, int c){
	if(r<1 || r>R || c<1 || c>C) return false;
	return true;
}
bool goDown(Monster *monster){
	int r=monster->r;
	int c=monster->c;

	if(r < R-1 && (map[r+1][c-1]==0) && (map[r+2][c]==0) && (map[r+1][c+1]==0)){
		monster->r=r+1;
		return true;
	}
	return false;
}

bool isMoveWestAndSouth(Monster *monster){
	int c = monster->c;
	int r = monster->r;
	if(c<3 || r+2>R) return false;

	if(isInForest(r+2,c-2) && map[r-1][c-1]==0 && map[r][c-2]==0 && map[r+1][c-2]==0 && map[r+1][c-1]==0 && map[r+2][c-1]==0){
		monster->r=r+1;
		monster->c=c-1;
		monster->d = (monster->d+3)%4;
		return true;
	}else{
		return false;
	}
}
bool isMoveEastAndSouth(Monster *monster){
	int c = monster->c;
	int r = monster->r;
	if(c+2>C || r+2>R) return false;

	if(isInForest(r+2,c+2) && map[r-1][c+1]==0 && map[r][c+2]==0 && map[r+1][c+2]==0 && map[r+1][c+1]==0 && map[r+2][c+1]==0){
		monster->r=r+1;
		monster->c=c+1;
		monster->d = (monster->d+1)%4;
		return true;
	}else{
		return false;
	}
}
int bfs(Monster *monster){
	int ret=0;
	bool isVisited[80][80]={false,};
	queue<pair<int, int> > q;
	q.push(make_pair(monster->r,monster->c));
	while(!q.empty()){
		int r = q.front().first;
		int c = q.front().second;
		q.pop();
		isVisited[r][c]=true;
		for(int i=0; i<4; i++){
			int nr = r+dr[i];
			int nc = c+dc[i];
			if(isInForest(nr, nc) && map[nr][nc]!=0 && !isVisited[nr][nc] && ((map[r][c]==map[nr][nc]) || ((map[r][c]!=map[nr][nc]) && isExit[r][c])) ){
				q.push(make_pair(nr, nc));
				ret = max(ret, nr);
			}
		}
	}
	// cout<<"store: "<<ret<<endl;
	return ret;
}
void resetMap(){
	for(int i=1; i<=R; i++){
		for(int j=1; j<=C; j++){
			map[i][j]=0;
			isExit[i][j]=false;
		}
	}
}
void showMap(){
	for(int i=1; i<=R; i++){
		for(int j=1; j<=C; j++){
			cout<<map[i][j]<<"\t";
		}
		cout<<endl;
	}
	for(int i=1; i<=R; i++){
		for(int j=1; j<=C; j++){
			if(isExit[i][j]){
				cout<<"1"<<"\t";	
			}else{
				cout<<"0"<<"\t";
			}
		}
		cout<<endl;
	}
	cout<<endl;
	cout<<endl;
}
int solve(){
	int sum=0;
	for(int i=0; i<K; i++){
		// cout<<"Monster: "<<i+1<<endl;
		while(1){
			if(goDown(&monsters[i])){
				// cout<<"goDown"<<endl;
				continue;
			}
			else if(isMoveWestAndSouth(&monsters[i])){
				// cout<<"isMoveWestAndSouth"<<endl;
				continue;
			}
			else if(isMoveEastAndSouth(&monsters[i])){
				// cout<<"isMoveEastAndSouth"<<endl;
				continue;
			}else{
				// cout<<"r: "<<monsters[i].r<<" c: "<<monsters[i].c<<" d: "<<monsters[i].d<<endl;
				break;
			}
		}
		if(monsters[i].r <=1){
			resetMap();
			continue;
		}
		//Marking Map
		for(int j=0; j<4; j++){
			int r = monsters[i].r+dr[j];
			int c = monsters[i].c+dc[j];
			map[r][c]=i+10000;
		}
		map[monsters[i].r][monsters[i].c]=i+10000;

		int d=monsters[i].d;
		isExit[monsters[i].r+dr[d]][monsters[i].c+dc[d]]=true;

		// showMap();
		sum+=bfs(&monsters[i]);

	}
	return sum;
}


int main(){
	ios_base::sync_with_stdio(false);
	cin >> R >> C >> K;
	for(int i=0; i<K; i++){
		monsters[i].r = -2;
		cin >> monsters[i].c >> monsters[i].d;
	}
	for(int i=0; i<=R; i++){
		for(int j=0; j<=C; j++){
			map[i][j]=0;
		}
	}
	cout<<solve();

	return 0;
}