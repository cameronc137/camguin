#include <stdlib.h>
#include <iostream>
#include "camIO.hh"
#include "camHist.hh"
using namespace std;
void getMean_loop(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0", TString leaf = "hw_sum", Int_t runNumber = 0, Int_t nRuns = -1){
  runNumber = getRunNumber_h(runNumber);
  nRuns     = getNruns_h(nRuns);
  TString channel = tree + "_" + branch + "_" + leaf;
  // Make an instance of the relevant data source 
  TLeaf   *Leaf   = getLeaf_h(tree,branch,leaf);
  TBranch *Branch = Leaf->GetBranch();
  TTree   *Tree   = Branch->GetTree();
  Int_t    numEntries = Tree->GetEntries();

  Double_t data     = 0.0;
  Int_t    n_data   = 0;
  Double_t data_mean = 0.0;
  TString  analysis = "mean_"+channel;
  TBranch *dataBranch;

  for (int j = 0; j < numEntries; j++) 
  { // Loop over the input file's entries
    Tree->GetEntry(j);
    data+=Leaf->GetValue(0);
    n_data+=1;
  }
  data_mean = data/(1.0*n_data);
  writeFile_h(analysis,data_mean,runNumber,nRuns);
}
