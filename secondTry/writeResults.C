#include <stdlib.h>
#include <iostream>
#include "camguin.hh"
using namespace std;
void writeResults(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0", TString leaf = "hw_sum"){
  TString channel = tree + "_" + branch + "_" + leaf;
  // Make an instance of the relevant data source 
  TLeaf   *Leaf   = getLeaf_h();
  TBranch *Branch = Leaf->GetBranch();
  TTree   *Tree   = Branch->GetTree();
  Int_t    numEntries = Tree->GetEntries();
  TTree   *newTree;
  TFile   *aggregatorFile;
  TString  aggregatorFileName = "run_aggregator.root";//_"+channel+".root";
  aggregatorFile = new TFile(aggregatorFileName,"UPDATE");
  newTree = new TTree("agg","Aggregator Tree");
  
  // FIXME This is where the data obtained from analysis methods gets read and stored, data, nData, and avg are placeholder
  
  Double_t data     = 0; 
  Int_t    n_data   = 0;
  Double_t data_avg = 0;
  TString  analysis = channel+"_average";
  TBranch *dataBranch;
  dataBranch = newTree->Branch(analysis, &data_avg);

  // end placeholder

  //dataBranch = newTree->Branch(channel, &data);
  for (int j = 0; j < numEntries; j++) 
  { // Loop over the input file's entries and fill the new tree with the results of one of the leaves
    Tree->GetEntry(j);
    data+=Leaf->GetValue(0);
    n_data+=1;
    //newTree->Fill();
  }
  data_avg = data/(1.0*n_data);
  newTree->Fill();
  aggregatorFile->Write();
  //TCanvas * c1 = new TCanvas();
  newTree->Draw(analysis);
  //c1->SaveAs(analysis+".pdf");
  delete newTree;
  aggregatorFile->Close();
  delete aggregatorFile;
  //delete c1;
}
