#include<iostream>
#include<set>
using namespace std;
void perm(string str,int s,int e,set<string> &ans){
    if(s>=e){
		ans.insert(str);
    }
    else{
    	for(int i=s;i<3;i++){
    		swap(str[s],str[i]);
    		perm(str,s+1,e,ans);
    		swap(str[s],str[i]);
		}
    }
}

int main(){
	string str="367";
	set<string> ans;
	perm(str,0,3,ans);
	
	for(auto i:ans)
		cout<<i<<",";
	cout<<endl;

}


