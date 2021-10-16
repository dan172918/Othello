#include"lib.h"
#include <pthread.h>
#define Size 4
#define limitDep 5
using namespace std; 
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
int nodetotal = 0;
fstream LogCheckerboard;
							 
struct Node{
	int row,col,dir;
	void* v;
	Node():row(-1),col(-1),dir(0){}
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
};
 
struct parameter{
	int checkerboard[Size][Size],black,white,player,depth;
	Node sn,en;
	parameter(int temp[][Size],int b,int w,int p,int d,Node S,Node E){
		memcpy(checkerboard,temp,sizeof(checkerboard));
		black=b;
		white=w;
		player=p;
		depth=d;
		sn=S;
		en=E;
	}
};

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

void printCheckerboard(parameter par){
	LogCheckerboard<<"    ";
	for(int i=0;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(i==0){
				LogCheckerboard<<par.checkerboard[i][j]<<"  ";				
			}
			else{
				if(par.checkerboard[i][j]==0)
					LogCheckerboard<<" - ";	
				else if(par.checkerboard[i][j]==1)
					LogCheckerboard<<" O ";	
				else
					LogCheckerboard<<" X ";				
			}
		}
		if(i!=3){
			LogCheckerboard<<"\n\n "<<par.checkerboard[i+1][0]<<" ";			
		}
	}
	LogCheckerboard<<endl;
}

bool updateCheckerboard(int checkerboard[][Size],Node node,int &black,int &white,int player){
	int initBlack = black, initwhite = white;
	for(int i=node.dir;i<8;i++){
		int xStart=node.row+dir[i][0],yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||checkerboard[xStart][yStart]==0||(player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1))
			continue;
		bool findplayerColor=false;
		xStart+=dir[i][0];
		yStart+=dir[i][1];
		while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&checkerboard[xStart][yStart]!=0){
			if((player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1)){
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
		checkerboard[node.row][node.col]=1;
		black++;
	}
	else{
		checkerboard[node.row][node.col]=-1;
		white++;
	}
	checkerboard[node.row][0]++;
	checkerboard[0][node.col]++;
	return true;
}

bool checkCanPlayChess(int checkerboard[][Size],int player,Node sn,Node en){
	for(int i=sn.row;i<=en.row;i++){
		for(int j=sn.col;j<=en.col;j++){
			if(checkerboard[i][j])
				continue;
			for(int k=0;k<8;k++){
				int xStart=i+dir[k][0],yStart=j+dir[k][1];
				if(xStart==0||yStart==0||checkerboard[xStart][yStart]==0||(player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1))
					continue;
				xStart+=dir[k][0];
				yStart+=dir[k][1];
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&checkerboard[xStart][yStart]!=0){
					if((player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1))
						return true;
					xStart+=dir[k][0];
					yStart+=dir[k][1];
				}
			}
		}		
	}
	return false;
}

bool isGameOver(int checkerboard[][Size],int black,int white,Node sn,Node en){
	if(black+white==Size*Size||white==0||black==0||!(checkCanPlayChess(checkerboard,0,sn,en)||checkCanPlayChess(checkerboard,1,sn,en)))
		return true;
	else
		return false;
}

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
Node ans(0,0);
vector<Node> findnode(int checkerboard[][Size],int player,Node sn,Node en){
	vector<Node> arr,f,s;
	for(int i=sn.row;i<=en.row;i++){
		for(int j=sn.col;j<=en.col;j++){
			bool find = false; 
			if(checkerboard[i][j])
				continue;
			for(int k=0;k<8;k++){
				int xStart=i+dir[k][0],yStart=j+dir[k][1];
				if(xStart==0||yStart==0||checkerboard[xStart][yStart]==0||(player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1))
					continue;
				xStart+=dir[k][0];
				yStart+=dir[k][1];
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&checkerboard[xStart][yStart]!=0){
					if((player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1)){
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

int testDep=2;
void* minimax(void* data){
	parameter par = *(parameter*)data;
	if(par.depth==0||isGameOver(par.checkerboard,par.black,par.white,par.sn,par.en)){
		if(par.depth==limitDep-(testDep+1))
			pthread_exit((void*)((par.player)?par.white-par.black:par.black-par.white));
		else
			return (void*)((par.player)?par.white-par.black:par.black-par.white);
	}
	else{
		vector<Node> arr = findnode(par.checkerboard,par.player,par.sn,par.en);
		void* val;int m=INT_MIN;
		bool samePlayer=false;
		if(arr.size()==0){
			par.player=!(par.player);
			samePlayer=true;
			arr = findnode(par.checkerboard,par.player,par.sn,par.en);
		}
		for(int i=0;i<arr.size();i++){
			parameter temp=par;
			updateCheckerboard(temp.checkerboard,arr[i],temp.black,temp.white,temp.player);
			updateNode(temp.checkerboard,temp.sn,temp.en,arr[i].row,arr[i].col);
			temp.player=!(temp.player);
			temp.depth--;
			arr[i].v = minimax((void*)&temp);
			*(int*)&arr[i].v*=-1;
			if(samePlayer)
				*(int*)&arr[i].v*=-1;
//			pthread_mutex_lock(&count_mutex);	
//			LogCheckerboard<<"depth : "<<par.depth<<endl;
//			LogCheckerboard<<"row :"<<arr[i].row<<" col : "<<arr[i].col<<endl;
//			LogCheckerboard<<"player : "<<par.player<<endl;
//			LogCheckerboard<<"score : "<<*(int*)&arr[i].v<<endl<<endl;
//			pthread_mutex_unlock(&count_mutex);
			if(*(int*)&arr[i].v>m)
				m=*(int*)&arr[i].v;
		}
		if(par.depth==limitDep-(testDep+1))
			pthread_exit((void*)m);
		else
			return (void*)m;
	}
}

void* minimax_thread(void* data){
	parameter par = *(parameter*)data;
	if(par.depth==0||isGameOver(par.checkerboard,par.black,par.white,par.sn,par.en))
		pthread_exit((void*)((par.player)?par.white-par.black:par.black-par.white));
	else{
		vector<Node> arr = findnode(par.checkerboard,par.player,par.sn,par.en);
		void* value; int m=INT_MIN;
		bool samePlayer=false;
		if(arr.size()==0){
			par.player=!(par.player);
			samePlayer=true;
			arr = findnode(par.checkerboard,par.player,par.sn,par.en);
		}
		pthread_t thread[arr.size()];
		for(int i=0;i<arr.size();i++){
			parameter temp=par;
			updateCheckerboard(temp.checkerboard,arr[i],temp.black,temp.white,temp.player);
			updateNode(temp.checkerboard,temp.sn,temp.en,arr[i].row,arr[i].col);
			temp.player=!temp.player;
			temp.depth--;
			pthread_create(&thread[i],NULL,(par.depth==limitDep-testDep)?minimax:minimax_thread,(void*)&temp);
			_sleep(150);
		}
		for(int i=0;i<arr.size();i++){
			pthread_join(thread[i],&arr[i].v);
			if(!samePlayer&&par.depth!=limitDep)
				*(int*)&arr[i].v*=-1;
			pthread_mutex_lock(&count_mutex);	
			LogCheckerboard<<"depth : "<<par.depth<<endl;
			LogCheckerboard<<"row :"<<arr[i].row<<" col : "<<arr[i].col<<endl;
			LogCheckerboard<<"player : "<<par.player<<endl;
			LogCheckerboard<<"score : "<<*(int*)&arr[i].v<<endl<<endl;
			pthread_mutex_unlock(&count_mutex);
			if(*(int*)&arr[i].v>m){
				m=*(int*)&arr[i].v;
				if(par.depth==limitDep){
					ans=arr[i];
				}
			}
		}
		pthread_exit((void*)m);
	}
}
void AIChess(int checkerboard[][Size]){	
	parameter par(checkerboard,black,white,0,limitDep,sn,en);
	pthread_t id;
	pthread_create(&id,NULL,minimax_thread,(void*)&par);
	void* value;
	pthread_join(id,&value);
//	pthread_join(id,NULL);
	cout<<"end"<<endl;
	cout<<*(int*)&value<<endl;
	cout<<ans.row<<" "<<ans.col<<endl;
}

int main(){
	LogCheckerboard.open("./DEBUG-pthread.txt",ios::out);
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));

	checkerboard[1][1]=checkerboard[2][2]=1; checkerboard[1][2]=checkerboard[2][1]=-1;
	checkerboard[1][0]=checkerboard[2][0]=checkerboard[0][1]=checkerboard[0][2]=2;

	AIChess(checkerboard);
	LogCheckerboard.close();
}

