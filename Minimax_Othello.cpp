#include<iostream>
#include<vector>
#include <conio.h>
#include<climits>
#include<algorithm>
#include<ctime>
#include<fstream>
#define Size 9
#define limitDep 11 
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int nodetotal = 0;
fstream file1,file2,log,LogCheckerboard;

struct Node{
	int row,col;
	Node(int r,int c):row(r),col(c){}
};
 
void printCheckerboard(int checkerboard[][Size],int black,int white,int player){
	system("cls");
	cout<<"    ";
	LogCheckerboard<<"    ";
	for(int i=0;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(i==0){
				cout<<j<<"  ";
				LogCheckerboard<<j<<"  ";	
			}
			else{
				cout<<" ";
				LogCheckerboard<<" ";
				if(checkerboard[i][j]==0){
					cout<<"-";
					LogCheckerboard<<"-";	
				}
				else if(checkerboard[i][j]==1){
					cout<<"O";
					LogCheckerboard<<"O";
				}
				else{
					cout<<"X";
					LogCheckerboard<<"X";	
				}
				cout<<" ";
				LogCheckerboard<<" ";
			}	
		}
		if(i!=8){
			cout<<"\n\n "<<i+1<<" ";
			LogCheckerboard<<"\n\n "<<i+1<<" ";
		}
			
	}
	cout<<"\n O : "<<black<<"   X : "<<white;
	LogCheckerboard<<"\n O : "<<black<<"   X : "<<white;
	if(player){
		cout<<"\t->player(O)\tAI(X)"<<endl;	
		LogCheckerboard<<"\t->player(O)\tAI(X)"<<endl;	
	}
	else{
		cout<<"\tplayer(O)\t->AI(X)"<<endl;	
		LogCheckerboard<<"\tplayer(O)\t->AI(X)"<<endl;	
	}
	cout<<"Node : "<<nodetotal<<endl;
	cout<<"Time : "<<(END - START)/CLOCKS_PER_SEC<<endl;
	LogCheckerboard<<endl;
}

bool updateCheckerboard(int checkerboard[][Size],int row,int col,int &black,int &white,int player){
	int initBlack = black, initwhite = white;
	for(int i=0;i<8;i++){
		if(checkerboard[row+dir[i][0]][col+dir[i][1]]==0||(player==1&&checkerboard[row+dir[i][0]][col+dir[i][1]]==1)||(player==0&&checkerboard[row+dir[i][0]][col+dir[i][1]]==-1))
			continue;
		bool findplayerColor=false;
		int xStart=row,yStart=col;
		while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size){
			if((player==1&&checkerboard[xStart][yStart]==1)||(player==0&&checkerboard[xStart][yStart]==-1)){
				findplayerColor=true;
				break;
			}
			xStart+=dir[i][0];
			yStart+=dir[i][1];
			if(checkerboard[xStart][yStart]==0)
				break;
		}
		while(findplayerColor){
			xStart-=dir[i][0];
			yStart-=dir[i][1];
			if(xStart==row&&yStart==col)
				break;
			if(player==1&&checkerboard[xStart][yStart]==-1){
				white--;
				black++;
				checkerboard[xStart][yStart]=1;
			}
			else if(player==0&&checkerboard[xStart][yStart]==1){
				white++;
				black--;
				checkerboard[xStart][yStart]=-1;
			}

		}
	}
	if(initBlack==black&&initwhite==white)
		return false;
	if(player==1){
		checkerboard[row][col]=1;
		black++;
	}
	else{
		checkerboard[row][col]=-1;
		white++;
	}
	return true;
}

bool checkCanPlayChess(int checkerboard[][Size],int player){
	for(int i=0;i<Size;i++){
		for(int j=0;j<Size;j++){
			if(checkerboard[i][j]!=0)
				continue;
			for(int k=0;k<8;k++){
				if(checkerboard[i+dir[k][0]][j+dir[k][1]]==0||(player==1&&checkerboard[i+dir[k][0]][j+dir[k][1]]==1)||(player==0&&checkerboard[i+dir[k][0]][j+dir[k][1]]==-1))
					continue;
				int xStart=i,yStart=j;
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size){
					if((player==1&&checkerboard[xStart][yStart]==1)||(player==0&&checkerboard[xStart][yStart]==0))
						return true;
					xStart+=dir[k][0];
					yStart+=dir[k][1];
					if(checkerboard[xStart][yStart]==0)
						break;
				}
			}
		}		
	}
	return false;
}

void playerChess(int checkerboard[][Size],int &black,int &white,int &player){
	if(!checkCanPlayChess(checkerboard,player)){
		player = 0;
		return ;
	}
	int row,col;
	char confirm;
	while(1){
		cout<<"\nrow(¦C) : "; 
		cin>>row;
		cout<<"col(¦æ) : ";
		cin>>col;
		if(row<0||row>8||col<0||col>8)
			continue;
		while(1){
			bool Invalid;
			if(checkerboard[row][col]==0){
				Invalid = updateCheckerboard(checkerboard,row,col,black,white,player);
				if(!Invalid){
					cout<<"Invalid input\n";
					_sleep(2*1000);
					break;
				}
				else{
					file1<<"player"<<endl<<endl;
					file1<<"row : "<<row<<endl;
					file1<<"col : "<<col<<endl;
					player = 0;
					return;	
				}
			}
			else{
				cout<<"Invalid input\n";
				_sleep(2*1000);
				break;
			}	
		}
	}
}

bool isGameOver(int checkerboard[][Size],int black,int white){
	if(black+white==64||white==0||black==0||!(checkCanPlayChess(checkerboard,0)||checkCanPlayChess(checkerboard,1)))
		return true;
	else
		return false;
}

vector<Node> findnode(int checkerboard[][Size],int player){
	vector<Node> arr;
	for(int i=1;i<Size;i++){
		for(int j=1;j<Size;j++){
			bool find = false; 
			if(checkerboard[i][j]!=0){
				continue;
			}
			for(int k=0;k<8;k++){
				if(checkerboard[i+dir[k][0]][j+dir[k][1]]==0||(player==1&&checkerboard[i+dir[k][0]][j+dir[k][1]]==1)||(player==0&&checkerboard[i+dir[k][0]][j+dir[k][1]]==-1)){
					continue;
				}
				int xStart=i,yStart=j;
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size){
					if((player==1&&checkerboard[xStart][yStart]==1)||(player==0&&checkerboard[xStart][yStart]==-1)){
						arr.push_back(Node(i,j));
						find = true;
						break;
					}
					xStart+=dir[k][0];
					yStart+=dir[k][1];
					if(checkerboard[xStart][yStart]==0)
						break;
				}
				if(find)
					break;
			}
		}		
	}
	return arr;
}

int ansR,ansC;
int Minimax(int checkerboard[][Size],int black,int white,int player,int depth){
	//cout<<nodetotal<<endl;
	nodetotal++;
	int m;
	if(depth==0||isGameOver(checkerboard,black,white)){
		if(depth%2==0)
			return white-black;
		else
			return -1*white-black;
	}
	else{
		vector<Node> arr = findnode(checkerboard,player);
		if(arr.size()==0)
			m=Minimax(checkerboard,black,white,!player,depth-1);
		else{
			m=INT_MIN;
			for(int i=0;i<arr.size();i++){
				//log<<"Parent : "<<arr[i].row<<" "<<arr[i].col<<" depth : "<<depth-1<<endl;
				int temp[Size][Size],tempB=black,tempW=white;
				copy(checkerboard,checkerboard+Size,temp);
				updateCheckerboard(temp,arr[i].row,arr[i].col,tempB,tempW,player);
				int t=-1*Minimax(temp,tempB,tempW,!player,depth-1);
				if(depth==limitDep)
					t*=-1;
				if(t>m){
					m=t;
					if(depth==limitDep){
						ansR=arr[i].row;
						ansC=arr[i].col;	
					}
				}
			}	
		}
	}
	return m;
}

void AIChess(int checkerboard[][Size],int &black,int &white,int &player){
	nodetotal = 0;
	if(!checkCanPlayChess(checkerboard,player)){
		player = 1;
		return ;
	}
	int temp[Size][Size];
	copy(checkerboard,checkerboard+Size,temp);
	log.open("./test/log.txt", ios::out|ios::trunc);
	START = clock();
	Minimax(temp,black,white,player,limitDep);
	END = clock();
	log.close();
	file2<<"AI"<<endl;
	file2<<"row : "<<ansR<<endl;
	file2<<"col : "<<ansC<<endl;
	file2<<"node : "<<nodetotal<<endl;
	file2<<"time : "<<(END - START)/CLOCKS_PER_SEC<<endl<<endl;
	LogCheckerboard<<ansR<<" "<<ansC<<endl;
//	_sleep(2*1000);
	updateCheckerboard(checkerboard,ansR,ansC,black,white,player);
	player = 1;
}



int main(){
	file1.open("./test/player.txt", ios::out|ios::trunc);
	file2.open("./test/AI.txt", ios::out|ios::trunc);
	LogCheckerboard.open("./test/Checkerboard.txt", ios::out|ios::trunc);
	int checkerboard[Size][Size];
	int black=2,white=2,player=1;
	for(int i=0;i<Size;i++)
		for(int j=0;j<Size;j++)
			checkerboard[i][j]=0;
	checkerboard[4][4]=checkerboard[5][5]=1; checkerboard[4][5]=checkerboard[5][4]=-1;
	printCheckerboard(checkerboard,black,white,player);
	while(1){
		if(player)
			playerChess(checkerboard,black,white,player);
		else
			AIChess(checkerboard,black,white,player);	
		printCheckerboard(checkerboard,black,white,player);
		if(isGameOver(checkerboard,black,white)){
			break;
		}
	}
	if(white==0||(black+white==64&&black>white))
		cout<<"player is win";
	else if(black==0||(black+white==64&&white>black))
		cout<<"AI is win";
	else if(black+white==64&&white==black)
		cout<<"Tie";
	file1.close();
	file2.close();
	LogCheckerboard.close();
}

