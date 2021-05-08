#include<iostream>
#include <conio.h>
using namespace std;

/*print Othello checkerboard and chess*/
void printCheckerboard(char checkerboard[][9],int size){
	system("cls");
	cout<<"    ";
	for(int i=0;i<9;i++){
		for(int j=1;j<9;j++){
			if(i==0)
				cout<<j<<"  ";
			else
				cout<<" "<<checkerboard[i][j]<<" ";
		}
		if(i!=8)
			cout<<"\n\n "<<i+1<<" ";
	}
}

void playChess(char checkerboard[][9],int size){
	int row,col;
	char confirm;
	while(1){
		printCheckerboard(checkerboard,9);
		cout<<"\nrow(¦C) : "; 
		cin>>row;
		cout<<"col(¦æ) : ";
		cin>>col;
		while(1){
			cout<<"Confirm?(Y/N) : ";
			cin>>confirm;
			if(confirm=='Y'||confirm=='y')
				return;
			else if(confirm=='N'||confirm=='n'){
				system("cls");
				break;
			}
			else{
				cout<<"Invalid input\n";
			}	
		}
	}
}

int main(){
	/*init Othello checkerboard*/
	char checkerboard[9][9];
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			checkerboard[i][j]='-';
		}	
	}
	checkerboard[1][1]='*'; checkerboard[4][4]=checkerboard[5][5]='O'; checkerboard[4][5]=checkerboard[5][4]='X'; 
	while(1){
		playChess(checkerboard,9);
		printCheckerboard(checkerboard,9);
	}
	
}

