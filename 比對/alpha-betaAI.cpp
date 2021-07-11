#include"lib.h"
#define Size 9
#define limitDep 6
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=0,white=0,player=0;
fstream file1,file2;

struct Node{
	int row,col;
	Node *next;
	Node():row(-1),col(-1),next(NULL){}
	Node(int r,int c):row(r),col(c),next(NULL){}
};

int Max(int a,int b){
	return (a>b)?a:b;
}

bool checkCanPlayChess(int checkerboard[][Size],int player){
	for(int i=1;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(checkerboard[i][j])
				continue;
			for(int k=0;k<8;k++){
				if(checkerboard[i+dir[k][0]][j+dir[k][1]]==0||(player&&checkerboard[i+dir[k][0]][j+dir[k][1]]==1)||(!player&&checkerboard[i+dir[k][0]][j+dir[k][1]]==-1))
					continue;
				int xStart=i,yStart=j;
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size){
					if((player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1))
						return true;
					xStart+=dir[k][0];
					yStart+=dir[k][1];
					if(checkerboard[xStart][yStart]==0)
						break;
				}
			}
		}		
	}
	return false;
}

bool updateCheckerboard(int checkerboard[][Size],int row,int col,int &black,int &white,int player){
	int initBlack = black, initwhite = white;
	for(int i=0;i<8;i++){
		if(checkerboard[row+dir[i][0]][col+dir[i][1]]==0||(player&&checkerboard[row+dir[i][0]][col+dir[i][1]]==1)||(!player&&checkerboard[row+dir[i][0]][col+dir[i][1]]==-1))
			continue;
		bool findplayerColor=false;
		int xStart=row,yStart=col;
		while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size){
			if((player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1)){
				findplayerColor=true;
				break;
			}
			xStart+=dir[i][0];
			yStart+=dir[i][1];
			if(checkerboard[xStart][yStart]==0)
				break;
		}
		while(findplayerColor){
			xStart-=dir[i][0];
			yStart-=dir[i][1];
			if(xStart==row&&yStart==col)
				break;
			if(player&&checkerboard[xStart][yStart]==-1){
				white--;
				black++;
				checkerboard[xStart][yStart]=1;
			}
			else if(!player&&checkerboard[xStart][yStart]==1){
				white++;
				black--;
				checkerboard[xStart][yStart]=-1;
			}

		}
	}
	if(initBlack==black&&initwhite==white)
		return false;
	if(player){
		checkerboard[row][col]=1;
		black++;
	}
	else{
		checkerboard[row][col]=-1;
		white++;
	}
	return true;
}

bool isGameOver(int checkerboard[][Size],int player,int black,int white){
	if(black+white==64||white==0||black==0||(!checkCanPlayChess(checkerboard,player)&&!checkCanPlayChess(checkerboard,!player)))
		return true;
	else
		return false;
}

vector<Node*> findnode(int checkerboard[][Size],int player){
	vector<Node*> arr;
	for(int i=1;i<Size;i++){
		for(int j=1;j<Size;j++){
			bool find = false; 
			if(checkerboard[i][j])
				continue;
			for(int k=0;k<8;k++){
				if(checkerboard[i+dir[k][0]][j+dir[k][1]]==0||(player&&checkerboard[i+dir[k][0]][j+dir[k][1]]==1)||(!player&&checkerboard[i+dir[k][0]][j+dir[k][1]]==-1))
					continue;
				int xStart=i,yStart=j;
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size){
					if((player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1)){
						Node *node = new Node(i,j);
						arr.push_back(node);
						find = true;
						break;
					}
					xStart+=dir[k][0];
					yStart+=dir[k][1];
					if(checkerboard[xStart][yStart]==0)
						break;
				}
				if(find)
					break;
			}
		}		
	}
//	for(int i=0;i<arr.size();i++){
//		cout<<arr[i]->row<<" "<<arr[i]->col<<endl;
//	}
//	_sleep(10*1000);
	return arr;
}

int AlphaBeta(int checkerboard[][Size],int alpha,int beta,int black,int white,int player,int depth,Node* &node){
	int m,t;
	if(depth==0||isGameOver(checkerboard,player,black,white)){
		if(player)
			return white-black;
		else
			return black-white;
	}
	else{
		vector<Node*> arr = findnode(checkerboard,player);
		if(arr.size()==0)
			m=-1*AlphaBeta(checkerboard,-1*beta,-1*alpha,black,white,!player,depth,node);
		else{
			m=alpha;
			for(int i=0;i<arr.size();i++){
				int temp[Size][Size],tempB=black,tempW=white;
				memcpy(temp,checkerboard,sizeof(temp));
				updateCheckerboard(temp,arr[i]->row,arr[i]->col,tempB,tempW,player);
				if(depth==limitDep)
					t=AlphaBeta(temp,-1000,1000,tempB,tempW,!player,depth-1,arr[i]->next);
				else
					t=-1*AlphaBeta(temp,-1*beta,-1*m,tempB,tempW,!player,depth-1,arr[i]->next);
				if(t>m){
					m=t;
					node=arr[i];	
				}
				if(m>=beta)
					return m;
			}
			if(depth==limitDep)
				file2<<"best move : "<<m<<endl;	
		}
	}
	return m;
}


int main(){
	string str;
	file2.open("./400s.txt",ios::out|ios::trunc);
	int checkerboard[Size][Size];
	for(int i=1;i<=400;i++){
		black=0;white=0;
		memset (checkerboard,0,sizeof(checkerboard));
		file1.open("./othello-initial board-400/"+to_string(i)+".txt",ios::in);
		file1>>str;
		player = (str=="o")?1:0;
		for(int j=1;j<Size;j++){
			file1>>str;
			for(int k=1;k<Size;k++){
				if(str[k-1]=='o'){
					checkerboard[j][k]=1;
					black++;
				}
				else if(str[k-1]=='x'){
					checkerboard[j][k]=-1;
					white++;
				}
			}
		}
		file1.close();
		Node *root = new Node();
		file2<<i<<" : "<<endl;
		AlphaBeta(checkerboard,-1000,1000,black,white,player,limitDep,root->next);
		while(1){
			root = root->next;
			file2<<"( "<<root->row<<","<<root->col<<" )";
			if(root->next!=NULL)
				file2<<" -> ";
			else
				break;	
		}
		file2<<endl<<endl;
	}

	file2.close();
}

