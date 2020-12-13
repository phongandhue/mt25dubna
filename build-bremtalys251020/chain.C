#include "TChain.h"
#include "TLatex.h"

#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include "TString.h"
void chain(Int_t end=24,Int_t begin=0){
  TChain* ch = new TChain("BremSpec","BremSpec");
  for (Int_t i=begin;i<end;i++){
      char tempchar2[1000];
      sprintf(tempchar2,"result_t%i.root/BremSpec",i);
      ch->Add(tempchar2);
  }
}
