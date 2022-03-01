#include"lib.h"
#define Size 9
using namespace std;
//8個方向 
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
//計算時間
clock_t start;
//黑子數量，白子數量， 輪黑(player=1)或白(player=0)，搜尋節點深度
int black=2,white=2,player=1,limitDep;
//搜尋節點數量 
unsigned long long int nodetotal;
fstream LogBoard,HistoryBoard;

int Order[Size][Size]={{0,1,2,3,4,5,6,7,8},
					  {1,0,7,1,3,3,1,7,0},
					  {2,7,8,4,5,5,4,8,7},
					  {3,1,4,2,6,6,2,4,1},
				 	  {4,3,5,6,0,0,6,5,3},
					  {5,3,5,6,0,0,6,5,3},
					  {6,1,4,2,6,6,2,4,1},
					  {7,7,8,4,5,5,4,8,7},
					  {8,0,7,1,3,3,1,7,0}};

//棋子紀錄列、行和方向 						  
struct Node{
	int row,col,dir;
	Node():row(-1),col(-1),dir(0){}
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
};

int Max(int a,int b){
	return (a>b)?a:b;
}

//sn - start node : 目前已下棋盤中左上角
//en - end node : 目前已下棋盤中右下角 
Node sn(3,3),en(6,6);
//更新搜尋範圍區間
//board : 當前棋盤 row : 當前走步列 col : 當前走步的行 
void SearchRange(int board[][Size],Node &sn,Node &en,int row,int col){
	if(sn.row==row){
		if(sn.row!=1&&board[row-1][0]!=8)
			sn.row--;
		while(board[sn.row][0]==8)
			sn.row++;
	}	
	else if(en.row==row){
		if(en.row!=8&&board[row+1][0]!=8)
			en.row++;
		while(board[en.row][0]==8)
			en.row--;
	}
	if(sn.col==col){
		if(sn.col!=1&&board[0][col-1]!=8)
			sn.col--;
		while(board[0][sn.col]==8)
			sn.col++;
	}	
	else if(en.col==col){
		if(en.col!=8&&board[0][col+1]!=8)
			en.col++;
		while(board[0][en.col]==8)
			en.col--;
	}
}

void OutputBoard(int board[][Size]){
	HistoryBoard<<"    ";
	for(int i=0;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(i==0)
				HistoryBoard<<j<<"  ";
			else
				if(board[i][j]>=0)
					HistoryBoard<<" - ";
				else if(board[i][j]==-2)
					HistoryBoard<<" O ";
				else
					HistoryBoard<<" X ";
		}
		if(i!=Size-1)
			HistoryBoard<<"\n\n "<<i+1<<" ";			
	}
	HistoryBoard<<"\n O : "<<black<<"   X : "<<white;
	if(player)
		HistoryBoard<<"\t->player(O)\tAI(X)"<<endl;		
	else
		HistoryBoard<<"\tplayer(O)\t->AI(X)"<<endl;		
}

void PrintBoard(int board[][Size]){
	system("cls");
	cout<<"    ";
	for(int i=0;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(i==0)
				cout<<j<<"  ";	
			else
				if(board[i][j]>=0)
					cout<<" - ";
				else if(board[i][j]==-2)
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

//更新盤面 - 翻棋 、 計算每列每行棋子數量 和 修改空位置最早出現棋子方向 
void UpdateBoard(int board[][Size], Node NowMove, int &black, int &white, int player){
	int xStart, yStart;
	for(int i=NowMove.dir; i<8; i++){
		xStart=NowMove.row+dir[i][0]; yStart=NowMove.col+dir[i][1];
		//判斷不能翻棋方向 
		if(xStart==0 || yStart==0 || xStart==Size || yStart==Size || (player && board[xStart][yStart]==-2) || (!player && board[xStart][yStart]==-1))
			continue;
		//更新空位置最早出現棋子方向 NowMove.dir ~ 8 
		if(board[xStart][yStart]>=0){
			board[xStart][yStart]=( board[xStart][yStart]>(i+4)%8 ) ? (i+4)%8 : board[xStart][yStart];
			continue;
		}
		
		/////Code Start///// 
		//翻子和計算每列每行棋子數量 
		bool findplayerColor=false;
		xStart+=dir[i][0];
		yStart+=dir[i][1];
		while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&board[xStart][yStart]<0){
			if((player&&board[xStart][yStart]==-2)||(!player&&board[xStart][yStart]==-1)){
				findplayerColor=true;
				break;
			}
			xStart+=dir[i][0];
			yStart+=dir[i][1];
		}
		while(findplayerColor){
			xStart-=dir[i][0];
			yStart-=dir[i][1];
			if(xStart==NowMove.row&&yStart==NowMove.col)
				break;
			if(player&&board[xStart][yStart]==-1){
				white--;
				black++;
				board[xStart][yStart]=-2;
			}
			else if(!player&&board[xStart][yStart]==-2){
				white++;
				black--;
				board[xStart][yStart]=-1;
			}
		}

	}
	if(player){
		board[NowMove.row][NowMove.col]=-2;
		black++;
	}
	else{
		board[NowMove.row][NowMove.col]=-1;
		white++;
	}
	board[NowMove.row][0]++;
	board[0][NowMove.col]++;
	/////Code End///// 
	
	//更新空位置最早出現棋子方向 0 ~ NowMove.dir 
	for(int i=0;i<NowMove.dir;i++){
		xStart=NowMove.row+dir[i][0];yStart=NowMove.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size)
			continue;
		if(board[xStart][yStart]>=0)
			board[xStart][yStart]=(board[xStart][yStart]>(i+4)%8)?(i+4)%8:board[xStart][yStart];		
	}
}

bool isGameOver(int board[][Size],int black,int white,Node sn,Node en){
	if(black+white==64||white==0||black==0)
		return true;
	else{
		for(int i=sn.row;i<=en.row;i++){
			for(int j=sn.col;j<=en.col;j++){
				if(board[i][j]<0||board[i][j]==8)
					continue;
				for(int k=board[i][j];k<8;k++){
					int xStart=i+dir[k][0],yStart=j+dir[k][1];
					if(xStart==0||yStart==0||xStart==Size||yStart==Size||board[xStart][yStart]>=0)
						continue;
					xStart+=dir[k][0];
					yStart+=dir[k][1];
					while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&board[xStart][yStart]<0){
						if(board[xStart-dir[k][0]][yStart-dir[k][1]]!=board[xStart][yStart])
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

bool method(Node a, Node b){
	if(Order[a.row][a.col] < Order[b.row][b.col]) return true;
	return false;
}

//合法走步生成 
vector<Node> MoveGenerator(int board[][Size], int player, Node sn, Node en){
	vector<Node> arr;
	//i : 列
	//j : 行
	//k : 方向 
	for(int i=sn.row; i<=en.row; i++){
		for(int j=sn.col; j<=en.col; j++){
			bool find = false;
			if(board[i][j]<0||board[i][j]==8)
				continue;
			for(int k=board[i][j]; k<8; k++){
				int xStart=i+dir[k][0], yStart=j+dir[k][1];
				//判斷不能下棋方向 
				if(xStart==0 || yStart==0 || xStart==Size || yStart==Size || board[xStart][yStart]>=0 || (player&&board[xStart][yStart]==-2) || (!player&&board[xStart][yStart]==-1))
					continue;
				/////Code Start/////
				//判斷(i,j)位置的k方向是否可下棋
				xStart+=dir[k][0];
				yStart+=dir[k][1];
				while(xStart>0 && xStart<Size && yStart>0 && yStart<Size&&board[xStart][yStart]<0){
					if((player && board[xStart][yStart]==-2) || (!player&&board[xStart][yStart]==-1)){
						arr.push_back(Node(i,j,k));
						find = true;
						break;
					}
					xStart+=dir[k][0];
					yStart+=dir[k][1];
				}
				if(find) break;
				/////Code End///// 
			}
		}		
	}
	return arr;
}

Node ans(0,0);
int AlphaBeta(int board[][Size], int alpha, int beta, int black, int white, int player, int depth, Node sn, Node en, bool skip){
	//當深度=0或gameover
	if(depth==0 || black+white==64 || white==0 || black==0){ return (player)?black-white:white-black; }
	else{
		// MoveGenerator
		vector<Node> arr = MoveGenerator(board, player, sn, en);
		int m=INT_MIN;
		// 無子可下
		if(arr.size()==0){
			if(skip) return (!player) ? black-white : white-black;
			return -1*AlphaBeta(board, -1*beta, -1*alpha, black, white, !player, depth, sn, en, true);
		}
		
		nodetotal+=arr.size();
		if(depth>=4){ sort(arr.begin(),arr.end(),method); }
		
		for(int i=0;i<arr.size();i++){
			int temp[Size][Size], tempB=black, tempW=white;
			Node tempSn=sn, tempEn=en;
			memcpy(temp, board, sizeof(temp));
			UpdateBoard(temp, arr[i], tempB, tempW, player);
			SearchRange(temp, tempSn, tempEn, arr[i].row, arr[i].col);
			int t=-1*AlphaBeta(temp, -1*beta, -1*Max(m,alpha), tempB, tempW, !player, depth-1, tempSn, tempEn, false);
			if(t>m){
				if(depth==limitDep) { ans=arr[i]; } 
				m=t;
			}
			if(m>=beta){ return m; }				
		}
		return m;
	}
}

int i=1;
void AI(int board[][Size]){
	nodetotal = 0;
	vector<Node> arr = MoveGenerator(board,player,sn,en);
	if(arr.size()==0){
		player = 1;
		cout<<"無可下位置，換玩家";
		_sleep(0.5*1000);
		PrintBoard(board);
		return ;
	}
	
	int temp[Size][Size];
	memcpy(temp,board,sizeof(temp));
	start = clock();
	LogBoard<<i++;

	for(limitDep=2 ; limitDep<=12 ; limitDep+=2){
		cout<<"depth "<<setw(2)<<limitDep;
		nodetotal = 0;
		AlphaBeta(temp,-10000,10000,black,white,player,limitDep,sn,en,false);
		LogBoard<<","<<fixed<<setprecision(2)<<(double)(clock() - start) / CLOCKS_PER_SEC<<","<<nodetotal;
		cout<<" : finish"<<endl;
	}
	LogBoard<<endl;	
	
	//update
	UpdateBoard(board,ans,black,white,player);
	SearchRange(board,sn,en,ans.row,ans.col);
	player = 1;
	//update print board message
	HistoryBoard<<endl<<"AI : ( "<<ans.row<<","<<ans.col<<" )"<<endl;
	OutputBoard(board);
	PrintBoard(board);

}

void Player1(int board[][Size]){
	vector<Node> arr = MoveGenerator(board, player, sn, en);
	if(arr.size()==0){
		player = 0;
		cout<<"無可下位置，換AI";
		_sleep(0.5*1000);
		PrintBoard(board);
		return ;
	}
	
	cout<<"可下位置 : "<<endl;
	for(auto i:arr){ cout<<"( "<<i.row<<","<<i.col<<" ) "; }	
	Node node(0,0);
	while(1){
		cout<<"\nrow(列) : "; 
		cin>>node.row;
		cout<<"col(行) : ";
		cin>>node.col;
		bool Invalid = true;
		for(auto i:arr){
			if(i.row==node.row && i.col==node.col) Invalid=false;
		}
		if(node.row<1 || node.row>8 || node.col<1 || node.col>8 || board[node.row][node.col]<0 || Invalid){
			cout<<"輸入錯誤\n";
			_sleep(0.5*1000);
		}
		else{
			UpdateBoard(board, node, black, white, player);
			SearchRange(board, sn, en, node.row, node.col);
			player = 0;
			HistoryBoard<<endl<<"player : ( "<<node.row<<","<<node.col<<" )"<<endl;
			OutputBoard(board);
			PrintBoard(board);
			return;					
		}
	}
}

int main(){
	LogBoard.open("./ID-原版.csv",ios::out);
	LogBoard<<",depth2,,depth4,,depth6,,depth8,,depth10,,depth12,,"<<endl;
	LogBoard<<"ply,Time,Node,Time,Node,Time,Node,Time,Node,Time,Node,Time,Node"<<endl;	
	HistoryBoard.open("./ID-原版HisBoard.txt",ios::out);
	
	int board[Size][Size];
	//initial board
	memset (board,0,sizeof(board));
	for(int i=1;i<Size;i++){
		for(int j=1;j<Size;j++){
			board[i][j]=8;
		}
	}	
	board[4][4]=board[5][5]=-2; board[4][5]=board[5][4]=-1;
	board[4][0]=board[5][0]=board[0][4]=board[0][5]=2;
	for(int i=4;i<=5;i++){
		for(int j=4;j<=5;j++){
			for(int k=0;k<8;k++){
				if(board[i+dir[k][0]][j+dir[k][1]]>=0)
					board[i+dir[k][0]][j+dir[k][1]]=(board[i+dir[k][0]][j+dir[k][1]]>(k+4)%8)?(k+4)%8:board[i+dir[k][0]][j+dir[k][1]];
			}
		}
	}
	
	//Start Othello Game
	PrintBoard(board);
	while(!isGameOver(board,black,white,sn,en)){
		if(player)
			Player1(board);
		else
			AI(board);
	}
	
	//Game over
	if(black>white)
		cout<<"player is win";
	else if(white>black)
		cout<<"AI is win";
	else
		cout<<"Tie";
	HistoryBoard.close();
	LogBoard.close();

}

