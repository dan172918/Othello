#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <chrono>
#include <random>
using namespace std;
using namespace boost::multiprecision;
int main (){
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	uint128_t ans;
	std::mt19937_64 g1 (seed1);	
	for(int i=0;i<2;i++){
		for(int j=0;j<64;j++){
			ans=g1();
			ans=ans<<64;
			ans|=g1();
			cout<<ans<<((j!=127)?" ,":"");
		}
		cout<<endl;		
	}

	return 0;
}
