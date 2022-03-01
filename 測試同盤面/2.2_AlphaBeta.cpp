#include"lib.h"
#define Size 9
#define limitDep 12 
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
unsigned long long int nodetotal;
fstream LogCheckerboard,file1,randomboard,heatMap,initial;
long long int heatMapArr[Size][Size]={{0,1,2,3,4,5,6,7,8},
									  {1,0,0,0,0,0,0,0,0},
									  {2,0,0,0,0,0,0,0,0},
									  {3,0,0,0,0,0,0,0,0},
								 	  {4,0,0,0,0,0,0,0,0},
									  {5,0,0,0,0,0,0,0,0},
									  {6,0,0,0,0,0,0,0,0},
									  {7,0,0,0,0,0,0,0,0},
									  {8,0,0,0,0,0,0,0,0}};
									  
long long int initialArr[Size][Size]={{0,1,2,3,4,5,6,7,8},
									  {1,0,0,0,0,0,0,0,0},
									  {2,0,0,0,0,0,0,0,0},
									  {3,0,0,0,0,0,0,0,0},
								 	  {4,0,0,0,0,0,0,0,0},
									  {5,0,0,0,0,0,0,0,0},
									  {6,0,0,0,0,0,0,0,0},
									  {7,0,0,0,0,0,0,0,0},
									  {8,0,0,0,0,0,0,0,0}};									  
//int Priority[Size][Size]={{0,1,2,3,4,5,6,7,8},
//						  {1,0,7,1,0,0,1,7,0},
//						  {2,7,8,0,0,0,0,8,7},
//						  {3,1,0,2,0,0,2,0,1},
//					 	  {4,0,0,0,0,0,0,0,0},
//						  {5,0,0,0,0,0,0,0,0},
//						  {6,1,0,2,0,0,2,0,1},
//						  {7,7,8,0,0,0,0,8,7},
//						  {8,0,7,1,0,0,1,7,0}};
						  
//int Priority[Size][Size]={{0,1,2,3,4,5,6,7,8},
//						  {1,0,0,1,3,3,1,0,0},
//						  {2,0,0,4,5,5,4,0,0},
//						  {3,1,4,0,6,6,0,4,1},
//					 	  {4,3,5,6,0,0,6,5,3},
//						  {5,3,5,6,0,0,6,5,3},
//						  {6,1,4,0,6,6,0,4,1},
//						  {7,0,0,4,5,5,4,0,0},
//						  {8,0,0,1,3,3,1,0,0}};

int Priority[Size][Size]={{0,1,2,3,4,5,6,7,8},
						  {1,0,0,1,0,0,1,0,0},
						  {2,0,0,0,0,0,0,0,0},
						  {3,1,0,0,0,0,0,0,1},
					 	  {4,0,0,0,0,0,0,0,0},
						  {5,0,0,0,0,0,0,0,0},
						  {6,1,0,0,0,0,0,0,1},
						  {7,0,0,0,0,0,0,0,0},
						  {8,0,0,1,0,0,1,0,0}};
						  
//int Priority[Size][Size]={{0,1,2,3,4,5,6,7,8},
//						  {1,0,1,2,3,3,2,1,0},
//						  {2,1,7,4,5,5,4,7,1},
//						  {3,2,4,8,6,6,8,4,2},
//					 	  {4,3,5,6,0,0,6,5,3},
//						  {5,3,5,6,0,0,6,5,3},
//						  {6,2,4,8,6,6,8,4,2},
//						  {7,1,7,4,5,5,4,7,1},
//						  {8,0,1,2,3,3,2,1,0}};

//int Priority[Size][Size]={{0,1,2,3,4,5,6,7,8},
//						  {1,0,7,1,3,3,1,7,0},
//						  {2,7,8,4,5,5,4,8,7},
//						  {3,1,4,2,6,6,2,4,1},
//					 	  {4,3,5,6,0,0,6,5,3},
//						  {5,3,5,6,0,0,6,5,3},
//						  {6,1,4,2,6,6,2,4,1},
//						  {7,7,8,4,5,5,4,8,7},
//						  {8,0,7,1,3,3,1,7,0}};
						  
//int scoreBBB[Size][Size]={{0,1,2,3,4,5,6,7,8},
//						  {1,30,-10,1,1,1,1,-10,30},
//						  {2,-10,-20,1,1,1,1,-20,-10},
//						  {3,1,1,1,1,1,1,1,1},
//					 	  {4,1,1,1,0,0,1,1,1},
//						  {5,1,1,1,0,0,1,1,1},
//						  {6,1,1,1,1,1,1,1,1},
//						  {7,-10,-20,1,1,1,1,-20,-10},
//						  {8,30,-10,1,1,1,1,-10,30}};
						  
struct Node{
	int row,col,dir,val,player;
	Node():row(-1),col(-1),dir(0){}
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
//				white-=scoreBBB[xStart][yStart];
//				black+=scoreBBB[xStart][yStart];
				white--;
				black++;
				checkerboard[xStart][yStart]=-2;
			}
			else if(!player&&checkerboard[xStart][yStart]==-2){
//				white+=scoreBBB[xStart][yStart];
//				black-=scoreBBB[xStart][yStart];
				white++;
				black--;
				checkerboard[xStart][yStart]=-1;
			}
		}

	}
	if(player){
		checkerboard[node.row][node.col]=-2;
//		black+=scoreBBB[node.row][node.col];
		black++;
	}
	else{
		checkerboard[node.row][node.col]=-1;
//		white+=scoreBBB[node.row][node.col];
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

int AlphaBeta(int checkerboard[][Size],int alpha,int beta,int black,int white,int player,int depth,Node sn,Node en,bool skip){
	if(depth==limitDep||black+white==64||white==0||black==0){
//	if(depth==limitDep){
		return (player)?black-white:white-black;
	}
	else{
		vector<Node> arr = findnode(checkerboard,player,sn,en);
		int m=INT_MIN;
		if(arr.size()==0){
			if(skip)
				return (!player)?black-white:white-black;
			return -1*AlphaBeta(checkerboard,-1*beta,-1*alpha,black,white,!player,depth,sn,en,true);
		}
		nodetotal+=arr.size();
		if(depth<6){
			sort(arr.begin(),arr.end(),method);			
		}
		for(int i=0;i<arr.size();i++){
//			heatMapArr[arr[i].row][arr[i].col]++;
			int temp[Size][Size],tempB=black,tempW=white;
			Node tempSn=sn,tempEn=en;
			memcpy(temp,checkerboard,sizeof(temp));
			updateCheckerboard(temp,arr[i],tempB,tempW,player);
			updateNode(temp,tempSn,tempEn,arr[i].row,arr[i].col);
			int t=-1*AlphaBeta(temp,-1*beta,-1*Max(m,alpha),tempB,tempW,!player,depth+1,tempSn,tempEn,false);
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

void AIChess(int checkerboard[][Size]){
	nodetotal = 0;
	int temp[Size][Size];
	memcpy(temp,checkerboard,sizeof(temp));
	START = clock();
	AlphaBeta(temp,-10000,10000,black,white,player,0,sn,en,false);
	END = clock();
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
	srand(time(0));
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));
	string str;
	
	vector<Node> updateNode1({Node(3,2),Node(4,1),Node(4,2),Node(4,3)});
	vector<Node> updateNode({Node(2,1),Node(2,2),Node(3,3)});
//	heatMap.open("./heatMap.csv",ios::out);
//	initial.open("./initialheatMap.csv",ios::out);

	
//	int Permutation[14]={4356,5346,4365,6345,4335,4345,4355,5345,3455,3344,4334,4344,3334,3333};
//	int Permutation[6]={4356,4355,4365,4345,5346,3344};
//	int Permutation[1]={4356};
	int Permutation[6]={278,287,728,782,827,872};
	
	vector<int> boardrandom({356,252,1127,1870,610,331,103,1644,1377,1957,1201,1923,1768,1704,591,153,440,1649,757,926});
//	vector<int> boardrandom({319,597,1958,1432,1807,1634,1347,1151,1103,1908,262,404,1631,205,1377,1019,559,809,1318,1106,618,156,1750,178,1794,1977,1547,1380,701,389,580,646,1105,1467,1183,1038,620,32,908,271,1399,138,1982,410,1339,1549,821,246,1941,1334});
//	randomboard.open("./7!/randomboard.txt",ios::out);
//	for(int ii=0;ii<20;ii++){
//		int testNum=rand()%2000+1;
//		if(find(boardrandom.begin(),boardrandom.end(),testNum)==boardrandom.end()){
//			randomboard<<ii+1<<" : "<<testNum<<endl;
//			boardrandom.push_back(testNum);
//		}
//		else{
//			ii--;
//		}	
//	}
//	randomboard.close();
	
	for(int count=1;count<6;count++){
		int num=100,tempA=Permutation[count];
		/*many Priority*/
		for(int i=0;i<updateNode.size();i++){
			for(int j=0;j<4;j++){
				Priority[updateNode[i].row][updateNode[i].col]=Permutation[count]/num;
				swap(updateNode[i].row,updateNode[i].col);
				Priority[updateNode[i].row][updateNode[i].col]=Permutation[count]/num;
				updateNode[i].col=Size-updateNode[i].col;
			}
			Permutation[count]%=num;
			num/=10;
		}
		Permutation[count]=tempA;
		int Permutation1[24]={3456,3465,3546,3564,3645,3654,4356,4365,4536,4563,4635,4653,5346,5364,5436,5463,5634,5643,6345,6354,6435,6453,6534,6543}; 
		/*many Priority*/
		for(int coot=0;coot<24;coot++){
			int num1=1000,tempB=Permutation1[coot];
			/*many Priority*/
			for(int i=0;i<updateNode1.size();i++){
				for(int j=0;j<4;j++){
					Priority[updateNode1[i].row][updateNode1[i].col]=Permutation1[coot]/num1;
					swap(updateNode1[i].row,updateNode1[i].col);
					Priority[updateNode1[i].row][updateNode1[i].col]=Permutation1[coot]/num1;
					updateNode1[i].col=Size-updateNode1[i].col;
				}
				Permutation1[coot]%=num1;
				num1/=10;
			}
			Permutation1[coot]=tempB;
			/*many Priority*/
	//		LogCheckerboard.open("./Priority/"+to_string(Permutation[count])+".csv",ios::out);
			LogCheckerboard.open("./7!/"+to_string(Permutation[count])+"/"+to_string(Permutation1[coot])+".csv",ios::out);
			LogCheckerboard<<Permutation1[coot]<<endl;
			LogCheckerboard<<"Time,Node"<<endl;
	
			/*one Priority*/	
			for(int i=0;i<20;i++){
				black=0;white=0;
				memset (checkerboard,0,sizeof(checkerboard));
				for(int k=1;k<Size;k++){
					for(int j=1;j<Size;j++){
						checkerboard[k][j]=8;
					}
				}
				
				file1.open("./othello-initial board-2000/"+to_string(boardrandom[i])+".txt",ios::in);
	
				file1>>str;
				player = (str=="o")?1:0;
				for(int j=1;j<Size;j++){
					file1>>str;
					for(int k=1;k<Size;k++){
						if(str[k-1]!='_'){
							checkerboard[j][0]++;
							checkerboard[0][k]++;
	//						initialArr[j][k]++;
							if(str[k-1]=='o'){
								checkerboard[j][k]=-2;
	//							black+=scoreBBB[j][k];
								black++;
							}
							else{
								checkerboard[j][k]=-1;
	//							white+=scoreBBB[j][k];
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
				LogCheckerboard<<fixed<<setprecision(2)<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<","<<endl;
			}
			LogCheckerboard.close();
			/*one Priority*/							
		}

	}
//	for(int i=0;i<Size;i++){
//		for(int j=0;j<Size;j++){
//			heatMap<<heatMapArr[i][j]<<",";
//			initial<<initialArr[i][j]<<",";
//		}
//		heatMap<<endl;
//		initial<<endl;
//	}
//	heatMap.close();
//	initial.close();
}

