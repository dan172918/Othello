#include"lib.h"
#include <pthread.h>
#define Size 9
#define limitDep 9
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
int nodetotal = 0;
fstream file1,LogCheckerboard;

struct Node{
	int row,col;
	Node(int r,int c):row(r),col(c){}
};
 
struct parameter{
	int checkerboard[Size][Size],black,white,player,depth;
};

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

void playerChess(int checkerboard[][Size]){
	if(!checkCanPlayChess(checkerboard,player)){
		player = 0;
		cout<<"無可下位置，換AI";
		_sleep(0.5*1000);
		return ;
	}
	int row,col;
	bool Invalid;
	while(1){
		cout<<"\nrow(列) : "; 
		cin>>row;
		cout<<"col(行) : ";
		cin>>col;
		if(row<1||row>8||col<1||col>8)
			continue;
		while(1){
			if(checkerboard[row][col]==0){
				Invalid = updateCheckerboard(checkerboard,row,col,black,white,player);
				if(!Invalid){
					cout<<"輸入錯誤\n";
					_sleep(0.5*1000);
					break;
				}
				else{
					LogCheckerboard<<endl<<"player : ( "<<row<<","<<col<<" )"<<endl;
					player = 0;
					return;	
				}
			}
			else{
				cout<<"輸入錯誤\n";
				_sleep(0.5*1000);
				break;
			}	
		}
	}
}

bool isGameOver(int checkerboard[][Size],int black,int white){
	if(black+white==64||white==0||black==0||!(checkCanPlayChess(checkerboard,0)||checkCanPlayChess(checkerboard,1)))
		return true;
	else
		return false;
}

vector<Node> findnode(int checkerboard[][Size],int player){
	vector<Node> arr;
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
						arr.push_back(Node(i,j));
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
	return arr;
}

int ansR,ansC;
//int Minimax(int checkerboard[][Size],int black,int white,int player,int depth){
//	nodetotal++;
//	int m;
//	if(depth==0||isGameOver(checkerboard,black,white)){
//		if(player)
//			return white-black;
//		else
//			return black-white;
//	}
//	else{
//		vector<Node> arr = findnode(checkerboard,player);
//		if(arr.size()==0)
//			m=-1*Minimax(checkerboard,black,white,!player,depth);
//		else{
//			m=INT_MIN;
//			for(int i=0;i<arr.size();i++){
//				int temp[Size][Size],tempB=black,tempW=white;
//				memcpy(temp,checkerboard,sizeof(temp));
//				updateCheckerboard(temp,arr[i].row,arr[i].col,tempB,tempW,player);
//				int t=-1*Minimax(temp,tempB,tempW,!player,depth-1);
//				if(depth==limitDep)
//					t*=-1;
//				if(t>m){
//					m=t;
//					if(depth==limitDep){
//						ansR=arr[i].row;
//						ansC=arr[i].col;	
//					}
//				}
//			}	
//		}
//	}
//	return m;
//}

void* minimax(void* data){
	nodetotal++;
	parameter par = *(parameter*)data;
	void* val;
	int m=INT_MIN;
	if(par.depth==0||isGameOver(par.checkerboard,par.black,par.white)){
		if(par.player)
			if(par.depth==limitDep-1)
				pthread_exit((void*)(par.white-par.black));
			else
				return (void*)(par.white-par.black);
		else
			if(par.depth==limitDep-1)
				pthread_exit((void*)(par.black-par.white));	
			else
				return (void*)(par.black-par.white);	
	}
	else{
		vector<Node> arr = findnode(par.checkerboard,par.player);
		pthread_t thread[arr.size()];
		if(arr.size()==0){
			par.player=!(par.player);
			pthread_create(&thread[0],NULL,minimax,(void*)&par);
			pthread_join(thread[0],&val);
			m=-1*(*((int*)&val));
		}
		else{
			if(par.depth==limitDep){
				for(int i=0;i<arr.size();i++){
					parameter temp=par;
					updateCheckerboard(temp.checkerboard,arr[i].row,arr[i].col,temp.black,temp.white,par.player);
					temp.player=!(par.player);
					temp.depth--;
					pthread_create(&thread[i],NULL,minimax,(void*)&temp);
					_sleep(1);
				}
				for(int i=0;i<arr.size();i++){
					pthread_join(thread[i],&val);
					int t=(*((int*)&val));
					if(t>m){
						m=t;
						ansR=arr[i].row;
						ansC=arr[i].col;	
					}
				}
			}
			else{
				for(int i=0;i<arr.size();i++){
					parameter temp=par;
					updateCheckerboard(temp.checkerboard,arr[i].row,arr[i].col,temp.black,temp.white,par.player);
					temp.player=!(par.player);
					temp.depth--;
					val = minimax(&temp);
					int t=-1*(*((int*)&val));
					if(t>m)
						m=t;
				}	
			}	
		}
	}
	if(par.depth==limitDep-1)
		pthread_exit((void*)m);
	else
		return (void*)m;
}

void AIChess(int checkerboard[][Size]){	
	nodetotal = 0;
	if(!checkCanPlayChess(checkerboard,player)){
		player = 1;
		cout<<"無可下位置，換玩家";
		_sleep(0.5*1000);
		return ;
	}
	parameter par;
	memcpy(par.checkerboard,checkerboard,sizeof(par.checkerboard));
	par.black=black;par.white=white;
	par.player = 0;
	par.depth=limitDep;
	
//	pthread_t rootThread;
	START = clock();
//	pthread_create(&rootThread,NULL,minimax,(void*)&par);
//	pthread_join(rootThread,NULL);
	minimax(&par);
	END = clock();

	file1<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<endl;
	updateCheckerboard(checkerboard,ansR,ansC,black,white,player);
	player = 1;
	LogCheckerboard<<endl<<"AI : ( "<<ansR<<","<<ansC<<" )"<<endl;
}



int main(){
	file1.open("./test/6/1Minimax.csv",ios::out);
	file1<<"depth 6 :"<<endl;
	file1<<"Time,Node"<<endl;
	file1<<endl;
	LogCheckerboard.open("./test/6/board.txt",ios::out);
	
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));
	checkerboard[4][4]=checkerboard[5][5]=1; checkerboard[4][5]=checkerboard[5][4]=-1;
	printCheckerboard(checkerboard);
	while(1){
		if(player)
			playerChess(checkerboard);
		else
			AIChess(checkerboard);	
		printCheckerboard(checkerboard);
		if(isGameOver(checkerboard,black,white)){
			break;
		}
	}
	if(white==0||(black+white==64&&black>white))
		cout<<"player is win";
	else if(black==0||(black+white==64&&white>black))
		cout<<"AI is win";
	else if(black+white==64&&white==black)
		cout<<"Tie";
	file1.close();
	LogCheckerboard.close();
}

