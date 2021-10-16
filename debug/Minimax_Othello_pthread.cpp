#include"lib.h"
#include <pthread.h>
#define Size 9
#define limitDep 9
using namespace std; 
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
int nodetotal = 0;
fstream file1,LogCheckerboard;
							 
struct Node{
	int row,col,dir;
	void* v;
	Node():row(-1),col(-1),dir(0){}
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
};
 
struct parameter{
	int checkerboard[Size][Size],black,white,player,depth;
	Node sn,en;
	bool next;
	parameter(int temp[][Size],int b,int w,int p,int d,Node S,Node E){
		memcpy(checkerboard,temp,sizeof(checkerboard));
		black=b;
		white=w;
		player=p;
		depth=d;
		sn=S;
		en=E;
		next=false;
	}
};

Node sn(3,3),en(6,6); 
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

void outputCheckerboard(int checkerboard[][Size]){
	LogCheckerboard<<"    ";
	for(int i=0;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(i==0)
				LogCheckerboard<<j<<"  ";
			else
				if(checkerboard[i][j]>=0)
					LogCheckerboard<<" - ";
				else if(checkerboard[i][j]==-2)
					LogCheckerboard<<" O ";
				else
					LogCheckerboard<<" X ";
		}
		if(i!=Size-1)
			LogCheckerboard<<"\n\n "<<i+1<<" ";			
	}
	LogCheckerboard<<"\n O : "<<black<<"   X : "<<white;
	if(player)
		LogCheckerboard<<"\t->player(O)\tAI(X)"<<endl;		
	else
		LogCheckerboard<<"\tplayer(O)\t->AI(X)"<<endl;		
}

void printCheckerboard(int checkerboard[][Size]){
	system("cls");
	cout<<"    ";
	for(int i=0;i<Size;i++){
		for(int j=1;j<Size;j++){
			if(i==0)
				cout<<j<<"  ";	
			else
				if(checkerboard[i][j]>=0)
					cout<<" - ";
				else if(checkerboard[i][j]==-2)
					cout<<" O ";
				else
					cout<<" X ";
		}
		if(i!=Size-1)
			cout<<"\n\n "<<i+1<<" ";
	}
	cout<<"\n O : "<<black<<"   X : "<<white;
	if(player)
		cout<<"\t->player(O)\tAI(X)"<<endl;
	else
		cout<<"\tplayer(O)\t->AI(X)"<<endl;
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

bool isGameOver(int checkerboard[][Size],int black,int white,Node sn,Node en){
	if(black+white==64||white==0||black==0)
		return true;		
	else{
		for(int i=sn.row;i<=en.row;i++){
			for(int j=sn.col;j<=en.col;j++){
				if(checkerboard[i][j]<0||checkerboard[i][j]==8)
					continue;
				for(int k=checkerboard[i][j];k<8;k++){
					int xStart=i+dir[k][0],yStart=j+dir[k][1];
					if(xStart==0||yStart==0||xStart==Size||yStart==Size||checkerboard[xStart][yStart]>=0)
						continue;
					xStart+=dir[k][0];
					yStart+=dir[k][1];
					while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&checkerboard[xStart][yStart]<0){
						if(checkerboard[xStart-dir[k][0]][yStart-dir[k][1]]!=checkerboard[xStart][yStart])
							return false;
						xStart+=dir[k][0];
						yStart+=dir[k][1];
					}
				}
			}		
		}		
	}
	return true;
}
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
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
//	pthread_mutex_lock(&count_mutex);
	nodetotal+=arr.size();
//	pthread_mutex_unlock(&count_mutex);
	return arr;
}

int testDep=1;
void* minimax(void* data){
	parameter par = *(parameter*)data;
	if(par.depth==0||isGameOver(par.checkerboard,par.black,par.white,par.sn,par.en)){
		if(par.depth==limitDep-(testDep+1)&&(limitDep-(testDep+1))%2==!par.player)
			pthread_exit((void*)((par.player)?par.white-par.black:par.black-par.white));
		else
			return (void*)((par.player)?par.white-par.black:par.black-par.white);
	}
	else{
		vector<Node> arr = findnode(par.checkerboard,par.player,par.sn,par.en);
		void* val;int m=INT_MIN;
		if(arr.size()==0){
			parameter temp=par;
			temp.player=!(temp.player);
			temp.next=true;
			val = minimax((void*)&temp);
			m=-1*(*(int*)&val);
		}
		else{
			for(int i=0;i<arr.size();i++){
				parameter temp=par;
				updateCheckerboard(temp.checkerboard,arr[i],temp.black,temp.white,temp.player);
				updateNode(temp.checkerboard,temp.sn,temp.en,arr[i].row,arr[i].col);
				temp.player=!(temp.player);
				temp.depth--;
				temp.next=false;
				arr[i].v = minimax((void*)&temp);
				if(!par.next)
					*(int*)&arr[i].v*=-1;
				if(*(int*)&arr[i].v>m)
					m=*(int*)&arr[i].v;
			}		
		}
		if(par.next)
			m*=-1;
		if(par.depth==limitDep-(testDep+1)&&(limitDep-(testDep+1))%2==!par.player)
			pthread_exit((void*)m);
		else
			return (void*)m;
	}
}

Node ans(0,0);
void* minimax_thread(void* data){
	parameter par = *(parameter*)data;
	if(par.depth==0||isGameOver(par.checkerboard,par.black,par.white,par.sn,par.en))
		pthread_exit((void*)((par.player)?par.white-par.black:par.black-par.white));
	else{
		vector<Node> arr = findnode(par.checkerboard,par.player,par.sn,par.en);
		void* value; int m=INT_MIN;
		pthread_t thread[arr.size()];
		if(arr.size()==0){
			parameter temp=par;
			temp.player=!(temp.player);
			temp.next=true;
			pthread_create(&thread[0],NULL,minimax_thread,(void*)&temp);
			pthread_join(thread[0],&value);
			m=-1*(*(int*)value);
		}
		else{
			for(int i=0;i<arr.size();i++){
				parameter temp=par;
				updateCheckerboard(temp.checkerboard,arr[i],temp.black,temp.white,temp.player);
				updateNode(temp.checkerboard,temp.sn,temp.en,arr[i].row,arr[i].col);
				temp.player=!temp.player;
				temp.depth--;
				temp.next=false;
				pthread_create(&thread[i],NULL,(par.depth==limitDep-testDep)?minimax:minimax_thread,(void*)&temp);
				_sleep(150);
			}
			for(int i=0;i<arr.size();i++){
				pthread_join(thread[i],&arr[i].v);
				if(!par.next&&par.depth!=limitDep)
					*(int*)&arr[i].v*=-1;
//				else
//					cout<<"threadID : "<<thread[i]<<" row :"<<arr[i].row<<" col : "<<arr[i].col<<" score : "<<*(int*)&arr[i].v<<endl;
				if(*(int*)&arr[i].v>m){
					m=*(int*)&arr[i].v;
					if(par.depth==limitDep)
						ans=arr[i];	
				}
			}
		}
		if(par.next)
			m*=-1;
		pthread_exit((void*)m);
	}
}

void AIChess(int checkerboard[][Size]){	
	if(!checkCanPlayChess(checkerboard,player,sn,en)){
		player = 1;
		cout<<"無可下位置，換玩家";
		_sleep(0.5*1000);
		printCheckerboard(checkerboard);
		return ;
	}
	nodetotal=0;
	parameter par(checkerboard,black,white,0,limitDep,sn,en);
	pthread_t id;
	START = clock();
	pthread_create(&id,NULL,minimax_thread,(void*)&par);
	pthread_join(id,NULL);
	END = clock();
	
	file1<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<endl;
	updateCheckerboard(checkerboard,ans,black,white,player);
	updateNode(checkerboard,sn,en,ans.row,ans.col);
	player = 1;
	LogCheckerboard<<endl<<"AI : ( "<<ans.row<<","<<ans.col<<" )"<<endl;
	outputCheckerboard(checkerboard);
	printCheckerboard(checkerboard);
}

void playerChess(int checkerboard[][Size]){
	vector<Node> arr = findnode(checkerboard,player,sn,en);
	if(arr.size()==0){
		player = 0;
		cout<<"無可下位置，換AI";
		_sleep(0.5*1000);
		printCheckerboard(checkerboard);
		return ;
	}
	cout<<"可下位置 : "<<endl; 
	for(int i=0;i<arr.size();i++)
		cout<<"( "<<arr[i].row<<","<<arr[i].col<<" ) ";
	Node node(0,0);
	while(1){
		cout<<"\nrow(列) : "; 
		cin>>node.row;
		cout<<"col(行) : ";
		cin>>node.col;
		bool Invalid = true;
		for(int i=0;i<arr.size();i++){
			if(arr[i].row==node.row&&arr[i].col==node.col)
				Invalid = false;
		}
		if(node.row<1||node.row>8||node.col<1||node.col>8||checkerboard[node.row][node.col]<0||Invalid){
			cout<<"輸入錯誤\n";
			_sleep(0.5*1000);
		}
		else{
			updateCheckerboard(checkerboard,node,black,white,player);
			updateNode(checkerboard,sn,en,node.row,node.col);
			player = 0;
			LogCheckerboard<<endl<<"player : ( "<<node.row<<","<<node.col<<" )"<<endl;
			outputCheckerboard(checkerboard);
			printCheckerboard(checkerboard);
			return;					
		}
	}
}


int main(){
	file1.open("./test/"+to_string(limitDep-1)+"/pthread.csv",ios::out);
	file1<<"depth "+to_string(limitDep-1)+" :"<<endl;
	file1<<"Time,Node"<<endl;
	LogCheckerboard.open("./test/"+to_string(limitDep-1)+"/pthread.txt",ios::out);
	int checkerboard[Size][Size];
	memset (checkerboard,0,sizeof(checkerboard));
	for(int i=1;i<Size;i++){
		for(int j=1;j<Size;j++){
			checkerboard[i][j]=8;
		}
	}
	checkerboard[4][4]=checkerboard[5][5]=-2; checkerboard[4][5]=checkerboard[5][4]=-1;
	checkerboard[4][0]=checkerboard[5][0]=checkerboard[0][4]=checkerboard[0][5]=2;
	for(int i=4;i<=5;i++){
		for(int j=4;j<=5;j++){
			for(int k=0;k<8;k++){
				if(checkerboard[i+dir[k][0]][j+dir[k][1]]>=0)
					checkerboard[i+dir[k][0]][j+dir[k][1]]=(checkerboard[i+dir[k][0]][j+dir[k][1]]>(k+4)%8)?(k+4)%8:checkerboard[i+dir[k][0]][j+dir[k][1]];
			}
		}
	}

	printCheckerboard(checkerboard);
	while(!isGameOver(checkerboard,black,white,sn,en)){
		if(player)
			playerChess(checkerboard);
		else
			AIChess(checkerboard);	
	}
	if(black>white)
		cout<<"player is win";
	else if(white>black)
		cout<<"AI is win";
	else
		cout<<"Tie";
	
	file1.close();
	LogCheckerboard.close();
}

