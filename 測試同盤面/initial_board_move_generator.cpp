#include"lib.h"
#define Size 9
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
int black=2,white=2,player=1;
fstream file1;

int boardarr[25]={706,1569,121,1766,228,200,1634,1104,1591,1003,1677,1881,235,1110,503,1487,1508,1338,1254,266,594,644,97,349,181};

struct Node{
	int row,col,dir;
	Node():row(-1),col(-1),dir(0){}
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
};

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
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));
	string str;
	int boardNum;
	cin>>boardNum;
	for(int folder=10;folder<50;folder+=10){
		black=0;white=0;
		memset (checkerboard,0,sizeof(checkerboard));
		for(int k=1;k<Size;k++){
			for(int j=1;j<Size;j++){
				checkerboard[k][j]=8;
			}
		}
		file1.open("./board/"+to_string(folder)+"/"+to_string(boardNum)+".txt",ios::in);
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
		vector<Node> arr = findnode(checkerboard,player,sn,en);
		cout<<"folder"<<to_string(folder)<<" : ";
		for(auto i:arr){
			cout<<"( "<<i.row<<","<<i.col<<" ) ";
		}
		cout<<endl;
	}

}
