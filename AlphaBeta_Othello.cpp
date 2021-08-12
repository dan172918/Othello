#include"lib.h"
#define Size 9
#define limitDep 9
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
int nodetotal = 0;
fstream file1,LogCheckerboard;
int PriorityArr[Size][Size]={{0,0,0,0,0,0,0,0,0},
							 {0,4,0,3,3,3,3,0,4},
							 {0,0,0,2,2,2,2,0,0},
							 {0,3,2,1,1,1,1,2,3},
							 {0,3,2,1,1,1,1,2,3},
							 {0,3,2,1,1,1,1,2,3},
							 {0,3,2,1,1,1,1,2,3},
							 {0,0,0,2,2,2,2,0,0},
							 {0,4,0,3,3,3,3,0,4}};
struct Node{
	int row,col,dir;
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
};

int Max(int a,int b){
	return (a>b)?a:b;
}

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
//						if(PriorityArr[i][j]==4)
//							f.insert(f.begin(),Node(i,j,k));
//						else if(PriorityArr[i][j]==3)
//							f.push_back(Node(i,j,k));
//						else if(PriorityArr[i][j]==2)
//							s.insert(s.begin(),Node(i,j,k));
//						else if(PriorityArr[i][j]==1)
//							s.push_back(Node(i,j,k));
//						else
//							arr.push_back(Node(i,j,k));
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
//	arr.insert(arr.begin(),s.begin(),s.end());
//	arr.insert(arr.begin(),f.begin(),f.end());
	return arr;
}

Node ans(0,0);
int AlphaBeta(int checkerboard[][Size],int alpha,int beta,int black,int white,int player,int depth,Node sn,Node en){
	nodetotal++;
	int m,t;
	if(depth==0||isGameOver(checkerboard,black,white,sn,en)){
		if(player)
			return white-black;
		else
			return black-white;
	}
	else{
		vector<Node> arr = findnode(checkerboard,player,sn,en);
		if(arr.size()==0)
			m=-1*AlphaBeta(checkerboard,-1*beta,-1*alpha,black,white,!player,depth,sn,en);
		else{
			//m=alpha;
			m=-1000;
			for(int i=0;i<arr.size();i++){
				int temp[Size][Size],tempB=black,tempW=white;
				Node tempSn=sn,tempEn=en;
				memcpy(temp,checkerboard,sizeof(temp));
				updateCheckerboard(temp,arr[i],tempB,tempW,player);
				updateNode(temp,tempSn,tempEn,arr[i].row,arr[i].col);
				if(depth==limitDep)
					t=AlphaBeta(temp,alpha,beta,tempB,tempW,!player,depth-1,tempSn,tempEn);
				else
					t=-1*AlphaBeta(temp,-1*beta,-1*Max(m,alpha),tempB,tempW,!player,depth-1,tempSn,tempEn);
				//	t=-1*AlphaBeta(temp,-1*beta,-1*m,tempB,tempW,!player,depth-1,tempSn,tempEn);
				if(t>m){
					m=t;
					if(depth==limitDep)
						ans=arr[i];	
				}
				if(m>=beta)
					return m;
			}	
		}
	}
	return m;
}

void AIChess(int checkerboard[][Size]){
	nodetotal = 0;
	if(!checkCanPlayChess(checkerboard,player,sn,en)){
		player = 1;
		cout<<"無可下位置，換玩家";
		_sleep(0.5*1000);
		return ;
	}
	int temp[Size][Size];
	memcpy(temp,checkerboard,sizeof(temp));
	START = clock();
	AlphaBeta(temp,-1000,1000,black,white,player,limitDep,sn,en);
	END = clock();
	file1<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<endl;
	updateCheckerboard(checkerboard,ans,black,white,player);
	updateNode(checkerboard,sn,en,ans.row,ans.col);
	player = 1;
	LogCheckerboard<<endl<<"AI : ( "<<ans.row<<","<<ans.col<<" )"<<endl;
}



int main(){
	file1.open("./test/"+to_string(limitDep-1)+"A/Alpha-beta.csv",ios::out|ios::trunc);
	file1<<"depth "+to_string(limitDep-1)+" :"<<endl;
	file1<<"Time,Node"<<endl;
	LogCheckerboard.open("./test/"+to_string(limitDep-1)+"A/board.txt",ios::out|ios::trunc);
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));
	checkerboard[4][4]=checkerboard[5][5]=1; checkerboard[4][5]=checkerboard[5][4]=-1;
	checkerboard[4][0]=checkerboard[5][0]=checkerboard[0][4]=checkerboard[0][5]=2;
	printCheckerboard(checkerboard);
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
	else
		cout<<"Tie";
	file1.close();
	LogCheckerboard.close();

}

