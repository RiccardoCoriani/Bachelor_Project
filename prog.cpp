#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TAxis.h>
#include <cmath>

using namespace std;

double n(double l)
{
	double k = 1. + l*l/(0.0937*l*l - 0.008667);
	if (k < 0)
		return 0;
	else
		return sqrt(k);
}

int main(int argc, char** argv)
{
	gStyle -> SetOptFit(1111);
	
	ifstream dati("Si5.txt");
	
	TGraph *f = new TGraph();
	TGraph *g = new TGraph();
	
	int Npoints = 1126, i = 0;
	double x, y, R;
	
	while (1)
	{
		dati >> x;
		dati >> y;
		
		if (dati.eof() == 1)
			break;
		
		R = sqrt((n(x) - 1)/(n(x) + 1));
		
		f->SetPoint(i, x, y);
		g->SetPoint(i, x, R);
		
		i++;
	}
	dati.close();
	
	TCanvas *c1 = new TCanvas();
	f->GetXaxis()->SetTitle("#lambda[nm]");
	f->GetYaxis()->SetTitle("R (%)");
	f->SetMarkerStyle(7);
	c1->SetGridx();
	c1->SetGridy();
	f->Draw("AP");
	c1->Print("Si5.png", "png");
						//UNISCI F E G
	TCanvas *c2 = new TCanvas();
	g->GetXaxis()->SetTitle("#lambda[nm]");
	g->GetYaxis()->SetTitle("R (%)");
	g->SetMarkerStyle(7);
	g->SetMarkerColor(kRed);
	c2->SetGridx();
	c2->SetGridy();
	g->Draw("AP");
	c2->Print("Si55.png", "png");
	
	return 0;
}
