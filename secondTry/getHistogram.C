#include <stdlib.h>
#include <iostream>
#include "camIO.hh"
#include "camHist.hh"
using namespace std;
void getHistogram(TString mode = "default", TString tree = "mul", TString branch = "asym_vqwk_04_0ch0", TString leaf = "hw_sum", Int_t runNumber = 0, Int_t nRuns = -1){
  runNumber           = getRunNumber_h(runNumber);
  nRuns               = getNruns_h(nRuns);
  TString channel     = tree + "_" + branch + "_" + leaf;
  // Make an instance of the relevant data source 
  TLeaf   *Leaf       = getLeaf_h(tree,branch,leaf,runNumber,nRuns);
  if (!Leaf){
    return 0;
  }
  TString leafName    = branch+"."+(TString)Leaf->GetName();
  TBranch *Branch     = Leaf->GetBranch();
  TTree   *Tree       = Branch->GetTree();
  Int_t    numEntries = Tree->GetEntries();

  Double_t data_integral = 0.0;

  gROOT->SetBatch(kTRUE);
  Printf("Leaf name: %s",(const char*)leafName);
  Tree->Draw(Form("%s>>h1",(const char*)leafName));
  TH1 *h1 = (TH1*)gDirectory->Get("h1");
  
  if (mode == "default"){
	  data_integral = h1->GetMean();
  }
  else if (mode == "clean" || mode == "manual"){
    TH1 *h2 = rebinTH1_h(h1,mode,2,1,1000); // example use case of rebinTH1_h method
    TString h2_name = h2->GetName();
    Tree->Draw(Form("%s>>%s",(const char*)leafName,(const char*)h2_name)); // Manual
	  data_integral = h2->GetMean();
  }
  else if (mode == "auto" || mode == "loop"){
    TH1 *h2 = rebinTH1_h(h1,mode,2,1,1000); // example use case of rebinTH1_h method
	  data_integral = h2->GetMean();
  }

  Printf("Run %d histogram of branch %s returned",runNumber,(const char*)leafName);
  TCanvas * c2 = new TCanvas();
  h2->Draw();
}
