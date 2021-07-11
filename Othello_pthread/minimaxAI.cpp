#include"lib.h"
#include <pthread.h>
#define Size 9
#define limitDep 6
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=0,white=0,player=0;
fstream file1,file2;

struct Node{
	int row,col;
	Node *next;
	Node():row(-1),col(-1),next(NULL){}
	Node(int r,int c):row(r),col(c),next(NULL){}
};

struct parameter{
	int checkerboard[Size][Size],black,white,player,depth;
	Node* node = new Node();
};

bool checkCanPlayChess(int checkerboard[][Size],int player){
	for(int i=1;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(checkerboard[i][j])
				continue;
			for(int k=0;k<8;k++){
				if(checkerboard[i+dir[k][0]][j+dir[k][1]]==0||(player&&checkerboard[i+dir[k][0]][j+dir[k][1]]==1)||(!player&&checkerboard[i+dir[k][0]][j+dir[k][1]]==-1))
					continue;
				int xStart=i,yStart=j;
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size){
					if((player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1))
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

bool updateCheckerboard(int checkerboard[][Size],int row,int col,int &black,int &white,int player){
	int initBlack = black, initwhite = white;
	for(int i=0;i<8;i++){
		if(checkerboard[row+dir[i][0]][col+dir[i][1]]==0||(player&&checkerboard[row+dir[i][0]][col+dir[i][1]]==1)||(!player&&checkerboard[row+dir[i][0]][col+dir[i][1]]==-1))
			continue;
		bool findplayerColor=false;
		int xStart=row,yStart=col;
		while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size){
			if((player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1)){
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
			if(player&&checkerboard[xStart][yStart]==-1){
				white--;
				black++;
				checkerboard[xStart][yStart]=1;
			}
			else if(!player&&checkerboard[xStart][yStart]==1){
				white++;
				black--;
				checkerboard[xStart][yStart]=-1;
			}

		}
	}
	if(initBlack==black&&initwhite==white)
		return false;
	if(player){
		checkerboard[row][col]=1;
		black++;
	}
	else{
		checkerboard[row][col]=-1;
		white++;
	}
	return true;
}
bool isGameOver(int checkerboard[][Size],int player,int black,int white){
	if(black+white==64||white==0||black==0||(!checkCanPlayChess(checkerboard,player)&&!checkCanPlayChess(checkerboard,!player)))
		return true;
	else
		return false;
}

vector<Node*> findnode(int checkerboard[][Size],int player){
	vector<Node*> arr;
	for(int i=1;i<Size;i++){
		for(int j=1;j<Size;j++){
			bool find = false; 
			if(checkerboard[i][j])
				continue;
			for(int k=0;k<8;k++){
				if(checkerboard[i+dir[k][0]][j+dir[k][1]]==0||(player&&checkerboard[i+dir[k][0]][j+dir[k][1]]==1)||(!player&&checkerboard[i+dir[k][0]][j+dir[k][1]]==-1))
					continue;
				int xStart=i,yStart=j;
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size){
					if((player&&checkerboard[xStart][yStart]==1)||(!player&&checkerboard[xStart][yStart]==-1)){
						Node *node = new Node(i,j);
						arr.push_back(node);
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
//	for(int i=0;i<arr.size();i++){
//		cout<<arr[i]->row<<" "<<arr[i]->col<<endl;
//	}
//	_sleep(10*1000);
	return arr;
}

void print(parameter par){
	for(int i=1;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(par.checkerboard[i][j]==0)
				cout<<'-';
			else if(par.checkerboard[i][j]==1)
				cout<<'o';
			else if(par.checkerboard[i][j]==-1)
				cout<<'x';
		}
		cout<<endl;
	}
	cout<<"black : "<<par.black<<" white : "<<par.white<<endl;
	if(par.player)
		cout<<"player"<<endl;
	else
		cout<<"AI"<<endl;
	cout<<"depth : "<<par.depth<<endl;
//	cout<<endl;
}
void* minimax(void* data){
	parameter par = *(parameter*)data;
	int m=INT_MIN;
	if(par.depth==0||isGameOver(par.checkerboard,par.player,par.black,par.white)){
		if(par.player){
			if(par.depth==limitDep-1)
				pthread_exit((void*)(par.white-par.black));
			else
				return (void*)(par.white-par.black);
		}
		else{
			if(par.depth==limitDep-1)
				pthread_exit((void*)(par.black-par.white));	
			else
				return (void*)(par.black-par.white);		
		}
	}
	else{
		vector<Node*> arr = findnode(par.checkerboard,par.player);
		if(arr.size()==0){
			par.player=!(par.player);
			void* val2=minimax(&par);
			m=-1*(*((int*)&val2));
		}
		else{
			pthread_t thread[arr.size()];
			void* val;
			if(par.depth==limitDep){
				for(int i=0;i<arr.size();i++){
					parameter temp=par;
					updateCheckerboard(temp.checkerboard,arr[i]->row,arr[i]->col,temp.black,temp.white,par.player);
					temp.player=!(par.player);
					temp.depth--;
					temp.node = arr[i];
					pthread_create(&thread[i],NULL,minimax,(void*)&temp);
					_sleep(1);
				}
				for(int i=0;i<arr.size();i++){
					pthread_join(thread[i],&val);
					int t=(*((int*)&val));
//					cout<<i<<" "<<t<<endl;
					if(t>m){
						m=t;
						par.node->next=arr[i];
					}
				}
				file2<<"best move : "<<m<<endl;
			}
			else{
				for(int i=0;i<arr.size();i++){
					parameter temp=par;
					updateCheckerboard(temp.checkerboard,arr[i]->row,arr[i]->col,temp.black,temp.white,par.player);
					temp.player=!(par.player);
					temp.depth--;
					temp.node = arr[i];
					void* val1=minimax(&temp);
					int t1=-1*(*((int*)&val1));
					if(t1>m){
						m=t1;
						par.node->next=arr[i];
					}
				}
				
			}			
		}
	}
	if(par.depth==limitDep-1)
		pthread_exit((void*)m);
	else
		return (void*)m;
}

int main(){
	void* bestmove;
	string str;
	file2.open("./400.txt",ios::out|ios::trunc);
	parameter Par;
	for(int i=1;i<=400;i++){
		Par.black=0;Par.white=0;
		memset (Par.checkerboard,0,sizeof(Par.checkerboard));
		file1.open("./othello-initial board-400/"+to_string(i)+".txt",ios::in);
		file1>>str;
		Par.player = (str=="o")?1:0;
		for(int j=1;j<Size;j++){
			file1>>str;
			for(int k=1;k<Size;k++){
				if(str[k-1]=='o'){
					Par.checkerboard[j][k]=1;
					Par.black++;
				}
				else if(str[k-1]=='x'){
					Par.checkerboard[j][k]=-1;
					Par.white++;
				}
			}
		}
		file1.close();
		Par.depth=limitDep;
		file2<<i<<" : "<<endl;
//		pthread_t rootThread;
//		pthread_create(&rootThread,NULL,minimax,(void*)&Par);
//		pthread_join(rootThread,&bestmove);
		minimax(&Par);
		

		while(1){
			Par.node = Par.node->next;
			file2<<"( "<<Par.node->row<<","<<Par.node->col<<" )";
			if(Par.node->next!=NULL)
				file2<<" -> ";
			else
				break;	
		}
		file2<<endl<<endl;
	}

	file2.close();
}

