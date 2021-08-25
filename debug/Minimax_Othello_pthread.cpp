#include"lib.h"
#include <pthread.h>
#define Size 9
#define limitDep 11
using namespace std; 
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
long long int nodetotal = 0;
fstream file1,LogCheckerboard;
							 
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

Node sn(3,3),en(6,6); 
void updateNode(int checkerboard[][Size],Node &sn,Node &en,int row,int col){
	if(sn.row==row){
		if(sn.row!=1&&checkerboard[row-1][0]!=8)
			sn.row--;
		while(checkerboard[sn.row][0]==8)
			sn.row++;
	}	
	else if(en.row==row){
		if(en.row!=8&&checkerboard[row+1][0]!=8)
			en.row++;
		while(checkerboard[en.row][0]==8)
			en.row--;
	}
	if(sn.col==col){
		if(sn.col!=1&&checkerboard[0][col-1]!=8)
			sn.col--;
		while(checkerboard[0][sn.col]==8)
			sn.col++;
	}	
	else if(en.col==col){
		if(en.col!=8&&checkerboard[0][col+1]!=8)
			en.col++;
		while(checkerboard[0][en.col]==8)
			en.col--;
	}
}

void printCheckerboard(int checkerboard[][Size]){
	system("cls");
	cout<<"    ";
	for(int i=0;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(i==0){
				cout<<j<<"  ";		
			}
			else
				if(checkerboard[i][j]==0){
					cout<<" - ";	
				}
				else if(checkerboard[i][j]==1){
					cout<<" O ";
				}
				else{
					cout<<" X ";				
				}
		}
		if(i!=8){
			cout<<"\n\n "<<i+1<<" ";		
		}
	}
	cout<<"\n O : "<<black<<"   X : "<<white;
	if(player){
		cout<<"\t->player(O)\tAI(X)"<<endl;	
	}
	else{
		cout<<"\tplayer(O)\t->AI(X)"<<endl;		
	}
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
	if(black+white==64||white==0||black==0||!(checkCanPlayChess(checkerboard,0,sn,en)||checkCanPlayChess(checkerboard,1,sn,en)))
		return true;
	else
		return false;
}

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
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
int testDep=1;
void* minimax(void* data){
	parameter par = *(parameter*)data;
	if(par.depth==0||isGameOver(par.checkerboard,par.black,par.white,par.sn,par.en)){
		if(par.player)
			if(par.depth==limitDep-(testDep+1))
				pthread_exit((void*)(par.white-par.black));
			else
				return (void*)(par.white-par.black);
		else
			if(par.depth==limitDep-(testDep+1))
				pthread_exit((void*)(par.black-par.white));	
			else
				return (void*)(par.black-par.white);	
	}
	else{
		vector<Node> arr = findnode(par.checkerboard,par.player,par.sn,par.en);
		pthread_mutex_lock(&count_mutex);
		nodetotal+=arr.size();
		pthread_mutex_unlock(&count_mutex);
		void* val;int m=INT_MIN;
		if(arr.size()==0){
			par.player=!(par.player);
			val = minimax(&par);
			m=-1*(*(int*)&val);
		}
		else{
			for(int i=0;i<arr.size();i++){
				parameter temp=par;
				updateCheckerboard(temp.checkerboard,arr[i],temp.black,temp.white,temp.player);
				updateNode(temp.checkerboard,temp.sn,temp.en,arr[i].row,arr[i].col);
				temp.player=!(temp.player);
				temp.depth--;
				arr[i].v = minimax(&temp);
				*(int*)&arr[i].v*=-1;
			}	
			for(int i=0;i<arr.size();i++){
				if(*(int*)&arr[i].v>m)
					m=*(int*)&arr[i].v;
			}		
		}

		if(par.depth==limitDep-(testDep+1))
			pthread_exit((void*)m);
		else
			return (void*)m;
		
	}
}

void* minimax_thread(void* data){
	parameter par = *(parameter*)data;
	if(par.depth==0||isGameOver(par.checkerboard,par.black,par.white,par.sn,par.en)){
		if(par.player)
			pthread_exit((void*)(par.white-par.black));
		else
			pthread_exit((void*)(par.black-par.white));	
	}
	else{
		vector<Node> arr = findnode(par.checkerboard,par.player,par.sn,par.en);
		pthread_mutex_lock(&count_mutex);
		nodetotal+=arr.size();
		pthread_mutex_unlock(&count_mutex);
		void* value; int m=INT_MIN;
		if(arr.size()==0){
			par.player=!par.player;
			pthread_t continueID;
			pthread_create(&continueID,NULL,minimax_thread,(void*)&par);
			pthread_join(continueID,&value);
			m=-1*(*(int*)value);
		}
		else{
			pthread_t thread[arr.size()];
			for(int i=0;i<arr.size();i++){
				parameter temp=par;
				updateCheckerboard(temp.checkerboard,arr[i],temp.black,temp.white,temp.player);
				updateNode(temp.checkerboard,temp.sn,temp.en,arr[i].row,arr[i].col);
				temp.player=!temp.player;
				temp.depth--;
				if(par.depth==limitDep-testDep)
					pthread_create(&thread[i],NULL,minimax,(void*)&temp);
				else
					pthread_create(&thread[i],NULL,minimax_thread,(void*)&temp);
				_sleep(150);
			}
			for(int i=0;i<arr.size();i++){
				pthread_join(thread[i],&arr[i].v);
				if(par.depth!=limitDep)
					*(int*)&arr[i].v*=-1;
				if(*(int*)&arr[i].v>m){
					m=*(int*)&arr[i].v;
				}
				
			}
		}
		pthread_exit((void*)m);
	}
}
void AIChess(int checkerboard[][Size]){	
	nodetotal = 0;
	parameter par(checkerboard,black,white,player,limitDep,sn,en);
	pthread_t id;
	START = clock();
	pthread_create(&id,NULL,minimax_thread,(void*)&par);
	pthread_join(id,NULL);
	END = clock();
	LogCheckerboard<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<endl;
}

void updateInitialNode(int checkerboard[][Size]){
	for(int i=1;i<Size;i++){
		if(checkerboard[i][0]==8)
			continue;
		if(checkerboard[i][0]!=0){
			sn.row=(i!=1)?((checkerboard[i-1][0]!=8)?i-1:i):i;
		break;
		}
	}
	for(int i=Size-1;i>0;i--){
		if(checkerboard[i][0]==8)
			continue;
		if(checkerboard[i][0]!=0){
			en.row=(i!=Size-1)?((checkerboard[i+1][0]!=8)?i+1:i):i;
		break;
		}
	}
	
	for(int i=1;i<Size;i++){
		if(checkerboard[0][i]==8)
			continue;
		if(checkerboard[0][i]!=0){
			sn.col=(i!=1)?((checkerboard[0][i-1]!=8)?i-1:i):i;
		break;
		}
	}
	for(int i=Size-1;i>0;i--){
		if(checkerboard[0][i]==8)
			continue;
		if(checkerboard[0][i]!=0){
			en.col=(i!=Size-1)?((checkerboard[0][i+1]!=8)?i+1:i):i;
		break;
		}
	}
}

int main(){
	LogCheckerboard.open("./Minimax10.csv",ios::out);
	LogCheckerboard<<"depth 10 :,,"<<endl;
	LogCheckerboard<<"Time,Node"<<endl;
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));
	string str;
	for(int i=1;i<=15;i++){
		black=0;white=0;
		memset (checkerboard,0,sizeof(checkerboard));
		file1.open("./othello-initial board-400/"+to_string(i)+".txt",ios::in);
		file1>>str;
		player = (str=="o")?1:0;
		for(int j=1;j<Size;j++){
			file1>>str;
			for(int k=1;k<Size;k++){
				if(str[k-1]!='_'){
					checkerboard[j][0]++;
					checkerboard[0][k]++;
					if(str[k-1]=='o'){
						checkerboard[j][k]=1;
						black++;
					}
					else{
						checkerboard[j][k]=-1;
						white++;
					}
				}
			}
		}
		file1.close();
		updateInitialNode(checkerboard);
//		printCheckerboard(checkerboard);
//		cout<<sn.row<<" "<<sn.col<<endl;
//		cout<<en.row<<" "<<en.col<<endl;
//		_sleep(500000);
		AIChess(checkerboard);
//		LogCheckerboard<<endl;
	}
	LogCheckerboard.close();
}

