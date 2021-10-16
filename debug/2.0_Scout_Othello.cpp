#include"lib.h"
#define Size 4
#define limitDep 6
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
int nodetotal = 0;
fstream file1,LogCheckerboard;

struct Node{
	int row,col,dir;
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
};

int Max(int a,int b){
	return (a>b)?a:b;
}

Node sn(1,1),en(3,3); 
void updateNode(int checkerboard[][Size],Node &sn,Node &en,int row,int col){
	if(sn.row==row){
		if(sn.row!=1&&checkerboard[row-1][0]!=Size-1)
			sn.row--;
		while(checkerboard[sn.row][0]==Size-1)
			sn.row++;
	}	
	else if(en.row==row){
		if(en.row!=Size-1&&checkerboard[row+1][0]!=Size-1)
			en.row++;
		while(checkerboard[en.row][0]==Size-1)
			en.row--;
	}
	if(sn.col==col){
		if(sn.col!=1&&checkerboard[0][col-1]!=Size-1)
			sn.col--;
		while(checkerboard[0][sn.col]==Size-1)
			sn.col++;
	}	
	else if(en.col==col){
		if(en.col!=Size-1&&checkerboard[0][col+1]!=Size-1)
			en.col++;
		while(checkerboard[0][en.col]==Size-1)
			en.col--;
	}
}

void updateCheckerboard(int checkerboard[][Size],Node node,int &black,int &white,int player){
	int xStart,yStart;
	for(int i=node.dir;i<8;i++){
		xStart=node.row+dir[i][0];yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size||(player&&checkerboard[xStart][yStart]==-2)||(!player&&checkerboard[xStart][yStart]==-1))
			continue;
		if(checkerboard[xStart][yStart]>=0){
			checkerboard[xStart][yStart]=(checkerboard[xStart][yStart]>(i+4)%8)?(i+4)%8:checkerboard[xStart][yStart];
			continue;
		}
		bool findplayerColor=false;
		xStart+=dir[i][0];
		yStart+=dir[i][1];
		while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&checkerboard[xStart][yStart]<0){
			if((player&&checkerboard[xStart][yStart]==-2)||(!player&&checkerboard[xStart][yStart]==-1)){
				findplayerColor=true;
				break;
			}
			xStart+=dir[i][0];
			yStart+=dir[i][1];
		}
		while(findplayerColor){
			xStart-=dir[i][0];
			yStart-=dir[i][1];
			if(xStart==node.row&&yStart==node.col)
				break;
			if(player&&checkerboard[xStart][yStart]==-1){
				white--;
				black++;
				checkerboard[xStart][yStart]=-2;
			}
			else if(!player&&checkerboard[xStart][yStart]==-2){
				white++;
				black--;
				checkerboard[xStart][yStart]=-1;
			}
		}

	}
	if(player){
		checkerboard[node.row][node.col]=-2;
		black++;
	}
	else{
		checkerboard[node.row][node.col]=-1;
		white++;
	}
	checkerboard[node.row][0]++;
	checkerboard[0][node.col]++;
	for(int i=0;i<node.dir;i++){
		xStart=node.row+dir[i][0];yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size)
			continue;
		if(checkerboard[xStart][yStart]>=0)
			checkerboard[xStart][yStart]=(checkerboard[xStart][yStart]>(i+4)%8)?(i+4)%8:checkerboard[xStart][yStart];		
	}
}

vector<Node> findnode(int checkerboard[][Size],int player,Node sn,Node en){
	vector<Node> arr;
	for(int i=sn.row;i<=en.row;i++){
		for(int j=sn.col;j<=en.col;j++){
			bool find = false;
			if(checkerboard[i][j]<0||checkerboard[i][j]==8)
				continue;
			for(int k=checkerboard[i][j];k<8;k++){
				int xStart=i+dir[k][0],yStart=j+dir[k][1];
				if(xStart==0||yStart==0||xStart==Size||yStart==Size||checkerboard[xStart][yStart]>=0||(player&&checkerboard[xStart][yStart]==-2)||(!player&&checkerboard[xStart][yStart]==-1))
					continue;
				xStart+=dir[k][0];
				yStart+=dir[k][1];
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&checkerboard[xStart][yStart]<0){
					if((player&&checkerboard[xStart][yStart]==-2)||(!player&&checkerboard[xStart][yStart]==-1)){
						arr.push_back(Node(i,j,k));
						find = true;
						break;
					}
					xStart+=dir[k][0];
					yStart+=dir[k][1];
				}
				if(find)
					break;
			}
		}		
	}
	return arr;
}

Node ans(0,0);
int Scout(int checkerboard[][Size],int alpha,int beta,int black,int white,int player,int depth,Node sn,Node en){
	if(depth==0||black+white==9||white==0||black==0){
		return (player)?black-white:white-black;
	}
	else{
		vector<Node> arr = findnode(checkerboard,player,sn,en);
		bool samePlayer=false;
		int m=INT_MIN,n=beta;
		if(arr.size()==0){
			samePlayer=true;
			player=!player;
			swap(alpha,beta);
			alpha*=-1;beta*=-1;
			arr = findnode(checkerboard,player,sn,en);
			if(arr.size()==0)
				return (!player)?black-white:white-black;
		}
		for(int i=0;i<arr.size();i++){
			int temp[Size][Size],tempB=black,tempW=white;
			Node tempSn=sn,tempEn=en;
			memcpy(temp,checkerboard,sizeof(temp));
			updateCheckerboard(temp,arr[i],tempB,tempW,player);
			updateNode(temp,tempSn,tempEn,arr[i].row,arr[i].col);
			int	t=-1*Scout(temp,-1*n,-1*Max(m,alpha),tempB,tempW,!player,depth-1,tempSn,tempEn);
			if(t>m){
				if(n==beta||depth<3||t>=beta)
					m=t;
				else{
					m=-1*Scout(temp,-1*beta,-1*t,tempB,tempW,!player,depth-1,tempSn,tempEn);					
				}
				if(depth==limitDep){
					ans=arr[i];	
				}

			}
			if(m>=beta){
				if(samePlayer)
					m*=-1;
				return m;
			}
			n=max(alpha,m)+1;
		}
		if(samePlayer)
			m*=-1;
		return m;
	}
}

void AIChess(int checkerboard[][Size]){
	int temp[Size][Size];
	memcpy(temp,checkerboard,sizeof(temp));
	int m=Scout(temp,-1000,1000,black,white,0,limitDep,sn,en);
	LogCheckerboard<<"ans : "<<endl;	
	LogCheckerboard<<"depth : "<<limitDep<<endl;
	LogCheckerboard<<"row :"<<ans.row<<" col : "<<ans.col<<endl;
	LogCheckerboard<<"player : "<<player<<endl;
	LogCheckerboard<<"score : "<<m<<endl<<endl;
}

int main(){
	LogCheckerboard.open("./DEBUG_Scout.txt",ios::out);
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));
	for(int i=1;i<Size;i++){
		for(int j=1;j<Size;j++){
			checkerboard[i][j]=8;
		}
	}
	checkerboard[1][1]=checkerboard[2][2]=-2; checkerboard[1][2]=checkerboard[2][1]=-1;
	checkerboard[1][0]=checkerboard[2][0]=checkerboard[0][1]=checkerboard[0][2]=2;
	for(int i=1;i<=2;i++){
		for(int j=1;j<=2;j++){
			for(int k=0;k<8;k++){
				if(checkerboard[i+dir[k][0]][j+dir[k][1]]>=0)
					checkerboard[i+dir[k][0]][j+dir[k][1]]=(checkerboard[i+dir[k][0]][j+dir[k][1]]>(k+4)%8)?(k+4)%8:checkerboard[i+dir[k][0]][j+dir[k][1]];
			}
		}
	}
	AIChess(checkerboard);	
	LogCheckerboard.close();

}

