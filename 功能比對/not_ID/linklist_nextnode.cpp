#include"lib.h"
#define Size 9
#define limitDep 12
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
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

void updateOthelloBoard(int OthelloBoard[][Size],Node node,int &black,int &white,int player,int linkNode[66]){
	int xStart,yStart;
	int left=linkNode[(node.row-1)*8+node.col]/100,right=linkNode[(node.row-1)*8+node.col]%100;
	linkNode[left]=linkNode[left]/100*100+right;
	linkNode[right]=linkNode[right]%100+left*100;
	for(int i=node.dir;i<8;i++){
		xStart=node.row+dir[i][0];yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size||(player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1))
			continue;
		if(OthelloBoard[xStart][yStart]>=0){
			if(OthelloBoard[xStart][yStart]==8){
				linkNode[linkNode[65]/100] = linkNode[linkNode[65]/100]-65 + ( (xStart-1)*8+(yStart) );
				linkNode[(xStart-1)*8+(yStart)]= linkNode[65]+65;
				linkNode[65]=( (xStart-1)*8+(yStart) )*100;
			}
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
	for(int i=0;i<node.dir;i++){
		xStart=node.row+dir[i][0];yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size)
			continue;
		if(OthelloBoard[xStart][yStart]>=0){
			if(OthelloBoard[xStart][yStart]==8){
				linkNode[linkNode[65]/100] = linkNode[linkNode[65]/100]-65 + ( (xStart-1)*8+(yStart) );
				linkNode[(xStart-1)*8+(yStart)]= linkNode[65]+65;
				linkNode[65]=( (xStart-1)*8+(yStart) )*100;
			}
			OthelloBoard[xStart][yStart]=(OthelloBoard[xStart][yStart]>(i+4)%8)?(i+4)%8:OthelloBoard[xStart][yStart];		
		}
	}
}

bool method(Node a,Node b){
	if(Priority[a.row][a.col]<Priority[b.row][b.col])
		return true;
	if(Priority[a.row][a.col]==Priority[b.row][b.col]){
		if(a.row<b.row)
			return true;
		else if(a.row==b.row){
			if(a.col<b.col)
				return true;
		}
	}
	return false;
}

vector<Node> findnode(int OthelloBoard[][Size],int player,int linkNode[66]){
	vector<Node> arr;
	for(int i=linkNode[0];i!=65;i=linkNode[i]%100){
		bool find = false;
		int row=(i-1)/8+1,col=(i-1)%8+1;
		for(int k=OthelloBoard[row][col];k<8;k++){
			int xStart=row+dir[k][0],yStart=col+dir[k][1];
			if(xStart==0||yStart==0||xStart==Size||yStart==Size||OthelloBoard[xStart][yStart]>=0||(player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1))
				continue;
			xStart+=dir[k][0];
			yStart+=dir[k][1];
			while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&OthelloBoard[xStart][yStart]<0){
				if((player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1)){
					arr.push_back(Node(row,col,k));
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
	return arr;
}

int AlphaBeta(int OthelloBoard[][Size],int alpha,int beta,int black,int white,int player,int depth,bool skip,int linkNode[66]){
	if(depth==limitDep||black+white==64||white==0||black==0){
		return (player)?black-white:white-black;
	}
	else{
		vector<Node> arr = findnode(OthelloBoard,player,linkNode);
		int m=INT_MIN;
		if(arr.size()==0){
			if(skip)
				return (!player)?black-white:white-black;
			return -1*AlphaBeta(OthelloBoard,-1*beta,-1*alpha,black,white,!player,depth,true,linkNode);
		}

		nodetotal+=arr.size();
//		if(depth<8){
			sort(arr.begin(),arr.end(),method);			
//		}

		for(int i=0;i<arr.size();i++){
			int temp[Size][Size],tempNext[66],tempB=black,tempW=white;
			memcpy(temp,OthelloBoard,sizeof(temp));
			memcpy(tempNext,linkNode,sizeof(tempNext));			
			updateOthelloBoard(temp,arr[i],tempB,tempW,player,tempNext);
			int t=-1*AlphaBeta(temp,-1*beta,-1*Max(m,alpha),tempB,tempW,!player,depth+1,false,tempNext);
			if(t>m){
				m=t;
			}
			if(m>=beta){
				return m;
			}				
		}
		return m;
	}
}

void AIChess(int OthelloBoard[][Size],int linkNode[66]){
	nodetotal = 0;
	int temp[Size][Size];
	memcpy(temp,OthelloBoard,sizeof(temp));
	START = clock();
	AlphaBeta(temp,-10000,10000,black,white,player,0,false,linkNode);
	END = clock();
}

int main(){
	LogOthelloBoard.open("./зя1_Alpha-Beta.csv",ios::out);
	LogOthelloBoard<<"depth "+to_string(limitDep)+" :"<<endl;
	LogOthelloBoard<<"board,Time,Node"<<endl;
	int OthelloBoard[Size][Size];
	int linkNode[66];
	memset (OthelloBoard,0,sizeof(OthelloBoard));
	memset (linkNode,0,sizeof(linkNode));
	linkNode[0]=65;
	string str;
	for(int i=1;i<=10;i++){
		//initial board
		black=0;white=0;
		memset (OthelloBoard,0,sizeof(OthelloBoard));
		memset (linkNode,0,sizeof(linkNode));
		linkNode[0]=65;
		for(int j=1;j<Size;j++){
			for(int k=1;k<Size;k++){
				OthelloBoard[j][k]=8;
			}
		}

		//file loading initial board
		file1.open("./board/"+to_string(i)+".txt",ios::in);
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
							if(OthelloBoard[a+dir[k][0]][j+dir[k][1]]==8){
								linkNode[linkNode[65]/100] = linkNode[linkNode[65]/100]-65 + ( (a+dir[k][0]-1)*8+(j+dir[k][1]) );
								linkNode[(a+dir[k][0]-1)*8+(j+dir[k][1])]= linkNode[65]+65;
								linkNode[65]=( (a+dir[k][0]-1)*8+(j+dir[k][1]) )*100;
							}
							OthelloBoard[a+dir[k][0]][j+dir[k][1]]=(OthelloBoard[a+dir[k][0]][j+dir[k][1]]>(k+4)%8)?(k+4)%8:OthelloBoard[a+dir[k][0]][j+dir[k][1]];
					}					
				}
			}
		}
		file1.close();
//		for(int i=linkNode[0];i!=65;i=linkNode[i]%100){
//			cout<<(i-1)/8+1<<","<<(i-1)%8+1<<endl;
//			if(linkNode[i]/100!=0)
//				cout<<"left : "<<(linkNode[i]/100-1)/8+1<<","<<(linkNode[i]/100-1)%8+1<<endl;
//			else
//				cout<<"left : start"<<endl;
//			if(linkNode[i]%100!=65)
//				cout<<"right : "<<(linkNode[i]%100-1)/8+1<<","<<(linkNode[i]%100-1)%8+1<<endl;
//			else
//				cout<<"right : end"<<endl;
//			
//		}
//		cout<<endl;cout<<endl;
//		cout<<linkNode[65];
//		cout<<endl;cout<<endl;
//		_sleep(1000000);
		//alpha-beta search
		AIChess(OthelloBoard,linkNode);
		//print cost times and nodes
		LogOthelloBoard<<i<<","<<fixed<<setprecision(2)<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<","<<endl;
	}
	LogOthelloBoard.close();
}

