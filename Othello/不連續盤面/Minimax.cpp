#include"lib.h"
#define Size 9
#define limitDep 8
using namespace std;
//8�Ӥ�V 
int dir[8][2]={ {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
//�p��ɶ� 
double START, END;
//�¤l�ƶq�A�դl�ƶq�A ����(player=1)�Υ�(player=0)�A�j�M�`�I�ƶq 
int black=2, white=2, player=1, nodetotal;

fstream LogBoard, TestBoard;

//�Ѥl�����C�B��M��V 
struct Node{
	int row, col, dir;
	Node():row(-1), col(-1), dir(0){}
	Node(int r, int c):row(r), col(c), dir(0){}
	Node(int r, int c, int d):row(r), col(c), dir(d){}
};

//sn - start node : �ثe�w�U�ѽL�����W��
//en - end node : �ثe�w�U�ѽL���k�U�� 
Node sn((Size-1)/2-1, (Size-1)/2-1), en((Size-1)/2+2, (Size-1)/2+2);
//��s�j�M�d��϶�
//board : ��e�ѽL row : ��e���B�C col : ��e���B���� 
void SearchRange(int board[][Size], Node &sn, Node &en, int row, int col){
	if(sn.row==row){
		if(sn.row!=1 && board[row-1][0]!=Size-1)
			sn.row--;
		while(board[sn.row][0]==Size-1)
			sn.row++;
	}	
	else if(en.row==row){
		if(en.row!=Size-1 && board[row+1][0]!=Size-1)
			en.row++;
		while(board[en.row][0]==Size-1)
			en.row--;
	}
	if(sn.col==col){
		if(sn.col!=1 && board[0][col-1]!=Size-1)
			sn.col--;
		while(board[0][sn.col]==Size-1)
			sn.col++;
	}	
	else if(en.col==col){
		if(en.col!=Size-1 && board[0][col+1]!=Size-1)
			en.col++;
		while(board[0][en.col]==Size-1)
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

//Minimax search - black: �¤l�ƥ� white: �դl�ƥ� skip : �ĤH�O�_�L�l�i�U 
int Minimax(int board[][Size],int black,int white,int player,int depth,Node sn,Node en,bool skip){ 
	//��`��=0��gameover 
	if(depth==0 || black+white==64 || white==0 || black==0){
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
			return -1*Minimax(board,black,white,!player,depth,sn,en,true);	
		}
		nodetotal+=arr.size();
		/////Code Start/////  
		for(int i=0;i<arr.size();i++){
			int temp[Size][Size],tempB=black,tempW=white;
			Node tempSn=sn,tempEn=en;
			memcpy(temp,board,sizeof(temp));
			UpdateBoard(temp,arr[i],tempB,tempW,player);
			SearchRange(temp,tempSn,tempEn,arr[i].row,arr[i].col);
			int t=-1*Minimax(temp,tempB,tempW,!player,depth-1,tempSn,tempEn,false);
			if(t>m){
				m=t;
			}
		}
		/////Code End///// 
		return m;
	}
}


void AI(int board[][Size]){
	nodetotal = 0;
	int temp[Size][Size];
	memcpy(temp,board,sizeof(temp));
	START = clock();
	Minimax(temp,black,white,player,limitDep,sn,en,false);
	END = clock();
	LogBoard<<fixed<<setprecision(2)<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<","<<endl;
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
	LogBoard.open("./Minimax.csv",ios::out);
	LogBoard<<"depth "+to_string(limitDep)+" :"<<endl;
	LogBoard<<"Board,Time,Node"<<endl;
	
	int board[Size][Size];
	memset (board,0,sizeof(board));
	string str;
	for(int i=1;i<=50;i++){
		/////Code Start///// 
		//��l�ƴѽL64�Ӫ�l��8 
		black=0; white=0;
		memset (board,0,sizeof(board));
		for(int i=1;i<Size;i++){
			for(int j=1;j<Size;j++){
				board[i][j]=8;
			}
		}
		/////Code End///// 
		
		/////Code Start/////
		//���J�L�� 
		TestBoard.open("./othello-initial board-2000/"+to_string(i)+".txt",ios::in);
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
		LogBoard<<i<<",";
		AI(board);
	}
	LogBoard.close();
}

