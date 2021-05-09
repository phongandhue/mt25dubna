#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TTree.h>
#include <iostream>
#include <fstream>
using namespace std;

void zzz(char* gndfile, char* isofile)
{
    TGraph* gr_flux = new TGraph("flux.txt","%lg %lg");
    gr_flux->Draw("AP*");
    TGraph* gr_gnd = new TGraph(gndfile,"%lg %lg %*lg");
    TGraph* gr_iso = new TGraph(isofile,"%lg %lg %*lg");
    Int_t ndata = gr_gnd->GetN();
    Double_t* E = gr_gnd->GetX();
    Double_t gnd_cs_sum = 0;
    Double_t iso_cs_sum = 0;


    for (int i=0;i<ndata;i++){
        cout<<E[i]<<"\t"<<gr_flux->Eval(E[i])<<"\t"<<gr_gnd->Eval(E[i])<<"\t"<<gr_iso->Eval(E[i])<<endl;
        gnd_cs_sum += gr_flux->Eval(E[i])*gr_gnd->Eval(E[i]);
        iso_cs_sum += gr_flux->Eval(E[i])*gr_iso->Eval(E[i]);
    }
    ofstream strout("ir.txt",std::ofstream::out | std::ofstream::app);
    strout<<gndfile<<"\t"<<iso_cs_sum/gnd_cs_sum<<endl;
    cout<<"\n\n*********\nIsomeric ratio = "<<iso_cs_sum/gnd_cs_sum<<endl;
}

