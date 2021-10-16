#include"lib.h"
#define Size 9
#define limitDep 8 
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
double START,END;
int black=2,white=2,player=1;
int nodetotal = 0;
fstream file1,LogCheckerboard,pvLog;

struct Node{
	int row,col,dir;
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
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

Node ans(0,0);
int Minimax(int checkerboard[][Size],int black,int white,int player,int depth,Node sn,Node en,Node test){
	if(depth==0||black+white==64||white==0||black==0){
		return (player)?black-white:white-black;
	}
	else{
		vector<Node> arr = findnode(checkerboard,player,sn,en);
		bool samePlayer=false;
		int m=INT_MIN;
		if(arr.size()==0){
			samePlayer=true;
			player=!player; 
			arr = findnode(checkerboard,player,sn,en);
			if(arr.size()==0)
				return (!player)?black-white:white-black;
		}
		nodetotal+=arr.size();
		for(int i=0;i<arr.size();i++){
			int temp[Size][Size],tempB=black,tempW=white;
			Node tempSn=sn,tempEn=en;
			memcpy(temp,checkerboard,sizeof(temp));
			updateCheckerboard(temp,arr[i],tempB,tempW,player);
			updateNode(temp,tempSn,tempEn,arr[i].row,arr[i].col);
			int t=-1*Minimax(temp,tempB,tempW,!player,depth-1,tempSn,tempEn,arr[i]);
//			if(depth==limitDep||depth==limitDep-1)
//				pvLog<<"pare : ("<<test.row<<","<<test.col<<") depth : "<<depth<<" ( "<<arr[i].row<<","<<arr[i].col<<" ) : "<<t<<endl;
			if(t>m){
				m=t;
				if(depth==limitDep){
					ans=arr[i];	
				}
			}
		}
		if(samePlayer)
			m*=-1;
		return m;
	}
}

void AIChess(int checkerboard[][Size]){
	nodetotal = 0;
	if(!checkCanPlayChess(checkerboard,player,sn,en)){
		player = 1;
		cout<<"無可下位置，換玩家";
		_sleep(0.5*1000);
		printCheckerboard(checkerboard);
		return ;
	}
	int temp[Size][Size];
	memcpy(temp,checkerboard,sizeof(temp));
	START = clock();
	Minimax(temp,black,white,player,limitDep,sn,en,ans);
	END = clock();
	file1<<(END - START)/CLOCKS_PER_SEC<<","<<nodetotal<<endl;
	updateCheckerboard(checkerboard,ans,black,white,player);
	updateNode(checkerboard,sn,en,ans.row,ans.col);
	player = 1;
	LogCheckerboard<<endl<<"AI : ( "<<ans.row<<","<<ans.col<<" )"<<endl;
	pvLog<<"( "<<ans.row<<","<<ans.col<<" )"<<endl<<endl<<endl;
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
	file1.open("./test/"+to_string(limitDep)+"/6.0_Minimax.csv",ios::out);
	file1<<"depth "+to_string(limitDep)+" :"<<endl;
	file1<<"Time,Node"<<endl;
	LogCheckerboard.open("./test/"+to_string(limitDep)+"/board.txt",ios::out);
	pvLog.open("./test/"+to_string(limitDep)+"/6.0_PV.txt",ios::out);
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
	int r,c,p=1;
	while(1){
		printCheckerboard(checkerboard);
		cin>>r>>c;
		if(r==-1)
			break;
		updateCheckerboard(checkerboard,Node(r,c),black,white,p);
		updateNode(checkerboard,sn,en,r,c);
		p=!p;	
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
	pvLog.close();	
	file1.close();
	LogCheckerboard.close();
}

