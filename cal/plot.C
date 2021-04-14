#include "TGraph.h"
void plot()
{
	TGraph* gr=new TGraph("test.txt","%lg %lg");
	TGraph* gr2=new TGraph("test2.txt","%lg %lg");	
	TGraph* gr3=new TGraph("test3.txt","%lg %lg");	
	gr3->Draw("APL");
	//gr->SetLineColor(2);
	//gr->Draw("PLsame");
}
