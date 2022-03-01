#include"lib.h"
#define Size 9
#define limitDep 10
using namespace std;
//8よ 
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
//璸衡丁 
double START,END;
//堵计秖フ计秖 近堵(player=1)┪フ(player=0)穓碝竊翴计秖 
int black=2,white=2,player=1,nodetotal;
fstream LogBoard,file1,file2,file3;

int Order[Size][Size]={{0,1,2,3,4,5,6,7,8},
					   {1,0,7,1,3,3,1,7,0},
					   {2,7,8,4,5,5,4,8,7},
					   {3,1,4,2,6,6,2,4,1},
					   {4,3,5,6,0,0,6,5,3},
					   {5,3,5,6,0,0,6,5,3},
					   {6,1,4,2,6,6,2,4,1},
					   {7,7,8,4,5,5,4,8,7},
					   {8,0,7,1,3,3,1,7,0}};
						  
struct Node{
	int row,col,dir,val,player;
	vector<Node> parents;
	Node *next;
	Node():row(-1),col(-1),dir(0),next(NULL){}
	Node(int r,int c):row(r),col(c),dir(0),next(NULL){}
	Node(int r,int c,int d):row(r),col(c),dir(d),next(NULL){}
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

bool checkCanPlayChess(int checkerboard[][Size],int player,Node sn,Node en){
	for(int i=sn.row;i<=en.row;i++){
		for(int j=sn.col;j<=en.col;j++){
			if(checkerboard[i][j]<0||checkerboard[i][j]==8)
				continue;
			for(int k=checkerboard[i][j];k<8;k++){
				int xStart=i+dir[k][0],yStart=j+dir[k][1];
				if(xStart==0||yStart==0||xStart==Size||yStart==Size||checkerboard[xStart][yStart]>=0||(player&&checkerboard[xStart][yStart]==-2)||(!player&&checkerboard[xStart][yStart]==-1))
					continue;
				xStart+=dir[k][0];
				yStart+=dir[k][1];
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&checkerboard[xStart][yStart]<0){
					if((player&&checkerboard[xStart][yStart]==-2)||(!player&&checkerboard[xStart][yStart]==-1))
						return true;
					xStart+=dir[k][0];
					yStart+=dir[k][1];
				}
			}
		}		
	}
	return false;
}

bool method(Node a,Node b){
	if(Order[a.row][a.col]<Order[b.row][b.col])
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

int arrayCUT[Size][Size],depthlog=3,betaArr[11];
map<int,int> score;
Node ans(0,0);
int AlphaBeta(int checkerboard[][Size],int alpha,int beta,int black,int white,int player,int depth,Node sn,Node en,bool skip,Node node){
	if(depth==limitDep||black+white==64||white==0||black==0){
//		if(score.find((player)?black-white:white-black)!=score.end()){
//			score[-1*(player)?black-white:white-black]++;
//		}
//		else{
//			score[-1*(player)?black-white:white-black]=1;
//		}
		return (player)?black-white:white-black;
	}
	else{
		vector<Node> arr = findnode(checkerboard,player,sn,en);
		int m=INT_MIN;
		if(arr.size()==0){
			if(skip)
				return (!player)?black-white:white-black;
			return -1*AlphaBeta(checkerboard,-1*beta,-1*alpha,black,white,!player,depth,sn,en,true,node);
		}
		nodetotal+=arr.size();
		if(depth<=6){
			sort(arr.begin(),arr.end(),method);			
		}
//		if(depth==8){
//			file2<<alpha<<" "<<beta<<endl;			
//		}
		for(int i=0;i<arr.size();i++){
			if(depth+1<=depthlog){
				arr[i].parents.insert(arr[i].parents.begin(),node.parents.begin(),node.parents.end());
				arr[i].parents.push_back(node);
			}
			int temp[Size][Size],tempB=black,tempW=white;
			Node tempSn=sn,tempEn=en;
//			arr[i].player=player;
			memcpy(temp,checkerboard,sizeof(temp));
			updateCheckerboard(temp,arr[i],tempB,tempW,player);
			updateNode(temp,tempSn,tempEn,arr[i].row,arr[i].col);
			int t=-1*AlphaBeta(temp,-1*beta,-1*Max(m,alpha),tempB,tempW,!player,depth+1,tempSn,tempEn,false,arr[i]);
			arr[i].val=t;
			if(t>m){
				m=t;
			}
			if(m>=beta){
//				if(depth==8){
//					file2<<alpha<<" "<<beta<<endl;			
//				}
//				if(m>beta){
//					betaArr[depth+1]++;
//				}
				if(depth+1<=depthlog){
					file2<<"        ";
					for(auto i:node.parents){
						if(i.row==0)
							continue;
						file2<<"          ("<<i.row<<","<<i.col<<",";
						if(i.player)
							file2<<"〈"<<")";
						else
							file2<<"〕"<<")";
					}
					file2<<"          ("<<node.row<<","<<node.col<<",";
					if(node.player)
						file2<<"〈"<<")"<<endl;
					else
						file2<<"〕"<<")"<<endl;
					file2<<"parent :";
					for(int i=0;i<node.parents.size();i++){
						file2<<" depth:"<<i<<" -------->";
					}
					file2<<" depth:"<<node.parents.size()<<endl;
					file2<<"[alpha,beta] : ["<<alpha<<","<<beta<<"]"<<endl;
					file2<<"depth : "<<depth+1;
					if(player)
						file2<<" 〈"<<endl;
					else
						file2<<" 〕"<<endl;
					file2<<"child:";
					/*print Total node*/ 
					for(int j=0;j<arr.size();j++){
						if(j>i)
							file2<<"("<<arr[j].row<<","<<arr[j].col<<",CUT)";
						else
							file2<<"("<<arr[j].row<<","<<arr[j].col<<","<<arr[j].val<<")";
						if(j!=arr.size()-1)
							file2<<" , ";
						else
							file2<<endl<<endl;	
					}
					/*print Total node*/ 					
				}
//				if(depth==0){
//					if(score.find(m)!=score.end()){
//						score[-1*m]++;
//					}
//					else{
//						score[-1*m]=1;
//					}			
//				}
				return m;
			}				
		}
//		/*print Total node*/ 
//		if(depth+1<=depthlog){
//			file2<<"        ";
//			for(auto i:node.parents){
//				if(i.row==0)
//					continue;
//				file2<<"          ("<<i.row<<","<<i.col<<",";
//				if(i.player)
//					file2<<"〈"<<")";
//				else
//					file2<<"〕"<<")";
//			}
//			file2<<"          ("<<node.row<<","<<node.col<<",";
//			if(node.player)
//				file2<<"〈"<<")"<<endl;
//			else
//				file2<<"〕"<<")"<<endl;
//			file2<<"parent :";
//			for(int i=0;i<node.parents.size();i++){
//				file2<<" depth:"<<i<<" -------->";
//			}
//			file2<<" depth:"<<node.parents.size()<<endl;
//			file2<<"[alpha,beta] : ["<<alpha<<","<<beta<<"]"<<endl;
//			file2<<"depth : "<<depth+1;
//			if(player)
//				file2<<" 〈"<<endl;
//			else
//				file2<<" 〕"<<endl;
//			file2<<"child:";
//			for(int i=0;i<arr.size();i++){
//				file2<<"("<<arr[i].row<<","<<arr[i].col<<","<<arr[i].val<<")";
//				if(i!=arr.size()-1)
//					file2<<" , ";
//				else
//					file2<<endl<<endl;	
//			}	
//		}
//		/*print Total node*/ 
//		if(depth==0){
//			if(score.find(m)!=score.end()){
//				score[-1*m]++;
//			}
//			else{
//				score[-1*m]=1;
//			}			
//		}

		return m;
	}
}

void AIChess(int checkerboard[][Size]){
	nodetotal = 0;
	int temp[Size][Size];
	memcpy(temp,checkerboard,sizeof(temp));
	memset(betaArr,0,sizeof(betaArr));
	START = clock();
	Node node(0,0);
	node.player=player;
	AlphaBeta(temp,-1000,1000,black,white,player,0,sn,en,false,node);
	END = clock();
//	for(auto i:score){
//		file3<<i.first<<","<<i.second<<endl;
//	}
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
	LogBoard.open("./2.1_Alpha-Beta.csv",ios::out);
//	LogBoard.open("./"+to_string(depthSort)+".csv",ios::out);
	LogBoard<<"depth 10 :"<<endl;
	LogBoard<<"board,Time,Node"<<endl;
	file2.open("./CUTlog.txt",ios::out);
	file3.open("./beta.txt",ios::out);
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));
	memset (arrayCUT,0,sizeof(arrayCUT));
	string str;
	for(int i=869;i<=869;i++){
		black=0;white=0;
		memset (checkerboard,0,sizeof(checkerboard));
		for(int k=1;k<Size;k++){
			for(int j=1;j<Size;j++){
				checkerboard[k][j]=8;
			}
		}
//		file1.open("./othello-initial board-2000/"+to_string(i)+".txt",ios::in);
		file1.open("./New-2000/"+to_string(i)+".txt",ios::in);
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
		LogBoard<<i<<","<<fixed<<setprecision(2)<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<",";
//		for(int i=0;i<11;i++)
//			LogBoard<<betaArr[i]<<",";
		LogBoard<<endl;
	}

//	file2<<",1,2,3,4,5,6,7,8"<<endl;
//	for(int i=1;i<Size;i++){
//		for(int j=0;j<Size;j++){
//			if(j==0)
//				file2<<i;
//			else
//				file2<<arrayCUT[i][j];
//			file2<<",";
//		}
//		file2<<endl;
//	}
	file3.close();
	file2.close();
	LogBoard.close();
}

