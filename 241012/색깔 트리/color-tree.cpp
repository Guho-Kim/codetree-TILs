#include<iostream>
#include<algorithm>
#include<map>
#include<vector>
using namespace std;

typedef struct Node{
	int color;
	int p_id;
	int max_depth;
	vector<int> c_id;
}Node;

Node nodePool[20005];
int nodeCnt=0;
map<int, Node*> Nodes;
vector<int> root_id;

bool isAddNode(int p_id){
	int depth=1;
	while(p_id!=-1){
		Node* pNode = Nodes[p_id];
		int p_max_depth = pNode->max_depth;
		if(p_max_depth>depth){
			depth++;
			p_id=pNode->p_id;
		}else{
			return false;
		}
	}
	return true;
}

void addNode(int m_id, int p_id, int color, int max_depth){
	// impossible for max_depth issue
	if(p_id!=-1 && !isAddNode(p_id)){
		return;
	}

	// setNode
	nodePool[nodeCnt].p_id=p_id;
	nodePool[nodeCnt].color=color;
	nodePool[nodeCnt].max_depth=max_depth;
	Nodes[m_id]=&nodePool[nodeCnt];
	nodeCnt++;

	// setChild
	if(p_id==-1){
		root_id.push_back(m_id);
		return;
	}
	Nodes[p_id]->c_id.push_back(m_id);
	
}

void changeColor(int m_id, int color){
	Nodes[m_id]->color=color;
	for(int c_id : Nodes[m_id]->c_id){
		changeColor(c_id, color);
	}
}

int getColor(int m_id){
	return Nodes[m_id]->color;
}
int	calculateScore(Node* node, int* colorCount){
	colorCount[node->color]=1;
	int sum = 0;
	for(int c_id : node->c_id){
		int childColorCount[6]={0};
		int score = calculateScore(Nodes[c_id], childColorCount);

		for(int i=1; i<=5; i++){
			colorCount[i]+=childColorCount[i];
		}

		sum+=score;
	}

	int count =0;
	for (int i = 1; i <= 5; i++){
		if(colorCount[i]>0) count++;
	}

	sum += (count * count);
	return sum;

}
int getScore(){
	int score = 0;
	for(auto root : root_id){
		int colorCount[6]={0};
		score += calculateScore(Nodes[root], colorCount);
	}

	return score;
}
// void viewTree(int m_id){
// 	Node* node = Nodes[m_id];
// 	cout<<m_id<<":\n";
// 	cout<<"("<<node->p_id<<", "<<node->color<<", "<<node->max_depth<<")\n";
// 	for(auto c_id : node->c_id){
// 		viewTree(c_id);
// 	}
// }

int main(){
	ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	// freopen("input.txt", "r", stdin);
	int Q, cmd, m_id, p_id, color, max_depth;
	cin>>Q;
	for(int i=0; i<Q; i++){
		cin>>cmd;
		if(cmd==100){
			cin>>m_id>>p_id>>color>>max_depth;
			addNode(m_id, p_id, color, max_depth);
		}else if(cmd==200){
			cin>>m_id>>color;
			changeColor(m_id, color);
		}else if(cmd==300){
			cin>>m_id;
			cout<<getColor(m_id)<<'\n';
		}else if(cmd==400){
			cout<<getScore()<<'\n';
		}
	}

	return 0;
}