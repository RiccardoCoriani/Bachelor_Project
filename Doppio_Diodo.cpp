//  c++ -o Doppio_Diodo Doppio_Diodo.cpp `root-config --cflags --glibs`

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <TCanvas.h>
#include <fstream>
#include <TApplication.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TPad.h>

using namespace std;

	//funzione CARATTERISTICA DEL DIODO
	
double caratteristica(double *V, double *par){

	double y=par[0] - par[1]*(  exp( ( 1.6e-19*V[0] ) / 4.11e-21 ) - 1)  - par[2]*(  exp(  (1.6e-19*V[0]) / 8.22e-21 ) - 1);
	return y;
}


int main (int argc, char** argv){
TApplication *myApp = new TApplication ("myapp", 0, 0);

	
	double I, V, I1,V1;
	
	int i = 0;
	
	TGraph *C = new TGraph;
	
	//lettura file
	ifstream infile("cellaSi.txt");
	
	
	while (1)	{
		infile >> V1 >> I1;
 		if (infile.eof()) break;

		if(I1<0 & V1>0){
				
			I = abs(I1);
			V=V1;					
			} 
 		C->SetPoint(i, V, I);

 		i++;
 	}
	infile.close();
	
	gStyle->SetOptFit(1111);
		
		//fit
		
	TF1 *funz = new TF1 ("funz", caratteristica, 0.,0.56 ,3);
	
	funz->SetParName(0,"Cost");
	funz->FixParameter(0, 0.3);
	funz->SetParName(1,"I_{01}");
	funz->SetParameter(1, 10^(-12));
	funz->SetParName(2,"I_{02}");
	funz->SetParameter(2, 10^(-7)); 

	C->GetXaxis()->SetTitle("V");
	C->SetMarkerStyle(2);
	C->GetYaxis()->SetTitle("I(V)");
 
 	TCanvas *c1 = new TCanvas("c1", "DoppioDiodo");
 	c1->SetGridx();
 	c1->SetGridy();
 	C->Draw("AP");
	C->Fit("funz","RM");
	
	//c1->Print("buio.png","png");
	
	myApp->Run();
return 0;
}
