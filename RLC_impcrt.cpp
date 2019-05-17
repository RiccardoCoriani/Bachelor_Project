//  c++ -o RLC_impcrt RLC_impcrt.cpp `root-config --cflags --glibs`

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

#define R 2964
#define V0 14
using namespace std;

	//funzione per lo SMORZAMENTO CRITICO
	
/*double potenziale(double *t, double *par){

	double y=par[0]*t[0]*exp((-par[1])*t[0]);
	return y;
}*/

double potenziale(double *t, double *par){

	double y=exp(-par[0]*t[0]/(2*par[1]))*V0*par[2]*pow(par[0],3)*t[0]/(4*pow(par[1],2));
	return y;
}


int main (int argc, char** argv){
TApplication *myApp = new TApplication ("myapp", 0, 0);


	double V, T,errV,errT;
	char var[256];
	int i = 0;
	
	TGraphErrors *C = new TGraphErrors;
	
	//lettura file
	ifstream infile("RLC_impcrt.txt");
	infile.getline (var, 256);    // salta la prima riga
	while (1)	{
		infile >> V >> T >> errV >> errT;
 		if (infile.eof()) break;
 		C->SetPoint(i, T, V);
		C->SetPointError(i, errT, errV);
 		i++;
 	}
	infile.close();
	
	gStyle->SetOptFit(1111);
		
		//fit
		
	TF1 *funz = new TF1 ("funz", potenziale, 0., 1e-3, 3);

		funz->SetParName(0, "R");
		funz->FixParameter(0, R);
		
		funz->SetParName(1, "L");
		funz->SetParameter(1, 0.1);
		
		funz->SetParName(2, "C");
		funz->SetParameter(2, 4.7e-8);
		
		funz->SetLineStyle(20);
 		funz->SetLineColor(kBlue);
 	
	C->GetXaxis()->SetTitle("t");
	C->GetYaxis()->SetTitle("V(t)");
 	
 	TCanvas *c1 = new TCanvas("c1", "Alternata smorzamento critico");
 	c1->SetGridx();
 	c1->SetGridy();
	C->Draw("AP");
	C->Fit("funz","R");  // R= fitta sul range 0, 0.0013  M= mi da il minimo del chiquadro	
		
	myApp->Run();
return 0;
}
