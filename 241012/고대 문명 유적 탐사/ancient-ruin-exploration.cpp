#include<iostream>
#include<algorithm>
#include<queue>
#include<vector>
using namespace std;

int M_list[400]={0};
int K, M;
int mCnt=0;
int tmp_mCnt=0;
int graph[5][5];
int rotatedGraph[5][5];
int maxGraph[5][5];
int score[3][3][3];
int dr[4]={-1,0,0,1};
int dc[4]={0,-1,1,0};
int bfsCount[8];

void viewMap(){
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			cout<< rotatedGraph[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

bool isInGraph(int x, int y){
	if(x<0 || x>=5 || y<0 || y>=5) return false;
	return true;
}
int getValue(){
	// viewMap();
	int value = 0;
	bool isVisited[5][5]={false};
	vector<pair<int,int>> trace;
	queue<pair<int, int>> q;

	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			if(isVisited[i][j]) continue;

			q.push(make_pair(i, j));
			trace.push_back(make_pair(i, j));
			isVisited[i][j]=true;
			while(!q.empty()){
				auto cur = q.front();
				q.pop();
				// isVisited[cur.first][cur.second]=true;
				for(int d=0; d<4; d++){
					int nr = cur.first+dr[d];
					int nc = cur.second+dc[d];
					if(isInGraph(nr, nc) && (rotatedGraph[nr][nc]==rotatedGraph[cur.first][cur.second]) && rotatedGraph[cur.first][cur.second] !=0 && !isVisited[nr][nc]){
						trace.push_back(make_pair(nr, nc));
						q.push(make_pair(nr,nc));
						isVisited[nr][nc]=true;
					}
				}
			}
			if(trace.size()>=3){
				value += trace.size();
				// cout<<value<<endl;
				// viewMap();
				for(auto t : trace){
					// cout<<"trace: ";
					// cout<<t.first<<", "<<t.second<<endl;
					// cout<<rotatedGraph[t.first][t.second]<<endl;
					rotatedGraph[t.first][t.second]=0;
				}
			}
			trace.clear();

		}
	}
	// cout<<"value: "<<value<<endl;
	return value;

}
void rotate(int degree, int r, int c){
	//copy
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			rotatedGraph[i][j]=graph[i][j];
		}
	}
	// viewMap();

	int start_x=r;
	int start_y=c;
	int squre_size=3;
	int ox, oy, nx, ny;
	for(int i=start_x; i<start_x+squre_size; i++){
		for(int j=start_y; j<start_y+squre_size; j++){
			ox=i-start_x;
			oy=j-start_y;
			// 90 degree rotate
			if(degree==90){
				nx=oy;
				ny=squre_size-1-ox;
			}
			// 180 degree rotate
			else if(degree==180){
				nx = squre_size -1 -ox;
				ny = squre_size -1 -oy;
			}
			// 270 degree rotate
			else{
				nx = squre_size -1 -oy;
				ny = ox;
			}
			nx+=start_x;
			ny+=start_y;
			// cout<<i<<j<<nx<<ny<<endl;
			rotatedGraph[nx][ny]=graph[i][j];
		}
	}
	// viewMap();
	
}
void setMaxGraph(){
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			maxGraph[i][j]=rotatedGraph[i][j];
		}
	}
}

void Fill(){
	// viewMap();
	for(int j=0; j<5; j++){
		for(int i=4; i>=0; i--){
			if(mCnt < M && rotatedGraph[i][j] == 0){
				// cout<<"m_list: "<<M_list[mCnt]<<endl;
				rotatedGraph[i][j] = M_list[mCnt++];
			}
		}
	}
	// viewMap();
}
int getFillScore(){
	int sum=0;
	while(1){
		Fill();
		int val = getValue();
		if(val==0){
			break;
		}
		sum+=val;
		
	}
	return sum;
}
void solve(){
	for(int k=0; k<K; k++){
		int sum=0;

		int maxVal=0;
		for(int degree=90; degree<=270; degree+=90){
			for(int j=0; j<3; j++){
				for(int i=0; i<3; i++){
					// tmp_mCnt=mCnt;
					rotate(degree, i, j);
					int value=getValue();
					if(value>maxVal){
						// viewMap();
						// cout<<value<<" ";
						// cout<<i+1<<", "<<j+1<<" "<<degree<<endl;
						maxVal=value; 
						// mCnt=tmp_mCnt;
						setMaxGraph();
					}

				}
			}
		}
		if(maxVal==0) break;

		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){
				rotatedGraph[i][j]=maxGraph[i][j];
			}
		}


		int val = getFillScore();
		maxVal+=val;
		// graph <- maxgraph
		if(maxVal>0)
			cout<<maxVal<<" ";



		for(int i=0; i<5; i++){
			for(int j=0; j<5; j++){
				graph[i][j]=rotatedGraph[i][j];
			}
		}
	}
}
int main(){
	ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	// freopen("input.txt", "r", stdin);

	cin>>K>>M;
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			cin>>graph[i][j];
		}
	}
	for(int k=0; k<M; k++){
		cin>>M_list[k];
	}
	solve();


	return 0;
}