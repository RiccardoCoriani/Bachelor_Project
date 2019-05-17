// c++ -o L alternata.cpp `root-config --cflags --glibs`

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

#define R 2.2e4

using namespace std;

	//funzione impedenza
double impedenza (double *w, double *par)	{
	return (par[0] * w[0]);
}

	//funzione sfasamento
double sfasamento (double *w, double *par)	{
	return atan(-1/(w[0]*par[0]*R));
}

	//funzione modulo
double mod (double x)	{
	if (x < 0) return x*(-1);
	else return x;
	}

int main (int argc, char** argv)	{
	TApplication *myApp = new TApplication ("myapp", 0, 0);
	double Va, ErrVa, Vb, ErrVb, Vba, ErrVba, Df, Df1, I, ErrI, w;
	char var[256];
	int i = 0;
	
	TGraphErrors *C = new TGraphErrors;
	TGraphErrors *C1 = new TGraphErrors;
	TH1F *F = new TH1F("F", "Sfasamento Df", 20, 1., 1.6);
	//TH1F *F1 = new TH1F("F1", "Sfasamento Df'", 10, 0., 0.7);
	
	//lettura file
	ifstream infile("L.txt");
	infile.getline (var, 256);
	double ErrX, ErrY, ErrY1;
	while (1)	{
 		infile >> Va >> ErrVa >> Vb >> ErrVb >> Vba >> ErrVba >>Df >> Df1 >> w;
 		if (infile.eof()) break;
		I = mod( Vb/R );
		ErrI = ErrVb/R;
		ErrY = sqrt((ErrVba*ErrVba)/(I*I) + (Vba/(I*I))*ErrI*ErrI);
		w = w*2*M_PI;
 		C->SetPoint(i, w, Vba/I);
		C->SetPointError(i, 0.01*2*M_PI, ErrY);
		C1->SetPoint(i, w, Df);
		C1->SetPointError(i, 0.001*2*M_PI, 0.1e-3);

		Df = w*Df;
		//Df1 = w*2*Df1*M_PI;

		F->Fill(Df);
		cout<<Df<<endl;
		//F1->Fill(Df1);	
 		i++;
 	}
	infile.close();
	
	gStyle->SetOptFit(1111);
		
		//fit
	TF1 *funz = new TF1 ("funz", impedenza, 0., 1500., 1);
		funz->SetParameter(0, 0.1);
		funz->SetParName(0, "L");
		funz->SetMarkerStyle(20);
 		funz->SetMarkerColor(kBlue);

	TF1 *sfasa = new TF1 ("sfasa", sfasamento, 0., 1500., 1);
		sfasa->SetParameter(0, 0);
		sfasa->SetParName(0, "L");
		sfasa->SetMarkerStyle(20);
 		sfasa->SetMarkerColor(kBlue);
 	
	C->GetXaxis()->SetTitle("w");
	C->GetYaxis()->SetTitle("Vba/I");

	C1->GetXaxis()->SetTitle("w");
	C1->GetYaxis()->SetTitle("Df");

	//F->gPad->SetLogX();
 	
 	TCanvas *c1 = new TCanvas("c1", "Alternata Vba");
 	c1->Divide(1,3);
	c1->cd(1);	
		C->Draw("AP");
		C->Fit("funz");	
	c1->cd(2);	
		C1->Draw("AP");
		//C1->Fit("sfasa");
		sfasa->Draw("same");
	c1->cd(3);
		F->Draw();
		F->Fit("gaus");

	
	c1->Print("altL_Df.png", "png");

	myApp->Run();
return 0;
}
