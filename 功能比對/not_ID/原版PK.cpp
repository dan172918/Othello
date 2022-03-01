#include"lib.h"
#define Size 9
#define limitDep 10 
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
unsigned long long int nodetotal;
fstream LogOthelloBoard;
int Priority[Size][Size]={{0,1,2,3,4,5,6,7,8},
						  {1,0,7,1,3,3,1,7,0},
						  {2,7,8,4,5,5,4,8,7},
						  {3,1,4,2,6,6,2,4,1},
					 	  {4,3,5,6,0,0,6,5,3},
						  {5,3,5,6,0,0,6,5,3},
						  {6,1,4,2,6,6,2,4,1},
						  {7,7,8,4,5,5,4,8,7},
						  {8,0,7,1,3,3,1,7,0}};
						  
struct Node{
	int row,col,dir;
	Node():row(-1),col(-1),dir(0){}
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
};

int Max(int a,int b){
	return (a>b)?a:b;
}

Node sn(3,3),en(6,6);
void updateNode(int OthelloBoard[][Size],Node &sn,Node &en,int row,int col){
	if(sn.row==row){
		if(sn.row!=1&&OthelloBoard[row-1][0]!=8)
			sn.row--;
		while(OthelloBoard[sn.row][0]==8)
			sn.row++;
	}	
	else if(en.row==row){
		if(en.row!=8&&OthelloBoard[row+1][0]!=8)
			en.row++;
		while(OthelloBoard[en.row][0]==8)
			en.row--;
	}
	if(sn.col==col){
		if(sn.col!=1&&OthelloBoard[0][col-1]!=8)
			sn.col--;
		while(OthelloBoard[0][sn.col]==8)
			sn.col++;
	}	
	else if(en.col==col){
		if(en.col!=8&&OthelloBoard[0][col+1]!=8)
			en.col++;
		while(OthelloBoard[0][en.col]==8)
			en.col--;
	}
}

void outputOthelloBoard(int OthelloBoard[][Size]){
	LogOthelloBoard<<"    ";
	for(int i=0;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(i==0)
				LogOthelloBoard<<j<<"  ";
			else
				if(OthelloBoard[i][j]>=0)
					LogOthelloBoard<<" - ";
				else if(OthelloBoard[i][j]==-2)
					LogOthelloBoard<<" O ";
				else
					LogOthelloBoard<<" X ";
		}
		if(i!=Size-1)
			LogOthelloBoard<<"\n\n "<<i+1<<" ";			
	}
	LogOthelloBoard<<"\n O : "<<black<<"   X : "<<white;
	if(player)
		LogOthelloBoard<<"\t->player(O)\tAI(X)"<<endl;		
	else
		LogOthelloBoard<<"\tplayer(O)\t->AI(X)"<<endl;		
}

void printOthelloBoard(int OthelloBoard[][Size]){
	system("cls");
	cout<<"    ";
	for(int i=0;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(i==0)
				cout<<j<<"  ";	
			else
				if(OthelloBoard[i][j]>=0)
					cout<<" - ";
				else if(OthelloBoard[i][j]==-2)
					cout<<" O ";
				else
					cout<<" X ";
		}
		if(i!=Size-1)
			cout<<"\n\n "<<i+1<<" ";
	}
	cout<<"\n O : "<<black<<"   X : "<<white;
	if(player)
		cout<<"\t->player(O)\tAI(X)"<<endl;
	else
		cout<<"\tplayer(O)\t->AI(X)"<<endl;
}

void updateOthelloBoard(int OthelloBoard[][Size],Node node,int &black,int &white,int player){
	int xStart,yStart;
	for(int i=node.dir;i<8;i++){
		xStart=node.row+dir[i][0];yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size||(player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1))
			continue;
		if(OthelloBoard[xStart][yStart]>=0){
			OthelloBoard[xStart][yStart]=(OthelloBoard[xStart][yStart]>(i+4)%8)?(i+4)%8:OthelloBoard[xStart][yStart];
			continue;
		}
		bool findplayerColor=false;
		xStart+=dir[i][0];
		yStart+=dir[i][1];
		while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&OthelloBoard[xStart][yStart]<0){
			if((player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1)){
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
			if(player&&OthelloBoard[xStart][yStart]==-1){
				white--;
				black++;
				OthelloBoard[xStart][yStart]=-2;
			}
			else if(!player&&OthelloBoard[xStart][yStart]==-2){
				white++;
				black--;
				OthelloBoard[xStart][yStart]=-1;
			}
		}

	}
	if(player){
		OthelloBoard[node.row][node.col]=-2;
		black++;
	}
	else{
		OthelloBoard[node.row][node.col]=-1;
		white++;
	}
	OthelloBoard[node.row][0]++;
	OthelloBoard[0][node.col]++;
	for(int i=0;i<node.dir;i++){
		xStart=node.row+dir[i][0];yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size)
			continue;
		if(OthelloBoard[xStart][yStart]>=0)
			OthelloBoard[xStart][yStart]=(OthelloBoard[xStart][yStart]>(i+4)%8)?(i+4)%8:OthelloBoard[xStart][yStart];		
	}
}

bool checkCanPlayChess(int OthelloBoard[][Size],int player,Node sn,Node en){
	for(int i=sn.row;i<=en.row;i++){
		for(int j=sn.col;j<=en.col;j++){
			if(OthelloBoard[i][j]<0||OthelloBoard[i][j]==8)
				continue;
			for(int k=OthelloBoard[i][j];k<8;k++){
				int xStart=i+dir[k][0],yStart=j+dir[k][1];
				if(xStart==0||yStart==0||xStart==Size||yStart==Size||OthelloBoard[xStart][yStart]>=0||(player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1))
					continue;
				xStart+=dir[k][0];
				yStart+=dir[k][1];
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&OthelloBoard[xStart][yStart]<0){
					if((player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1))
						return true;
					xStart+=dir[k][0];
					yStart+=dir[k][1];
				}
			}
		}		
	}
	return false;
}

bool isGameOver(int OthelloBoard[][Size],int black,int white,Node sn,Node en){
	if(black+white==64||white==0||black==0)
		return true;
	else{
		for(int i=sn.row;i<=en.row;i++){
			for(int j=sn.col;j<=en.col;j++){
				if(OthelloBoard[i][j]<0||OthelloBoard[i][j]==8)
					continue;
				for(int k=OthelloBoard[i][j];k<8;k++){
					int xStart=i+dir[k][0],yStart=j+dir[k][1];
					if(xStart==0||yStart==0||xStart==Size||yStart==Size||OthelloBoard[xStart][yStart]>=0)
						continue;
					xStart+=dir[k][0];
					yStart+=dir[k][1];
					while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&OthelloBoard[xStart][yStart]<0){
						if(OthelloBoard[xStart-dir[k][0]][yStart-dir[k][1]]!=OthelloBoard[xStart][yStart])
							return false;
						xStart+=dir[k][0];
						yStart+=dir[k][1];
					}
				}
			}		
		}		
	}
	return true;
}

bool method(Node a,Node b){
	if(Priority[a.row][a.col]<Priority[b.row][b.col])
		return true;
	return false;
}


vector<Node> findnode(int OthelloBoard[][Size],int player,Node sn,Node en){
	vector<Node> arr;
	for(int i=sn.row;i<=en.row;i++){
		for(int j=sn.col;j<=en.col;j++){
			bool find = false;
			if(OthelloBoard[i][j]<0||OthelloBoard[i][j]==8)
				continue;
			for(int k=OthelloBoard[i][j];k<8;k++){
				int xStart=i+dir[k][0],yStart=j+dir[k][1];
				if(xStart==0||yStart==0||xStart==Size||yStart==Size||OthelloBoard[xStart][yStart]>=0||(player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1))
					continue;
				xStart+=dir[k][0];
				yStart+=dir[k][1];
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&OthelloBoard[xStart][yStart]<0){
					if((player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1)){
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
int AlphaBeta(int OthelloBoard[][Size],int alpha,int beta,int black,int white,int player,int depth,Node sn,Node en,bool skip){
	if(depth==limitDep||black+white==64||white==0||black==0){
		return (player)?black-white:white-black;
	}
	else{
		vector<Node> arr = findnode(OthelloBoard,player,sn,en);
		int m=INT_MIN;
		if(arr.size()==0){
			if(skip)
				return (!player)?black-white:white-black;
			return -1*AlphaBeta(OthelloBoard,-1*beta,-1*alpha,black,white,!player,depth,sn,en,true);
		}
		nodetotal+=arr.size();
		if(depth<6){
			sort(arr.begin(),arr.end(),method);			
		}
			
		for(int i=0;i<arr.size();i++){
			int temp[Size][Size],tempB=black,tempW=white;
			Node tempSn=sn,tempEn=en;
			memcpy(temp,OthelloBoard,sizeof(temp));
			updateOthelloBoard(temp,arr[i],tempB,tempW,player);
			updateNode(temp,tempSn,tempEn,arr[i].row,arr[i].col);
			int t=-1*AlphaBeta(temp,-1*beta,-1*Max(m,alpha),tempB,tempW,!player,depth+1,tempSn,tempEn,false);
			if(t>m){
				m=t;
				if(depth==0)
					ans=arr[i];	
			}
			if(m>=beta){
				return m;
			}				
		}
		return m;
	}
}

void AIChess(int OthelloBoard[][Size]){
	nodetotal = 0;
	//if not move generator, Play chess for the player 
	if(!checkCanPlayChess(OthelloBoard,player,sn,en)){
		player = 1;
		cout<<"無可下位置，換玩家";
		_sleep(0.5*1000);
		printOthelloBoard(OthelloBoard);
		return ;
	}
	//alpha beta search
	int temp[Size][Size];
	memcpy(temp,OthelloBoard,sizeof(temp));
	START = clock();
	AlphaBeta(temp,-1000,1000,black,white,player,0,sn,en,false);
	END = clock();
	//update and output excel log
	LogOthelloBoard<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<endl;
	updateOthelloBoard(OthelloBoard,ans,black,white,player);
	updateNode(OthelloBoard,sn,en,ans.row,ans.col);
	//update print board message
	player = 1;
	printOthelloBoard(OthelloBoard);
}

void playerChess(int OthelloBoard[][Size]){
	vector<Node> arr = findnode(OthelloBoard,player,sn,en);
	//if not move generator, Play chess for the AI 
	if(arr.size()==0){
		player = 0;
		cout<<"無可下位置，換AI";
		_sleep(0.5*1000);
		printOthelloBoard(OthelloBoard);
		return ;
	}
	//print move generator 
	cout<<"可下位置 : "<<endl;
	for(auto i:arr){
		cout<<"( "<<i.row<<","<<i.col<<" ) ";
	}
	//input now play chess	
	Node node(0,0);
	while(1){
		cout<<"\nrow(列) : "; 
		cin>>node.row;
		cout<<"col(行) : ";
		cin>>node.col;
		bool Invalid = true;
		for(auto i:arr){
			if(i.row==node.row&&i.col==node.col)
				Invalid=false;
		}
		if(node.row<1||node.row>8||node.col<1||node.col>8||OthelloBoard[node.row][node.col]<0||Invalid){
			cout<<"輸入錯誤\n";
			_sleep(0.5*1000);
		}
		else{
			//update board
			updateOthelloBoard(OthelloBoard,node,black,white,player);
			//update move generator range
			updateNode(OthelloBoard,sn,en,node.row,node.col);
			player = 0;
			//update print board message
			printOthelloBoard(OthelloBoard);
			return;					
		}
	}
}

int main(){
	LogOthelloBoard.open("./原版PK.csv",ios::out);
	LogOthelloBoard<<"depth "+to_string(limitDep)+" :"<<endl;
	LogOthelloBoard<<"Time,Node"<<endl;
	int OthelloBoard[Size][Size];
	//initial board
	memset (OthelloBoard,0,sizeof(OthelloBoard));
	for(int i=1;i<Size;i++){
		for(int j=1;j<Size;j++){
			OthelloBoard[i][j]=8;
		}
	}
	OthelloBoard[4][4]=OthelloBoard[5][5]=-2; OthelloBoard[4][5]=OthelloBoard[5][4]=-1;
	OthelloBoard[4][0]=OthelloBoard[5][0]=OthelloBoard[0][4]=OthelloBoard[0][5]=2;
	for(int i=4;i<=5;i++){
		for(int j=4;j<=5;j++){
			for(int k=0;k<8;k++){
				if(OthelloBoard[i+dir[k][0]][j+dir[k][1]]>=0)
					OthelloBoard[i+dir[k][0]][j+dir[k][1]]=(OthelloBoard[i+dir[k][0]][j+dir[k][1]]>(k+4)%8)?(k+4)%8:OthelloBoard[i+dir[k][0]][j+dir[k][1]];
			}
		}
	}
	
	//Start Othello Game
	printOthelloBoard(OthelloBoard);
	while(!isGameOver(OthelloBoard,black,white,sn,en)){
		if(player)
			playerChess(OthelloBoard);
		else
			AIChess(OthelloBoard);	
	}
	
	//Game over
	if(black>white)
		cout<<"player is win";
	else if(white>black)
		cout<<"AI is win";
	else
		cout<<"Tie";
		
	LogOthelloBoard.close();

}

