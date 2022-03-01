#include"lib.h"
#define Size 9
#define TIME_LIMIT 59.5
using namespace std;
//8�Ӥ�V 
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
//�p��ɶ� 
clock_t start;
//�¤l�ƶq�A�դl�ƶq�A ����(player=1)�Υ�(player=0)�A�`�׭��� 
int black=2,white=2,player=1,limitDep;
//�j�M�`�I�ƶq 
unsigned long long int nodetotal;
fstream LogBoard,TestBoard;

int Order[Size][Size]={{0,1,2,3,4,5,6,7,8},
					  {1,0,7,1,3,3,1,7,0},
					  {2,7,8,4,5,5,4,8,7},
					  {3,1,4,2,6,6,2,4,1},
				 	  {4,3,5,6,0,0,6,5,3},
					  {5,3,5,6,0,0,6,5,3},
					  {6,1,4,2,6,6,2,4,1},
					  {7,7,8,4,5,5,4,8,7},
					  {8,0,7,1,3,3,1,7,0}};
						  
//�Ѥl�����C�B��M��V					  
struct Node{
	int row,col,dir;
	Node():row(-1),col(-1),dir(0){}
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
};

int Max(int a,int b){
	return (a>b)?a:b;
}

//sn - start node : �ثe�w�U�ѽL�����W��
//en - end node : �ثe�w�U�ѽL���k�U�� 
Node sn((Size-1)/2-1,(Size-1)/2-1),en((Size-1)/2+2,(Size-1)/2+2); 
//��s�j�M�d��϶�
//board : ��e�ѽL row : ��e���B�C col : ��e���B���� 
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

//��s�L�� - ½�� �B �p��C�C�C��Ѥl�ƶq �M �ק�Ŧ�m�̦��X�{�Ѥl��V 
void UpdateBoard(int board[][Size], Node NowMove, int &black, int &white, int player){
	int xStart, yStart;
	for(int i=NowMove.dir; i<8; i++){
		xStart=NowMove.row+dir[i][0]; yStart=NowMove.col+dir[i][1];
		//�P�_����½�Ѥ�V 
		if(xStart==0 || yStart==0 || xStart==Size || yStart==Size || (player && board[xStart][yStart]==-2) || (!player && board[xStart][yStart]==-1))
			continue;
		//��s�Ŧ�m�̦��X�{�Ѥl��V NowMove.dir ~ 8 
		if(board[xStart][yStart]>=0){
			board[xStart][yStart]=( board[xStart][yStart]>(i+4)%8 ) ? (i+4)%8 : board[xStart][yStart];
			continue;
		}
		
		/////Code Start///// 
		//½�l�M�p��C�C�C��Ѥl�ƶq 
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
	
	//��s�Ŧ�m�̦��X�{�Ѥl��V 0 ~ NowMove.dir 
	for(int i=0;i<NowMove.dir;i++){
		xStart=NowMove.row+dir[i][0];yStart=NowMove.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size)
			continue;
		if(board[xStart][yStart]>=0)
			board[xStart][yStart]=(board[xStart][yStart]>(i+4)%8)?(i+4)%8:board[xStart][yStart];		
	}
}

bool method(Node a,Node b){
	if(Order[a.row][a.col]<Order[b.row][b.col])
		return true;
	return false;
}

//�X�k���B�ͦ� 
vector<Node> MoveGenerator(int board[][Size], int player, Node sn, Node en){
	vector<Node> arr;
	//i : �C
	//j : ��
	//k : ��V 
	for(int i=sn.row; i<=en.row; i++){
		for(int j=sn.col; j<=en.col; j++){
			bool find = false;
			if(board[i][j]<0||board[i][j]==8)
				continue;
			for(int k=board[i][j]; k<8; k++){
				int xStart=i+dir[k][0], yStart=j+dir[k][1];
				//�P�_����U�Ѥ�V 
				if(xStart==0 || yStart==0 || xStart==Size || yStart==Size || board[xStart][yStart]>=0 || (player&&board[xStart][yStart]==-2) || (!player&&board[xStart][yStart]==-1))
					continue;
				/////Code Start/////
				//�P�_(i,j)��m��k��V�O�_�i�U��
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
int AlphaBeta(int board[][Size],int alpha,int beta,int black,int white,int player,int depth,Node sn,Node en,bool skip){
	//��`��=0��gameover
	if(depth==0||black+white==64||white==0||black==0){
		return (player)?black-white:white-black;
	}
	else{
		// MoveGenerator
		vector<Node> arr = MoveGenerator(board,player,sn,en);
		int m=INT_MIN;
		// �L�l�i�U
		if(arr.size()==0){
			if(skip)
				return (!player)?black-white:white-black;
			return -1*AlphaBeta(board,-1*beta,-1*alpha,black,white,!player,depth,sn,en,true);
		}
		
		nodetotal+=arr.size();
		if(depth>3){
			sort(arr.begin(),arr.end(),method);			
		}
		
		for(int i=0;i<arr.size();i++){
			int temp[Size][Size],tempB=black,tempW=white;
			Node tempSn=sn,tempEn=en;
			memcpy(temp,board,sizeof(temp));
			UpdateBoard(temp,arr[i],tempB,tempW,player);
			SearchRange(temp,tempSn,tempEn,arr[i].row,arr[i].col);
			int t=-1*AlphaBeta(temp,-1*beta,-1*Max(m,alpha),tempB,tempW,!player,depth-1,tempSn,tempEn,false);
			if(t>m){
				if(depth==0)
					ans=arr[i];
				m=t;
			}
			if(m>=beta){ return m; }				
		}
		return m;
	}
}

void AI(int board[][Size]){
	int temp[Size][Size];
	memcpy(temp,board,sizeof(temp));
	start = clock();
	
	//depth limit
	for(limitDep=2 ; limitDep<=12; limitDep+=2){
		nodetotal = 0;
		AlphaBeta(temp,-10000,10000,black,white,player,limitDep,sn,en,false);
		LogBoard<<fixed<<setprecision(2)<<","<<(double)(clock() - start) / CLOCKS_PER_SEC<<","<<nodetotal;
	}	
	LogBoard<<endl;

}
// �̾ڷs�L����l�Ʒj�M�d��϶�
void UpdateInitialRange(int board[][Size]){
	for(int i=1;i<Size;i++){
		if(board[i][0]==8)
			continue;
		if(board[i][0]!=0){
			sn.row=(i!=1)?((board[i-1][0]!=8)?i-1:i):i;
		break;
		}
	}
	for(int i=Size-1;i>0;i--){
		if(board[i][0]==8)
			continue;
		if(board[i][0]!=0){
			en.row=(i!=Size-1)?((board[i+1][0]!=8)?i+1:i):i;
		break;
		}
	}
	
	for(int i=1;i<Size;i++){
		if(board[0][i]==8)
			continue;
		if(board[0][i]!=0){
			sn.col=(i!=1)?((board[0][i-1]!=8)?i-1:i):i;
		break;
		}
	}
	for(int i=Size-1;i>0;i--){
		if(board[0][i]==8)
			continue;
		if(board[0][i]!=0){
			en.col=(i!=Size-1)?((board[0][i+1]!=8)?i+1:i):i;
		break;
		}
	}
}

int main(){
	LogBoard.open("./ID-�쪩.csv",ios::out);
	LogBoard<<",depth2,,depth4,,depth6,,depth8,,depth10,,depth12,,"<<endl;
	LogBoard<<"board,Time,Node,Time,Node,Time,Node,Time,Node,Time,Node,Time,Node"<<endl;
	int board[Size][Size];
	int boardNum[50]={1390,1963,1337,415,1612,560,1314,846,382,1667,1065,1960,915,155,40,1263,865,150,700,1741,876,1977,949,1626,570,89,341,269,1873,1467,1870,908,462,1657,85,393,151,457,1324,1786,806,336,1911,1609,1871,713,1671,1927,798,601};
	memset (board,0,sizeof(board));
	string str;
	for(int i=0;i<50;i++){
		LogBoard<<boardNum[i];
		/////Code Start///// 
		//��l�ƴѽL64�Ӫ�l��8 
		black=0;white=0;
		memset (board,0,sizeof(board));
		for(int j=1;j<Size;j++){
			for(int k=1;k<Size;k++){
				board[j][k]=8;
			}
		}
		/////Code End/////
		
		/////Code Start/////
		//���J�L�� 	
		TestBoard.open("./othello-initial board-2000/"+to_string(boardNum[i])+".txt",ios::in);
		TestBoard>>str;
		player = (str=="o")?1:0;
		for(int j=1;j<Size;j++){
			TestBoard>>str;
			for(int k=1;k<Size;k++){
				if(str[k-1]!='_'){
					board[j][0]++;
					board[0][k]++;
					if(str[k-1]=='o'){
						board[j][k]=-2;
						black++;
					}
					else{
						board[j][k]=-1;
						white++;
					}
				}
			}
		}
		/////Code End///// 
		
		/////Code Start/////
		//�վ�Ŧ�m�̦��X�{�Ѥl��V 
		for(int a=1;a<Size;a++){
			for(int j=1;j<Size;j++){
				if(board[a][j]<0){
					for(int k=0;k<8;k++){
						if(a+dir[k][0]>0&&a+dir[k][0]<Size&&j+dir[k][1]>0&&j+dir[k][1]<Size&&board[a+dir[k][0]][j+dir[k][1]]>=0)
							board[a+dir[k][0]][j+dir[k][1]]=(board[a+dir[k][0]][j+dir[k][1]]>(k+4)%8)?(k+4)%8:board[a+dir[k][0]][j+dir[k][1]];
					}					
				}
			}
		}
		/////Code End/////
		TestBoard.close();
		UpdateInitialRange(board);
		AI(board);
		cout<<"board "<<setw(4)<<boardNum[i]<<" : finish"<<endl;
	}
	LogBoard.close();
}

