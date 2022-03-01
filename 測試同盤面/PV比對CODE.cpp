#include"lib.h"
using namespace std;

int main(){
	string file1Name,file2Name;
	cout<<"file1 : ";
	getline(cin,file1Name);
	cout<<"file2 : ";
	getline(cin,file2Name);
	fstream file1,file2;
	file1.open("./"+file1Name+".txt",ios::in);
	file2.open("./"+file2Name+".txt",ios::in);
	string str1,str2;
	int same=0,diff=0,total=0;
	for(int i=1;i<=2000;i++){
		getline(file1,str1);
		getline(file2,str2);
		if(str1==str2){
			same++;
		}
		else{
			diff++;
			cout<<i<<endl;
		}
		total++;
	}
	cout<<"same : "<<same<<endl;
	cout<<"different : "<<diff<<endl;
	cout<<"total : "<<total<<endl;
	file2.close();
	file1.close();
	system("pause");
}
