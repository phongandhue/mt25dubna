#include "TChain.h"
#include "TLatex.h"

#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
void plotdef()
{
    Double_t y[]={2.5,4.22,3.7,2.35,2,3.2,3.7,3.3};
    Double_t x[8];
    for (Int_t i=0;i<8;i++)
        x[i]=i+0.5;

    TFile *_file0 = TFile::Open("result.root");
    TTree* br = (TTree*) _file0->Get("BremSpec");
    br->Draw("id>>h1(8,0,8)","","goff");
    TH1F* h1= (TH1F*)gDirectory->Get("h1");
    h1->Draw();
    Double_t scale = h1->GetMaximum();
    scale = scale/4.22;
    for (Int_t i=0;i<8;i++)
        y[i]=y[i]*scale;
    TGraph* gr=new TGraph(8,x,y);
    gr->SetFillColor(0);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(2);
    gr->Draw("PL same");
}




