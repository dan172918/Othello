#include"lib.h"
#define Size 9
#define limitDep 10
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1,nodetotal;
fstream LogCheckerboard,file1,file2;
//int Priority[Size][Size]={{0,0,0,0,0,0,0,0,0},
//						  {0,0,4,1,1,1,1,4,0},
//						  {0,4,4,2,2,2,2,4,4},
//						  {0,1,2,3,3,3,3,2,1},
//					 	  {0,1,2,3,3,3,3,2,1},
//						  {0,1,2,3,3,3,3,2,1},
//						  {0,1,2,3,3,3,3,2,1},
//						  {0,4,4,2,2,2,2,4,4},
//						  {0,0,4,1,1,1,1,4,0}};
//int Priority[Size][Size]={{0,0,0,0,0,0,0,0,0},
//						  {0,0,4,3,3,3,3,4,0},
//						  {0,4,4,2,2,2,2,4,4},
//						  {0,3,2,1,1,1,1,2,3},
//					 	  {0,3,2,1,1,1,1,2,3},
//						  {0,3,2,1,1,1,1,2,3},
//						  {0,3,2,1,1,1,1,2,3},
//						  {0,4,4,2,2,2,2,4,4},
//						  {0,0,4,3,3,3,3,4,0}};
int Priority[Size][Size]={{0,1,2,3,4,5,6,7,8},
						  {1,0,3,2,2,2,2,3,0},
						  {2,3,4,1,1,1,1,4,3},
						  {3,2,1,1,1,1,1,1,2},
					 	  {4,2,1,1,1,1,1,1,2},
						  {5,2,1,1,1,1,1,1,2},
						  {6,2,1,1,1,1,1,1,2},
						  {7,3,4,1,1,1,1,4,3},
						  {8,0,3,2,2,2,2,3,0}};
						  
struct Node{
	int row,col,dir;
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
};

int Max(int a,int b){
	return (a>b)?a:b;
}

Node sn((Size-1)/2-1,(Size-1)/2-1),en((Size-1)/2+2,(Size-1)/2+2); 
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

void updateCheckerboard(int checkerboard[][Size],Node node,int &black,int &white,int player){
	int xStart,yStart;
	for(int i=node.dir;i<8;i++){
		xStart=node.row+dir[i][0];yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size||(player&&checkerboard[xStart][yStart]==-2)||(!player&&checkerboard[xStart][yStart]==-1))
			continue;
		if(checkerboard[xStart][yStart]>=0){
			checkerboard[xStart][yStart]=(checkerboard[xStart][yStart]>(i+4)%8)?(i+4)%8:checkerboard[xStart][yStart];
			continue;
		}
		bool findplayerColor=false;
		xStart+=dir[i][0];
		yStart+=dir[i][1];
		while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&checkerboard[xStart][yStart]<0){
			if((player&&checkerboard[xStart][yStart]==-2)||(!player&&checkerboard[xStart][yStart]==-1)){
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
				checkerboard[xStart][yStart]=-2;
			}
			else if(!player&&checkerboard[xStart][yStart]==-2){
				white++;
				black--;
				checkerboard[xStart][yStart]=-1;
			}
		}

	}
	if(player){
		checkerboard[node.row][node.col]=-2;
		black++;
	}
	else{
		checkerboard[node.row][node.col]=-1;
		white++;
	}
	checkerboard[node.row][0]++;
	checkerboard[0][node.col]++;
	for(int i=0;i<node.dir;i++){
		xStart=node.row+dir[i][0];yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size)
			continue;
		if(checkerboard[xStart][yStart]>=0)
			checkerboard[xStart][yStart]=(checkerboard[xStart][yStart]>(i+4)%8)?(i+4)%8:checkerboard[xStart][yStart];		
	}
}

bool method(Node a,Node b){
	if(Priority[a.row][a.col]<Priority[b.row][b.col])
		return true;
	if(Priority[a.row][a.col]==Priority[b.row][b.col]){
		if(a.row<b.row)
			return true;
		if(a.row==b.row){
			if(a.col<b.col)
				return true;
		}
	}
	return false;
}

vector<Node> findnode(int checkerboard[][Size],int player,Node sn,Node en){
	vector<Node> arr;
	for(int i=sn.row;i<=en.row;i++){
		for(int j=sn.col;j<=en.col;j++){
			bool find = false;
			if(checkerboard[i][j]<0||checkerboard[i][j]==8)
				continue;
			for(int k=checkerboard[i][j];k<8;k++){
				int xStart=i+dir[k][0],yStart=j+dir[k][1];
				if(xStart==0||yStart==0||xStart==Size||yStart==Size||checkerboard[xStart][yStart]>=0||(player&&checkerboard[xStart][yStart]==-2)||(!player&&checkerboard[xStart][yStart]==-1))
					continue;
				xStart+=dir[k][0];
				yStart+=dir[k][1];
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&checkerboard[xStart][yStart]<0){
					if((player&&checkerboard[xStart][yStart]==-2)||(!player&&checkerboard[xStart][yStart]==-1)){
//						Node *node = new Node(i,j,k);
//						arr.push_back(node);
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
int dep123=10;
int cutarr[Size][Size];
int cut[limitDep],total[limitDep];
int Scout(int checkerboard[][Size],int alpha,int beta,int black,int white,int player,int depth,Node sn,Node en,bool skip){
	if(depth==0||black+white==64||white==0||black==0){
		return (player)?black-white:white-black;
	}
	else{
		vector<Node> arr = findnode(checkerboard,player,sn,en);
		if(arr.size()==0){
			if(skip)
				return (!player)?black-white:white-black;
			return -1*Scout(checkerboard,-1*beta,-1*alpha,black,white,!player,depth,sn,en,true);
		}
		nodetotal+=arr.size();
//		if(depth>dep123)
			sort(arr.begin(),arr.end(),method);
		total[depth-1]+=arr.size();
		int m=-1000,n=beta;
		for(int i=0;i<arr.size();i++){
			int temp[Size][Size],tempB=black,tempW=white;
			Node tempSn=sn,tempEn=en;
			memcpy(temp,checkerboard,sizeof(temp));
			updateCheckerboard(temp,arr[i],tempB,tempW,player);
			updateNode(temp,tempSn,tempEn,arr[i].row,arr[i].col);
			int	t=-1*Scout(temp,-1*n,-1*Max(alpha,m),tempB,tempW,!player,depth-1,tempSn,tempEn,false);
			if(t>m){
				if(n==beta||depth<3||t>=beta){
					m=t;					
				}
				else{
					m=-1*Scout(temp,-1*beta,-1*t,tempB,tempW,!player,depth-1,tempSn,tempEn,false);				
				}
			}
			if(m>=beta){
				cut[depth-1]+=arr.size()-i-1;
//				if(depth-1==8)
//					cutarr[arr[i].row][arr[i].col]+=arr.size()-i-1;
				return m;
			}
			n=Max(alpha,m)+1;
		}
		return m;
	}
}

void AIChess(int checkerboard[][Size]){
	nodetotal = 0;
	int temp[Size][Size];
	memcpy(temp,checkerboard,sizeof(temp));
	memset(cut,0,sizeof(cut));
	memset(total,0,sizeof(total));
	START = clock();
	Scout(temp,-1000,1000,black,white,player,limitDep,sn,en,false);
	END = clock();
	LogCheckerboard<<fixed<<setprecision(2)<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<",";
	for(int i=limitDep-1;i>=0;i--){
		file2<<"depth,"<<i<<endl;
		file2<<"Total,"<<total[i]<<endl;
		file2<<"Cut,"<<cut[i]<<endl;
		file2<<"Total - Cut,"<<total[i]-cut[i]<<endl;
	}
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
	LogCheckerboard.open("./"+to_string(dep123)+".csv",ios::out);
	LogCheckerboard<<"2.1"<<endl;
	LogCheckerboard<<"depth 10 :"<<endl;
	LogCheckerboard<<"Time,Node"<<endl;
	file2.open("./2.3_negascout_cutNum.csv",ios::out);
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));
	memset(cutarr,0,sizeof(cutarr));
	string str;
	for(int i=40;i<=40;i++){
		black=0;white=0;
		memset (checkerboard,0,sizeof(checkerboard));
		for(int i=1;i<Size;i++){
			for(int j=1;j<Size;j++){
				checkerboard[i][j]=8;
			}
		}
		file1.open("./othello-initial board-2000/"+to_string(i)+".txt",ios::in);
		file1>>str;
		player = (str=="o")?1:0;
		for(int j=1;j<Size;j++){
			file1>>str;
			for(int k=1;k<Size;k++){
				if(str[k-1]!='_'){
					checkerboard[j][0]++;
					checkerboard[0][k]++;
					if(str[k-1]=='o'){
						checkerboard[j][k]=-2;
						black++;
					}
					else{
						checkerboard[j][k]=-1;
						white++;
					}
				}
			}
		}
		for(int a=1;a<Size;a++){
			for(int j=1;j<Size;j++){
				if(checkerboard[a][j]<0){
					for(int k=0;k<8;k++){
						if(a+dir[k][0]>0&&a+dir[k][0]<Size&&j+dir[k][1]>0&&j+dir[k][1]<Size&&checkerboard[a+dir[k][0]][j+dir[k][1]]>=0)
							checkerboard[a+dir[k][0]][j+dir[k][1]]=(checkerboard[a+dir[k][0]][j+dir[k][1]]>(k+4)%8)?(k+4)%8:checkerboard[a+dir[k][0]][j+dir[k][1]];
					}					
				}
			}
		}
		file1.close();
		updateInitialNode(checkerboard);
		AIChess(checkerboard);
		LogCheckerboard<<endl;
	}
//	file2<<",";
//	for(int i=0;i<Size;i++){
//		for(int j=1;j<Size;j++){
//			if(i==0)
//				file2<<j<<",";
//			else
//				file2<<cutarr[i][j]<<",";
//		}
//		if(i!=Size-1)
//			file2<<endl<<i+1<<",";			
//	}
	file2.close();
	LogCheckerboard.close();
}

