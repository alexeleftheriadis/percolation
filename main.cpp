#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <sstream>
#include <iomanip> //setprecision
#include <math.h>
#include <cmath> //abs
#include <random>

using namespace std;
ofstream outf;
ofstream percol;
int N=1000;
int SL=100000; 		
vector<vector<int> >stuff(N,vector<int>(N,0));
vector<long long int> L(SL,0);
vector<long long int>  S(SL,0);
int Lcount=1;
double iav,pmax,iav2,Siav,Spmax,Siav2,dens;
double kappa=N*N;
int xx, x,y,pointA, pointB, point;

mt19937_64 gen((unsigned)time(NULL));
uniform_real_distribution<> rn(0.0, 1.0);
uniform_int_distribution<> rnd(0, kappa-1);
vector<vector<int> >zeroes;
int zeroes_vec_cnt;

void init(){
	outf.open("testaxliopas.txt");
	zeroes_vec_cnt = 0; 
		zeroes.clear();
		zeroes.resize(kappa, vector<int> (2,-1));
	cout<<zeroes.size() << "  SIZE SIZE SIZE"<< endl;
    for (int i=0; i <N; i++){    
        for (int j=0; j<N; j++){  	
        	if(stuff[i][j]==0){
 				zeroes[zeroes_vec_cnt][0] = i;
				zeroes[zeroes_vec_cnt][1] = j;
				zeroes_vec_cnt++;
		}
	}
    }
}

void stuff_output (std::string msg ){
	outf << msg << endl;
	cout << "  ";
	for (int i=0; i<stuff.size(); i++){
		cout << i << " ";
	}
	cout << endl << "  -------------------" << endl;
	for (int i=0; i<stuff.size(); i++){
		cout << i << "|";
		for (int j=0; j<stuff[i].size(); j++){
			cout << stuff[i][j] << " ";
			outf << '\t' << stuff[i][j];
		}
		cout << endl;
		outf << '\t' << endl;
	}
	cout << endl;
}

int point_chooser (){   //int gia na epistrefei poio tha kratisei
	uniform_int_distribution<> rndPnt(0, zeroes_vec_cnt - 1);
	pointA=rndPnt(gen);									
	do {							
		pointB=rndPnt(gen);
	}while( pointB==pointA );
	int sum_point[1][2];	//pointA --> sum_point[0][0]    |   pointB --> sum_point[0][1]
	int connections[4];
	for (int l=0; l<4; l++ ){
		connections[l]=-2;
	}
	for(int c=0; c<2; c++){		//midenizw tous pinakes
		sum_point[0][c]=1;
	}
	for(int cc=0; cc<2; cc++){	
		if(cc==0){
			x=zeroes[pointA][0];
			y=zeroes[pointA][1];
		}
		else{
			x=zeroes[pointB][0];
			y=zeroes[pointB][1];
		}
		if((x-1 >= 0) && (stuff[x-1][y] != 0)){		
			if(connections[0]==-2){
				connections[0]=L[stuff[x-1][y]];
				sum_point[0][cc]=sum_point[0][cc] * S[L[stuff[x-1][y]]];
			}
		}
		if((y+1 < N) && (stuff[x][y+1] != 0)){		

			if(connections[0] != L[stuff[x][y+1]]){
				connections[1]=L[stuff[x][y+1]];
				sum_point[0][cc]=sum_point[0][cc] * S[L[stuff[x][y+1]]];
			}
		}
		if((x+1 < N) && (stuff[x+1][y] != 0)){		
			if(L[stuff[x+1][y]] != connections[0] && L[stuff[x+1][y]] != connections[1] ){
				connections[2]=L[stuff[x+1][y]];
				sum_point[0][cc]=sum_point[0][cc] * S[L[stuff[x+1][y]]];
			}
		}
		if((y-1 >= 0) && (stuff[x][y-1] != 0)){		//elegxos aristera
			if(L[stuff[x][y-1]] != connections[0] && L[stuff[x][y-1]] != connections[1] && L[stuff[x][y-1]] != connections[2] ){
				connections[3]=L[stuff[x][y-1]];
				sum_point[0][cc]=sum_point[0][cc] * S[L[stuff[x][y-1]]];
			}
		}
	}
	if(sum_point[0][0] < sum_point[0][1]){
		point=pointA;
	}
	else{
		point=pointB;
	}
	return point;
}


void cluster_finder(){
	point=point_chooser();
	x=zeroes[point][0];
	y=zeroes[point][1];
	int first_label=-1;
	if((x-1 >= 0) && (stuff[x-1][y] != 0)){
		if(first_label==-1){
			first_label=L[stuff[x-1][y]];
			S[L[stuff[x-1][y]]]++;
			stuff[x][y]=first_label;
		}
	}
	if((y+1 < N) && (stuff[x][y+1] != 0)){	
		if(first_label==-1){
			first_label=L[stuff[x][y+1]];
			S[L[stuff[x][y+1]]]++;
			stuff[x][y]=first_label;
		}
		else{
			if(first_label != L[stuff[x][y+1]]){		
				int tempL=L[stuff[x][y+1]];
				stuff[x][y+1]=first_label;
				for (int w=0; w<Lcount; w++){
					if(L[w]==tempL){
						L[w]=first_label;
					}
					S[L[first_label]]=S[L[first_label]]+S[tempL];
					S[tempL]=0;
				}
			}
		}
	}
	if((x+1 < N) && (stuff[x+1][y] != 0)){	
		if(first_label==-1){
			first_label=L[stuff[x+1][y]];
			S[L[stuff[x+1][y]]]++;
			stuff[x][y]=first_label;
		}
		else{
			if(first_label != L[stuff[x+1][y]] ){
			int tempL=L[stuff[x+1][y]];
			stuff[x+1][y]=first_label;
			for (int w=0; w<Lcount; w++){
				if(L[w]==tempL){
					L[w]=first_label;
				}
			}
			S[L[first_label]]=S[L[first_label]]+S[tempL];
			S[tempL]=0;
			}
		}
	}
	if((y-1 >= 0) && (stuff[x][y-1] != 0)){
		if(first_label==-1){
			first_label=L[stuff[x][y-1]];
			S[L[stuff[x][y-1]]]++;
			stuff[x][y]=first_label;
		}
		else{
			if(first_label != L[stuff[x][y-1]] ){
			int tempL=L[stuff[x][y-1]];
			stuff[x][y-1]=first_label;
			for (int w=0; w<Lcount; w++){
				if(L[w]==tempL){
					L[w]=first_label;
				}
			}
			S[L[first_label]]=S[L[first_label]]+S[tempL];
			S[tempL]=0;
			}
		}
	}
	if(first_label==-1){
		L[Lcount]=Lcount;
		stuff[x][y]=L[Lcount];
		S[L[stuff[x][y]]]++;
		Lcount++;
	}
	zeroes.erase(zeroes.begin() + point);		
	zeroes_vec_cnt=zeroes_vec_cnt - 1;		
}


void  percol_finder(double& SIav, double& SPmax, double& SIav2){
	int Smax=0;
	SIav=0;
	for (int i=0; i<SL; i++){
		SIav=SIav + 1.0*S[i]*S[i]/(N*N*dens);
		if (S[i]>Smax){
			Smax=S[i];
		}
	}
	SIav2=SIav-1.0*Smax*Smax/(N*N*dens);
	SPmax=Smax/(N*N*dens);
}


int main(){
	int M=1;
	percol.open("output2swmatidiwn_me_pollaplasiamo.txt");
	vector<double> index;
	long double Iav [10]
	long double	Pmax [10];
	long double Iav2 [10];
	long double dns [10];
	double dns2 [10];
	long double Iav0607 [10];
	long double Pmax0607 [10];
	long double Iav20607 [10];
	double dns0607 [10];
	for(int i=0; i<10; i++){
		dns[i]=i/10.0;
	}
	for(int i=0; i<10; i++){
		dns2[i]=0.8+i/100.0;
	}
	for(int i=0; i<10; i++){
		Iav[i]=0;
		Pmax[i]=0;
		Iav2[i]=0;
		Iav0607[i]=0;
		Pmax0607[i]=0;
		Iav20607[i]=0;
	}
	for(int xx=0; xx<M; xx++){
		Lcount=1;
		cout << "epanlipsi " << xx << endl;
		for (int i=0; i <N; i++){
        	for (int j=0; j<N; j++){
 				stuff[i][j]=0;
			}
	   }
	   	for (int i=0; i<SL; i++){
			L[i]=0;
			S[i]=0;
		}
		init ();
		for(int i=1; i<kappa; i++){
			cluster_finder();
			for(int j=0; j<10; j++){
				if(j/10.0==i/kappa){
					dens=j/10.0;
					percol_finder(Siav, Spmax, Siav2);
					Iav[j]=Iav[j]+Siav/M;
					Pmax[j]=Pmax[j]+Spmax/M;
					Iav2[j]=Iav2[j]+Siav2/M;
					cout <<dens<<endl;
				}
				double tempdns=0.8+j/100.0;
				double epsilon = 0.0000001;
				if(abs(tempdns - i/kappa*1.0) < epsilon){
					dens=(0.8+j/100.0);
					percol_finder(Siav, Spmax, Siav2);
					Iav0607[j]=Iav0607[j]+Siav/M;
					Pmax0607[j]=Pmax0607[j]+Spmax/M;
					Iav20607[j]=Iav20607[j]+Siav2/M;
					cout << dens<< endl;
				}
			}
		}
	}
	percol << "dns" << '\t' << "Iav" << '\t' << '\t' << "Pmax" << '\t' << '\t'  <<"Iav2"<<endl; // '\t' << '\t'   << "dns2" << '\t' << "Iav" << '\t' << '\t' << "Pmax" << '\t' << '\t'  <<"Iav2" << endl;
	for(int k=0; k<10; k++){
		percol   << dns[k] << '\t' << Iav[k] <<  '\t' << '\t' << Pmax[k]  << '\t' << '\t' << Iav2[k] <<endl; // '\t' << '\t' << dns2[k] << '\t' << Iav0607[k] <<  '\t' << '\t' << Pmax0607[k]  << '\t' << '\t' << Iav20607[k] << endl;
	}
	percol << endl;
	for(int k=0; k<10; k++){
		percol   << dns2[k] << '\t' << Iav0607[k] <<  '\t' << '\t' << Pmax0607[k]  << '\t' << '\t' << Iav20607[k] << endl;
	}

	outf<< "Labels "<<endl;
	for (int i=0; i<Lcount+100; i++){
		outf << L[i] << "  ";
	}
	outf<<"Sizes "<<endl;
	outf << endl;
	for (int i=0; i<L.size(); i++){
		outf << S[i] << "  ";
	}
	return 0;
}
