#include"lib.h"
#define Size 9
#define TIME_LIMIT 59.5
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
clock_t start;
int black=2,white=2,player=1,limitDep;
unsigned long long int nodetotal;
fstream LogOthelloBoard,file1;
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

Node sn((Size-1)/2-1,(Size-1)/2-1),en((Size-1)/2+2,(Size-1)/2+2); 
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

bool method(Node a,Node b){
	if(Priority[a.row][a.col]<Priority[b.row][b.col])
		return true;
//	if(Priority[a.row][a.col]==Priority[b.row][b.col]){
//		if(a.row<b.row)
//			return true;
//		else if(a.row==b.row){
//			if(a.col<b.col)
//				return true;
//		}
//	}
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

bool timeIsUp(){
	return ((double)(clock() - start) / CLOCKS_PER_SEC >= TIME_LIMIT);
}

unsigned long long int CutNum=0;
int num=0;
Node ans(0,0);
int AlphaBeta(int OthelloBoard[][Size],int alpha,int beta,int black,int white,int player,int depth,Node sn,Node en,bool skip){
	if(/*timeIsUp()||*/depth==limitDep||black+white==64||white==0||black==0){
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
//		if(depth<(limitDep-3)){
			sort(arr.begin(),arr.end(),method);			
//		}

		for(auto i:arr)
			cout<<depth<<" "<<i.row<<" "<<i.col<<endl;
		cout<<endl;
		_sleep(50000000);
		for(int i=0;i<arr.size();i++){
			int temp[Size][Size],tempB=black,tempW=white;
			Node tempSn=sn,tempEn=en;
			memcpy(temp,OthelloBoard,sizeof(temp));
			updateOthelloBoard(temp,arr[i],tempB,tempW,player);
			updateNode(temp,tempSn,tempEn,arr[i].row,arr[i].col);
			int t=-1*AlphaBeta(temp,-1*beta,-1*Max(m,alpha),tempB,tempW,!player,depth+1,tempSn,tempEn,false);
//			cout<<depth<<" "<<(arr[i].row-1)*8+arr[i].col-1<<" "<<t<<endl;
			cout<<depth<<" "<<arr[i].row<<" "<<arr[i].col<<" "<<t<<endl;
			if(t>m){
				if(depth==0)
					ans=arr[i];
				m=t;
			}
			if(m>=beta){
				for(int j=i+1;j<arr.size();j++){
					cout<<depth<<" "<<arr[j].row<<" "<<arr[j].col<<" cut"<<endl;
				}
				cout<<alpha<<" "<<beta<<endl;
				return m;
			}				
		}
		cout<<alpha<<" "<<beta<<endl;
		return m;
	}
}

void AIChess(int OthelloBoard[][Size]){
	int temp[Size][Size],m;
	memcpy(temp,OthelloBoard,sizeof(temp));
	start = clock();
	//time limit
//	for(limitDep=2 ; (double)(clock() - start)/CLOCKS_PER_SEC < TIME_LIMIT ; limitDep+=2){
//		nodetotal = 0;
//		AlphaBeta(temp,-10000,10000,black,white,player,0,sn,en,false);
//		LogOthelloBoard<<","<<(timeIsUp() ? 'U' : 'D')<<","<<limitDep<<","<<nodetotal<<endl;	
//	}
	//depth<=16
	
	for(limitDep=2 ; limitDep<=2; limitDep+=2){
		nodetotal = 0;
		m=AlphaBeta(temp,-10000,10000,black,white,player,0,sn,en,false);
		LogOthelloBoard<<","<<(double)(clock() - start) / CLOCKS_PER_SEC<<","<<nodetotal<<","<<(ans.row-1)*8+ans.col-1;
	}
//	LogOthelloBoard<<","<<ans.row<<ans.col;	
	LogOthelloBoard<<endl;
//	cout<<CutNum<<endl;
//	cout<<num<<endl;
//	cout<<CutNum/num<<endl;
}

void updateInitialNode(int OthelloBoard[][Size]){
	for(int i=1;i<Size;i++){
		if(OthelloBoard[i][0]==8)
			continue;
		if(OthelloBoard[i][0]!=0){
			sn.row=(i!=1)?((OthelloBoard[i-1][0]!=8)?i-1:i):i;
		break;
		}
	}
	for(int i=Size-1;i>0;i--){
		if(OthelloBoard[i][0]==8)
			continue;
		if(OthelloBoard[i][0]!=0){
			en.row=(i!=Size-1)?((OthelloBoard[i+1][0]!=8)?i+1:i):i;
		break;
		}
	}
	
	for(int i=1;i<Size;i++){
		if(OthelloBoard[0][i]==8)
			continue;
		if(OthelloBoard[0][i]!=0){
			sn.col=(i!=1)?((OthelloBoard[0][i-1]!=8)?i-1:i):i;
		break;
		}
	}
	for(int i=Size-1;i>0;i--){
		if(OthelloBoard[0][i]==8)
			continue;
		if(OthelloBoard[0][i]!=0){
			en.col=(i!=Size-1)?((OthelloBoard[0][i+1]!=8)?i+1:i):i;
		break;
		}
	}
}

int main(){
	LogOthelloBoard.open("./IDª©_Alpha-Beta.csv",ios::out);
//	LogOthelloBoard<<"board,U/D,depth,Node"<<endl;
	LogOthelloBoard<<",depth2,,,depth4,,,depth6,,,depth8,,,depth10,,,depth12,,,depth14,,,"<<endl;
	LogOthelloBoard<<"board,Time,Node,value,Time,Node,value,Time,Node,value,Time,Node,value,Time,Node,value,Time,Node,value,Time,Node,value"<<endl;
	int OthelloBoard[Size][Size];
	int boardNum[50]={1390,1963,1337,415,1612,560,1314,846,382,1667,1065,1960,915,155,40,1263,865,150,700,1741,876,1977,949,1626,570,89,341,269,1873,1467,1870,908,462,1657,85,393,151,457,1324,1786,806,336,1911,1609,1871,713,1671,1927,798,601};
	memset (OthelloBoard,0,sizeof(OthelloBoard));
	string str;
	for(int i=3;i<=3;i++){
//		LogOthelloBoard<<boardNum[i];
		LogOthelloBoard<<i;
		//initial board
		black=0;white=0;
		memset (OthelloBoard,0,sizeof(OthelloBoard));
		for(int j=1;j<Size;j++){
			for(int k=1;k<Size;k++){
				OthelloBoard[j][k]=8;
			}
		}

		//file loading initial board
//		file1.open("./board/"+to_string(boardNum[i])+".txt",ios::in);
		file1.open("./Bo/"+to_string(i)+".txt",ios::in);
		file1>>str;
		player = (str=="o")?1:0;
		for(int j=1;j<Size;j++){
			file1>>str;
			for(int k=1;k<Size;k++){
				if(str[k-1]!='_'){
					OthelloBoard[j][0]++;
					OthelloBoard[0][k]++;
					if(str[k-1]=='o'){
						OthelloBoard[j][k]=-2;
						black++;
					}
					else{
						OthelloBoard[j][k]=-1;
						white++;
					}
				}
			}
		}
		//update dir 
		for(int a=1;a<Size;a++){
			for(int j=1;j<Size;j++){
				if(OthelloBoard[a][j]<0){
					for(int k=0;k<8;k++){
						if(a+dir[k][0]>0&&a+dir[k][0]<Size&&j+dir[k][1]>0&&j+dir[k][1]<Size&&OthelloBoard[a+dir[k][0]][j+dir[k][1]]>=0)
							OthelloBoard[a+dir[k][0]][j+dir[k][1]]=(OthelloBoard[a+dir[k][0]][j+dir[k][1]]>(k+4)%8)?(k+4)%8:OthelloBoard[a+dir[k][0]][j+dir[k][1]];
					}					
				}
			}
		}
		file1.close();
		//update move generator range
		updateInitialNode(OthelloBoard);
		
//		for(int i=1;i<9;i++){
//			for(int j=1;j<9;j++){
//				cout<<OthelloBoard[i][j]<<" ";
//			}
//			cout<<endl;
//		}		
		
		//alpha-beta search
		AIChess(OthelloBoard);
		
	}
	LogOthelloBoard.close();
}

