#include"lib.h"
#include<Windows.h>
#define Size 9
using namespace std;
fstream createBoard,file;

int main(){
	file.open("./boardNum.txt",ios::out);
	srand(time(0));
	vector<int> boardrandom;
	for(int i=0;i<50;i++){
		int testNum=rand()%2000+1;
		string str;
		createBoard.open("./othello-initial board-2000/"+to_string(testNum)+".txt",ios::in);
		int num=0;
		createBoard>>str;
		for(int j=1;j<Size;j++){
			createBoard>>str;
			for(int k=1;k<Size;k++){
				if(str[k-1]!='_'){
					num++;
				}
			}
		}
		createBoard.close();
		if(num!=10)
			i--;
		else if(find(boardrandom.begin(),boardrandom.end(),testNum)==boardrandom.end()){
			boardrandom.push_back(testNum);
		}
		else{
			i--;
		}	
	}
	for(int i=0;i<50;i++){
		string str1="./othello-initial board-2000/"+to_string(boardrandom[i])+".txt";
		string str2="./123/"+to_string(boardrandom[i])+".txt";
		CopyFile(str1.c_str(),str2.c_str(),FALSE);	
		file<<boardrandom[i]<<",";
	}
	file.close();
//	for(int i=0;i<25;i++){
//		createBoard.open("./othello-initial board-2000/"+to_string(boardrandom[i])+".txt",ios::in);
//		cout<<boardrandom[i]<<",";
//		createBoard.close();		
//	}


}
