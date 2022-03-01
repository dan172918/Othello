#include"lib.h"
#define Size 9
#define TIME_LIMIT 59.5
using namespace std;
int dir[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
clock_t start;
uint128_t hash_size=67108864;
int black=2,white=2,player=1,limitDep;
unsigned long long int nodetotal;
fstream LogOthelloBoard,file1;
int Priority[Size][Size]={{0,1,2,3,4,5,6,7,8},
						  {1,0,7,1,3,3,1,7,0},
						  {2,7,8,4,5,5,4,8,7},
						  {3,1,4,2,6,6,2,4,1},
					 	  {4,3,5,6,0,0,6,5,3},
						  {5,3,5,6,0,0,6,5,3},
						  {6,1,4,2,6,6,2,4,1},
						  {7,7,8,4,5,5,4,8,7},
						  {8,0,7,1,3,3,1,7,0}};
struct Node{
	int row,col,dir;
	Node():row(-1),col(-1),dir(0){}
	Node(int r,int c):row(r),col(c),dir(0){}
	Node(int r,int c,int d):row(r),col(c),dir(d){}
};

struct data{
	uint128_t hash_key;
	int depth;
	int best_value;
	int flag;
	Node best_move;
};

uint128_t color[2]={144273726689949495836995033479083039575 ,200046467506388815347287905258246642198};
uint128_t Rand_Table[2][64]={{331521139640376811545756934974339895708 ,104744006742484813818029533909823670323 ,137269820806552865747429763511354773094 ,87074413794280813077057021467030659230 ,55025862979473919767688068385354525715 ,35079077761981332779015104263258387551 ,137537570904392359969557424252087327682 ,36511153739360635291240387553462618979 ,312048509544638809986689286520869379691 ,12117610810550171370961974851406158403 ,27470109422580845528251842868886310398 ,5242290877804743547701214302707258872 ,224046841551830133082306818429112105284 ,226027822201921587456569446126986404533 ,20049225409539441041512653281461356845 ,225746129281228164580237356550897036314 ,100031192430852754868161288669339521299 ,238241611416837230431337668128210724475 ,85400137670637205908120816849758949460 ,284737775783071612099903326746033042525 ,63744561672611533323575065236378472806 ,209518030141349548149370889091828932822 ,280991288412704322718235054604198879353 ,43617070545313281803846351784332453258 ,262706481554008963679410566016177132977 ,221949477886859038976141599051105834290 ,4553281494066337565450674678318607883 ,138313275457234202191474482226448643874 ,46858734332342543723155349759718609009 ,276995956318558396557231410840147980236 ,150117908916731197159330552649173097007 ,3780802494016218808635788046552490222 ,101376246669537622809591402172466585710 ,333197581360877094462258002073826886246 ,240264221245175805867895800841085814124 ,30330504880661731401566332206194554263 ,115292627762971688891231002885494650889 ,51296413845741431612363776300287019683 ,172275475935992724884804547570933110477 ,75818083424174529827831808133207776473 ,263488366558716921894587433813326913313 ,39458057544902729873379526930760473450 ,310027324628581363338705529947075215648 ,28023827382524496248583876125036941693 ,190196671118629003458596186063559769179 ,213082110404234847241166525365009683823 ,189107170774567838377170332318150658481 ,83445644698009332698835769954774102113 ,231407893935487761581996854237247213962 ,158368656254088197014341298379991112922 ,322475720192353403233171326802888601908 ,71887590771209139809048187018510404582 ,308883751592206525837070766032179464940 ,118148183793022331652516283949834638423 ,207462951794078747865478582855700206132 ,132794083387077771653171979128701863561 ,104976423085145425473807378148469581826 ,153817600857651173682518430643452272849 ,268411052948233579706521117708378671401 ,240109379302016453788410462332692264723 ,332859357855661864551795313178068640155 ,268392356500094887048642442130986763217 ,327215502272647523757670517046466727788 ,267557349267479129887394869434003601032},
							 {240432487327567138781122664458544170335 ,123354108685614510403752739583427417772 ,286564165789247915105607783865124660772 ,207304855343258345232575352367281976548 ,77944717237469603157822701752780617046 ,198708457343113326456191100426500486576 ,176337136610626222592982521338236666296 ,174304317420628494419916553029822109493 ,209399608302510320555918128732762988925 ,186488229345662091843758776954689700333 ,250463024280090619923843941592474801231 ,210021388424662569229043652882186618650 ,336697426770430079919188588793525411314 ,269573386827746416168339497794636191342 ,89756895136202811932121581009061823960 ,182956027452391424569143282301898930690 ,318750221202872325542897620796190298321 ,147392983641534175044235283226739525809 ,34073660057072914512082132909294687601 ,341112968030979532010098638805969065 ,6343203132797287739901159767748155545 ,36332714260685075377522928593739186539 ,157873081901264878308303887489142963060 ,108653069716168184111373703560691143324 ,47468276786823665123134846444037384409 ,160021381664021577559413557697083331623 ,126120827214592405024050808299586537687 ,332470491985165939393576494812349848670 ,83252704962494437401746329915386107692 ,4002220648624783822780021045668670213 ,281292463517995612302955508222468541933 ,500092527565680182175209116466532793 ,54943972540007590171668011977704233962 ,154136874094873448173958378299875387863 ,229541284111333379265508162054183297006 ,50426017743925284454344853782299998560 ,42257645985191880996695201984658271088 ,105369497914295550270224226571064892457 ,170634629422958637542365139001046697410 ,9244206243219908300834481226126111214 ,90868297598496883634290869890828445264 ,232404322438482268923617804155737482717 ,167671076898961172730264868220581155599 ,171157011723927743861401495627839045420 ,262597851243837120286048602941279447513 ,55792329848759344025856734080362619139 ,316286194549212216453646464800178155500 ,215741607706107382372139674540681080296 ,334672935646756398353049193308968384789 ,314886311930555116358344167841101091864 ,253642323730561808273672573544376152611 ,302983887807088233482698011520755918810 ,36624266462037533805571689179384138797 ,300582521643459231754694435012021024680 ,300179411010097976401203771368325631411 ,185542961464751970161693792114459189350 ,24008925190939424909060765533905363949 ,169475686806657554737535400899933470559 ,203370001187235568155940368598248884749 ,53270138807730128071020577106099921587 ,316328945525909585625147531632198224530 ,304487151982720248031061745796755945908 ,251218968426099167684552531884593778315 ,141196538756525781907681926208166922289}};

data *hash_tab;

int Max(int a,int b){
	return (a>b)?a:b;
}

Node sn((Size-1)/2-1,(Size-1)/2-1),en((Size-1)/2+2,(Size-1)/2+2); 
void updateNode(int OthelloBoard[][Size],Node &sn,Node &en,int row,int col){
	if(sn.row==row){
		if(sn.row!=1&&OthelloBoard[row-1][0]!=8)
			sn.row--;
		while(OthelloBoard[sn.row][0]==8)
			sn.row++;
	}	
	else if(en.row==row){
		if(en.row!=8&&OthelloBoard[row+1][0]!=8)
			en.row++;
		while(OthelloBoard[en.row][0]==8)
			en.row--;
	}
	if(sn.col==col){
		if(sn.col!=1&&OthelloBoard[0][col-1]!=8)
			sn.col--;
		while(OthelloBoard[0][sn.col]==8)
			sn.col++;
	}	
	else if(en.col==col){
		if(en.col!=8&&OthelloBoard[0][col+1]!=8)
			en.col++;
		while(OthelloBoard[0][en.col]==8)
			en.col--;
	}
}

void updateOthelloBoard(int OthelloBoard[][Size],Node node,int &black,int &white,int player,uint128_t &hash_val){
	int xStart,yStart;
	for(int i=node.dir;i<8;i++){
		xStart=node.row+dir[i][0];yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size||(player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1))
			continue;
		if(OthelloBoard[xStart][yStart]>=0){
			OthelloBoard[xStart][yStart]=(OthelloBoard[xStart][yStart]>(i+4)%8)?(i+4)%8:OthelloBoard[xStart][yStart];
			continue;
		}
		bool findplayerColor=false;
		xStart+=dir[i][0];
		yStart+=dir[i][1];
		while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&OthelloBoard[xStart][yStart]<0){
			if((player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1)){
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
			if(player&&OthelloBoard[xStart][yStart]==-1){
				white--;
				black++;
				OthelloBoard[xStart][yStart]=-2;
			}
			else if(!player&&OthelloBoard[xStart][yStart]==-2){
				white++;
				black--;
				OthelloBoard[xStart][yStart]=-1;
			}
			hash_val^=Rand_Table[player][(xStart-1)*8+yStart-1]^Rand_Table[!player][(xStart-1)*8+yStart-1];
		}

	}
	if(player){
		OthelloBoard[node.row][node.col]=-2;
		black++;
	}
	else{
		OthelloBoard[node.row][node.col]=-1;
		white++;
	}
	hash_val^=Rand_Table[player][(node.row-1)*8+node.col-1];
	OthelloBoard[node.row][0]++;
	OthelloBoard[0][node.col]++;
	for(int i=0;i<node.dir;i++){
		xStart=node.row+dir[i][0];yStart=node.col+dir[i][1];
		if(xStart==0||yStart==0||xStart==Size||yStart==Size)
			continue;
		if(OthelloBoard[xStart][yStart]>=0)
			OthelloBoard[xStart][yStart]=(OthelloBoard[xStart][yStart]>(i+4)%8)?(i+4)%8:OthelloBoard[xStart][yStart];		
	}
}

bool method(Node a,Node b){
	if(Priority[a.row][a.col]<Priority[b.row][b.col])
		return true;
	return false;
}

vector<Node> findnode(int OthelloBoard[][Size],int player,Node sn,Node en){
	vector<Node> arr;
	for(int i=sn.row;i<=en.row;i++){
		for(int j=sn.col;j<=en.col;j++){
			bool find = false;
			if(OthelloBoard[i][j]<0||OthelloBoard[i][j]==8)
				continue;
			for(int k=OthelloBoard[i][j];k<8;k++){
				int xStart=i+dir[k][0],yStart=j+dir[k][1];
				if(xStart==0||yStart==0||xStart==Size||yStart==Size||OthelloBoard[xStart][yStart]>=0||(player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1))
					continue;
				xStart+=dir[k][0];
				yStart+=dir[k][1];
				while(xStart>0&&xStart<Size&&yStart>0&&yStart<Size&&OthelloBoard[xStart][yStart]<0){
					if((player&&OthelloBoard[xStart][yStart]==-2)||(!player&&OthelloBoard[xStart][yStart]==-1)){
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

bool timeIsUp(){
	return ((double)(clock() - start) / CLOCKS_PER_SEC >= TIME_LIMIT);
}

clock_t costTime=0;
void update_hash(uint128_t hash_val,int hash_index,bool NeedUpdate,int depth,int flag,int value,Node node){
//	clock_t test1 = clock();
	if(NeedUpdate){
		hash_tab[hash_index].hash_key=hash_val;
		hash_tab[hash_index].depth=depth;
		hash_tab[hash_index].flag=flag;
		hash_tab[hash_index].best_value=value;
		hash_tab[hash_index].best_move=node;			
	}
//	costTime+=clock() - test1;
}

Node ans(0,0);
int AlphaBeta(int OthelloBoard[][Size],int alpha,int beta,int black,int white,int player,int depth,Node sn,Node en,bool skip,uint128_t hash_val){
	int hash_index = int(hash_val % hash_size),m=INT_MIN;
	bool NeedUpdate=true;
//	clock_t test1 = clock();
	if(hash_tab[hash_index].hash_key==hash_val){
		if(hash_tab[hash_index].depth >= limitDep - depth){
			if(hash_tab[hash_index].flag==0){
//				costTime+=clock() - test1;
				return hash_tab[hash_index].best_value;				
			}			
			if(hash_tab[hash_index].flag==-1 && hash_tab[hash_index].best_value>alpha){
				alpha=hash_tab[hash_index].best_value;
			}
			else if(hash_tab[hash_index].flag==1 && hash_tab[hash_index].best_value<beta){
				beta=hash_tab[hash_index].best_value;
			}
			if(alpha>=beta){
//				costTime+=clock() - test1;
				return hash_tab[hash_index].best_value;				
			}
			if(hash_tab[hash_index].depth > limitDep - depth)
				NeedUpdate = false;
		}
	}
//	costTime+=clock() - test1;
	if(/*timeIsUp()||*/depth == limitDep || black + white == 64 || white == 0 || black == 0){
		return (player) ? black - white : white - black;
	}
	else{
		vector<Node> arr = findnode(OthelloBoard,player,sn,en);
		if(arr.size() == 0){
			if(skip)
				return (!player) ? black - white : white - black;
			hash_val ^= color[0] ^ color[1];
			return -1 * AlphaBeta(OthelloBoard,-1*beta,-1*alpha,black,white,!player,depth,sn,en,true,hash_val);
		}
		nodetotal += arr.size();
		if(depth < (limitDep - 3)){
			sort(arr.begin(),arr.end(),method);			
		}
		Node best;	
		for(int i=0 ; i<arr.size() ; i++){
			int temp[Size][Size],tempB = black,tempW = white;
			uint128_t temp_hash_val=hash_val;
			Node tempSn = sn,tempEn = en;
			temp_hash_val ^= color[0] ^ color[1];
			memcpy(temp,OthelloBoard,sizeof(temp));
			updateOthelloBoard(temp,arr[i],tempB,tempW,player,temp_hash_val);
			updateNode(temp,tempSn,tempEn,arr[i].row,arr[i].col);
			int t=-1*AlphaBeta(temp,-1*beta,-1*Max(m,alpha),tempB,tempW,!player,depth+1,tempSn,tempEn,false,temp_hash_val);
			if(t>m){
				if(limitDep==14)
					ans=arr[i];
				m=t;
			}
			if(m>=beta){
				update_hash(hash_val,hash_index,NeedUpdate,limitDep-depth,-1,m,arr[i]);
				return m;
			}				
		}
		update_hash(hash_val,hash_index,NeedUpdate,limitDep-depth,(m>alpha)?0:1,m,best);
		return m;
	}
}

void AIChess(int OthelloBoard[][Size],uint128_t hash_val){
	int temp[Size][Size],m;
	memcpy(temp,OthelloBoard,sizeof(temp));
	start = clock();
	
//	for(limitDep=2 ; (double)(clock() - start)/CLOCKS_PER_SEC < TIME_LIMIT ; limitDep+=2){
//		nodetotal = 0;
//		temp_hash_val=hash_val;
//		AlphaBeta(temp,-10000,10000,black,white,player,0,sn,en,false,temp_hash_val);
//		LogOthelloBoard<<","<<(timeIsUp() ? 'U' : 'D')<<","<<limitDep<<","<<nodetotal<<endl;	
//	}
	int hash_index = int(hash_val % hash_size);
	for(limitDep=2 ; limitDep<=14; limitDep+=2){
		nodetotal = 0;
		costTime=0;
		m=AlphaBeta(temp,-10000,10000,black,white,player,0,sn,en,false,hash_val);
//		cout<<limitDep<<" : "<<(double)costTime/ CLOCKS_PER_SEC<<endl;
//		ans=hash_tab[hash_index].best_move;
		LogOthelloBoard<<","<<(double)(clock() - start) / CLOCKS_PER_SEC<<","<<nodetotal<<","<<m;
	}
	LogOthelloBoard<<","<<ans.row<<ans.col;	
	LogOthelloBoard<<endl;
}

void updateInitialNode(int OthelloBoard[][Size]){
	for(int i=1;i<Size;i++){
		if(OthelloBoard[i][0]==8)
			continue;
		if(OthelloBoard[i][0]!=0){
			sn.row=(i!=1)?((OthelloBoard[i-1][0]!=8)?i-1:i):i;
		break;
		}
	}
	for(int i=Size-1;i>0;i--){
		if(OthelloBoard[i][0]==8)
			continue;
		if(OthelloBoard[i][0]!=0){
			en.row=(i!=Size-1)?((OthelloBoard[i+1][0]!=8)?i+1:i):i;
		break;
		}
	}
	
	for(int i=1;i<Size;i++){
		if(OthelloBoard[0][i]==8)
			continue;
		if(OthelloBoard[0][i]!=0){
			sn.col=(i!=1)?((OthelloBoard[0][i-1]!=8)?i-1:i):i;
		break;
		}
	}
	for(int i=Size-1;i>0;i--){
		if(OthelloBoard[0][i]==8)
			continue;
		if(OthelloBoard[0][i]!=0){
			en.col=(i!=Size-1)?((OthelloBoard[0][i+1]!=8)?i+1:i):i;
		break;
		}
	}
}


int main(){
	LogOthelloBoard.open("./2ID-hashª©_Alpha-Beta.csv",ios::out);
//	LogOthelloBoard<<"board,U/D,depth,Node"<<endl;
	LogOthelloBoard<<",depth2,,,depth4,,,depth6,,,depth8,,,depth10,,,depth12,,,depth14,,,"<<endl;
	LogOthelloBoard<<"board,Time,Node,value,Time,Node,value,Time,Node,value,Time,Node,value,Time,Node,value,Time,Node,value,Time,Node,value"<<endl;
	int OthelloBoard[Size][Size];
	int boardNum[50]={1390,1963,1337,415,1612,560,1314,846,382,1667,1065,1960,915,155,40,1263,865,150,700,1741,876,1977,949,1626,570,89,341,269,1873,1467,1870,908,462,1657,85,393,151,457,1324,1786,806,336,1911,1609,1871,713,1671,1927,798,601};
	
	uint128_t hash_val;
	memset (OthelloBoard,0,sizeof(OthelloBoard));
	string str;
	for(int i=0;i<1;i++){
		cout<<i<<endl;
		hash_tab=new data[int(hash_size)];
		LogOthelloBoard<<boardNum[i];
		
		//initial board
		black=0;white=0;hash_val=0;
		memset (OthelloBoard,0,sizeof(OthelloBoard));
		for(int j=1;j<Size;j++){
			for(int k=1;k<Size;k++){
				OthelloBoard[j][k]=8;
			}
		}

		//file loading initial board
		file1.open("./board/"+to_string(boardNum[i])+".txt",ios::in);
//		file1.open("./testB/1262.txt",ios::in);
		file1>>str;
		player = (str=="o")?1:0;
		hash_val^=color[player];
		for(int j=1;j<Size;j++){
			file1>>str;
			for(int k=1;k<Size;k++){
				if(str[k-1]!='_'){
					OthelloBoard[j][0]++;
					OthelloBoard[0][k]++;
					if(str[k-1]=='o'){
						hash_val^=Rand_Table[1][(j-1)*8+k-1];
						OthelloBoard[j][k]=-2;
						black++;
					}
					else{
						hash_val^=Rand_Table[0][(j-1)*8+k-1];
						OthelloBoard[j][k]=-1;
						white++;
					}
				}
			}
		}
		//update dir 
		for(int a=1;a<Size;a++){
			for(int j=1;j<Size;j++){
				if(OthelloBoard[a][j]<0){
					for(int k=0;k<8;k++){
						if(a+dir[k][0]>0&&a+dir[k][0]<Size&&j+dir[k][1]>0&&j+dir[k][1]<Size&&OthelloBoard[a+dir[k][0]][j+dir[k][1]]>=0)
							OthelloBoard[a+dir[k][0]][j+dir[k][1]]=(OthelloBoard[a+dir[k][0]][j+dir[k][1]]>(k+4)%8)?(k+4)%8:OthelloBoard[a+dir[k][0]][j+dir[k][1]];
					}					
				}
			}
		}
		file1.close();
		//update move generator range
		updateInitialNode(OthelloBoard);
		//alpha-beta search
		AIChess(OthelloBoard,hash_val);
		delete []hash_tab;
	}
	LogOthelloBoard.close();
}

