#include"lib.h"
#include <pthread.h>
#define Size 9
#define limitDep 9
using namespace std; 
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
int nodetotal = 0;
fstream file1,LogCheckerboard,lll;
							 
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
	LogCheckerboard<<"    ";
	for(int i=0;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(i==0){
				cout<<j<<"  ";	
				LogCheckerboard<<j<<"  ";	
			}
			else
				if(checkerboard[i][j]==0){
					cout<<" - ";	
					LogCheckerboard<<" - ";	
				}
				else if(checkerboard[i][j]==1){
					cout<<" O ";
					LogCheckerboard<<" O ";	
				}
				else{
					cout<<" X ";
					LogCheckerboard<<" X ";					
				}
		}
		if(i!=8){
			cout<<"\n\n "<<i+1<<" ";
			LogCheckerboard<<"\n\n "<<i+1<<" ";			
		}
	}
	cout<<"\n O : "<<black<<"   X : "<<white;
	LogCheckerboard<<"\n O : "<<black<<"   X : "<<white;
	if(player){
		cout<<"\t->player(O)\tAI(X)"<<endl;
		LogCheckerboard<<"\t->player(O)\tAI(X)"<<endl;		
	}
	else{
		cout<<"\tplayer(O)\t->AI(X)"<<endl;
		LogCheckerboard<<"\tplayer(O)\t->AI(X)"<<endl;		
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

void playerChess(int checkerboard[][Size]){
	Node node(0,0);
	if(!checkCanPlayChess(checkerboard,player,sn,en)){
		player = 0;
		cout<<"無可下位置，換AI";
		_sleep(0.5*1000);
		return ;
	}
	while(1){
		cout<<"\nrow(列) : "; 
		cin>>node.row;
		cout<<"col(行) : ";
		cin>>node.col;
//		node.row=1;node.col=1;
		if(node.row<1||node.row>8||node.col<1||node.col>8)
			continue;
		if(checkerboard[node.row][node.col]==0){
			bool Invalid = updateCheckerboard(checkerboard,node,black,white,player);
			if(!Invalid){
				cout<<"輸入錯誤\n";
				_sleep(0.5*1000);
			}
			else{
				LogCheckerboard<<endl<<"player : ( "<<node.row<<","<<node.col<<" )"<<endl;
				updateNode(checkerboard,sn,en,node.row,node.col);
				player = 0;
				return;	
			}
		}
		else{
			cout<<"輸入錯誤\n";
			_sleep(0.5*1000);
		}
	}
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
//			pthread_mutex_lock(&count_mutex);	
			for(int i=0;i<arr.size();i++){
				if(*(int*)&arr[i].v>m)
					m=*(int*)&arr[i].v;
			}
//			pthread_mutex_unlock(&count_mutex);			
		}

		if(par.depth==limitDep-(testDep+1))
			pthread_exit((void*)m);
		else
			return (void*)m;
		
	}
}

Node ans(0,0);
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
		nodetotal+=arr.size();
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
				_sleep(125);
			}
			for(int i=0;i<arr.size();i++){
				pthread_join(thread[i],&arr[i].v);
				if(par.depth!=limitDep)
					*(int*)&arr[i].v*=-1;
//				else
//					cout<<"threadID : "<<thread[i]<<" row :"<<arr[i].row<<" col : "<<arr[i].col<<" score : "<<*(int*)&arr[i].v<<endl;
				if(*(int*)&arr[i].v>m){
					m=*(int*)&arr[i].v;
					if(par.depth==limitDep)
						ans=arr[i];	
				}
				
			}
		}
		pthread_exit((void*)m);
	}
}
void AIChess(int checkerboard[][Size]){	
	nodetotal = 0;
	if(!checkCanPlayChess(checkerboard,player,sn,en)){
		player = 1;
		cout<<"無可下位置，換玩家";
		_sleep(0.5*1000);
		return ;
	}
	parameter par(checkerboard,black,white,0,limitDep,sn,en);
	pthread_t id;
	START = clock();
	pthread_create(&id,NULL,minimax_thread,(void*)&par);
	pthread_join(id,NULL);
	//minimax(&par);
	END = clock();
	file1<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<endl;
//	cout<<"Ans Row : "<<ans.row<<" Col : "<<ans.col<<endl;
//	_sleep(500000);
	updateCheckerboard(checkerboard,ans,black,white,player);
	updateNode(checkerboard,sn,en,ans.row,ans.col);
	player = 1;
	LogCheckerboard<<endl<<"AI : ( "<<ans.row<<","<<ans.col<<" )"<<endl;
}

int main(){
	file1.open("./test/"+to_string(limitDep-1)+"/Minimax.csv",ios::out);
	file1<<"depth "+to_string(limitDep-1)+" :"<<endl;
	file1<<"Time,Node"<<endl;
	LogCheckerboard.open("./test/"+to_string(limitDep-1)+"/board.txt",ios::out);
//	lll.open("./test/"+to_string(limitDep-1)+"/lll.txt",ios::out);
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));
	checkerboard[4][4]=checkerboard[5][5]=1; checkerboard[4][5]=checkerboard[5][4]=-1;
	checkerboard[4][0]=checkerboard[5][0]=checkerboard[0][4]=checkerboard[0][5]=2;
	printCheckerboard(checkerboard);
	
//	checkerboard[3][1]=checkerboard[3][2]=checkerboard[3][3]=checkerboard[3][7]=checkerboard[4][1]=1;
//	checkerboard[1][6]=checkerboard[2][2]=checkerboard[2][4]=checkerboard[2][5]=checkerboard[2][6]=checkerboard[3][4]=checkerboard[3][5]=checkerboard[3][6]=checkerboard[4][2]=checkerboard[4][3]=checkerboard[4][4]=checkerboard[4][5]=checkerboard[4][6]=checkerboard[4][7]=checkerboard[4][8]=checkerboard[5][2]=checkerboard[5][4]=checkerboard[5][5]=checkerboard[5][6]=-1;
//	checkerboard[1][0]=1;
//	checkerboard[2][0]=4;
//	checkerboard[3][0]=7;
//	checkerboard[4][0]=8;
//	checkerboard[5][0]=4;
//	checkerboard[0][1]=2;
//	checkerboard[0][2]=4;
//	checkerboard[0][3]=2;
//	checkerboard[0][4]=4;
//	checkerboard[0][5]=4;
//	checkerboard[0][6]=5;
//	checkerboard[0][7]=2;
//	checkerboard[0][8]=1;
//	black=5;white=19;
//	sn.row=1;
//	sn.col=1;
//	en.row=6;
//	en.col=8;
//	printCheckerboard(checkerboard);
//	for(int i=0;i<10;i++){
//		if(player)
//			playerChess(checkerboard);
//		else{
//			AIChess(checkerboard);
//			memset (checkerboard,0,sizeof(checkerboard));
//			checkerboard[3][1]=checkerboard[3][2]=checkerboard[3][3]=checkerboard[3][7]=checkerboard[4][1]=1;
//			checkerboard[1][6]=checkerboard[2][2]=checkerboard[2][4]=checkerboard[2][5]=checkerboard[2][6]=checkerboard[3][4]=checkerboard[3][5]=checkerboard[3][6]=checkerboard[4][2]=checkerboard[4][3]=checkerboard[4][4]=checkerboard[4][5]=checkerboard[4][6]=checkerboard[4][7]=checkerboard[4][8]=checkerboard[5][2]=checkerboard[5][4]=checkerboard[5][5]=checkerboard[5][6]=-1;
//			checkerboard[1][0]=1;
//			checkerboard[2][0]=4;
//			checkerboard[3][0]=7;
//			checkerboard[4][0]=8;
//			checkerboard[5][0]=4;
//			checkerboard[0][1]=2;
//			checkerboard[0][2]=4;
//			checkerboard[0][3]=2;
//			checkerboard[0][4]=4;
//			checkerboard[0][5]=4;
//			checkerboard[0][6]=5;
//			checkerboard[0][7]=2;
//			checkerboard[0][8]=1;
//			black=5;white=19;
//			sn.row=1;
//			sn.col=1;
//			en.row=6;
//			en.col=8;				
//		}
//		printCheckerboard(checkerboard);
//		if(isGameOver(checkerboard,black,white,sn,en)){
//			break;
//		}
//	}
	while(1){
		if(player)
			playerChess(checkerboard);
		else
			AIChess(checkerboard);	
		printCheckerboard(checkerboard);
		if(isGameOver(checkerboard,black,white,sn,en)){
			break;
		}
	}
	if(black>white)
		cout<<"player is win";
	else if(white>black)
		cout<<"AI is win";
	else if(white==black)
		cout<<"Tie";
		
	lll.close();
	
	file1.close();
	LogCheckerboard.close();
}

