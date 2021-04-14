#include "TF1.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
TGraph* grc_Ce137g;
TGraph* grc_Ce137m;
TGraph* grflux;

Double_t grc_Ce137g_tf1_f(Double_t *x, Double_t *) { return grc_Ce137g->Eval(x[0]);}
Double_t grc_Ce137m_tf1_f(Double_t *x, Double_t *) { return grc_Ce137m->Eval(x[0]);}

Double_t grflux_tf1_f(Double_t *x, Double_t *) {
    if (x[0]>25000000) return 0;
    else
        return grflux->Eval(x[0]);
}

Double_t grProd_Ce137g_tf1_f(Double_t *x, Double_t *) {
    if (x[0]>25000000) {
        return 0;
    }else{
        return grc_Ce137g->Eval(x[0])*grflux->Eval(x[0]);
    }
}

Double_t grProd_Ce137m_tf1_f(Double_t *x, Double_t *) {
    if (x[0]>25000000) {
        return 0;
    }else{
        return grc_Ce137m->Eval(x[0])*grflux->Eval(x[0]);
    }
}



void cal()
{
    grc_Ce137g=new TGraph("talyscross_Ce137mg.txt","%lg %lg %*lg");
    grc_Ce137m=new TGraph("talyscross_Ce137mg.txt","%lg %*lg %lg");
//    grc_Ce137g->Draw("APL");
//    grc_Ce137m->SetLineColor(2);
//    grc_Ce137m->Draw("PLSAME");

    TF1* grc_Ce137g_tf1=new TF1("grc_Ce137g_tf1",grc_Ce137g_tf1_f,9000000,30000000);
    TF1* grc_Ce137m_tf1=new TF1("grc_Ce137m_tf1",grc_Ce137m_tf1_f,9000000,30000000);
    grc_Ce137g_tf1->SetNpx(2000);
    grc_Ce137m_tf1->SetNpx(2000);

    TFile* f=TFile::Open("flux.root");
    TH1F* hflux = (TH1F*)f->Get("hflux");
    //hflux->Draw();
    Int_t nbins = hflux->GetNbinsX();
    Double_t xx[500];
    Double_t yy[500];
    for (Int_t i=0;i<nbins;i++){
        xx[i] = hflux->GetBinCenter(i+1);
        yy[i] = hflux->GetBinContent(i+1)/hflux->GetBinWidth(1);
    }
    grflux = new TGraph(nbins,xx,yy);
    TF1* grflux_tf1=new TF1("grflux_tf1",grflux_tf1_f,9000000,30000000);


    TF1* grProd_Ce137g_tf1=new TF1("grProd_Ce137g_tf1",grProd_Ce137g_tf1_f,9000000,30000000);
    TF1* grProd_Ce137m_tf1=new TF1("grProd_Ce137m_tf1",grProd_Ce137m_tf1_f,9000000,30000000);


    //grc_Ce137g_tf1->Draw();
    //grc_Ce137m_tf1->Draw("same");
    grflux_tf1->Draw();
    grProd_Ce137g_tf1->Draw("same");
    grProd_Ce137m_tf1->Draw("same");

    cout<<grProd_Ce137g_tf1->Integral(10000000,25000000,0.0001)<<endl;
    cout<<grProd_Ce137m_tf1->Integral(10000000,25000000,0.0001)<<endl;
    cout<<grProd_Ce137m_tf1->Integral(10000000,25000000,0.0001)/grProd_Ce137g_tf1->Integral(10000000,25000000,0.0001)<<endl;


}
