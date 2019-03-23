#include <stdlib.h>
#include <iostream>
#include "camguin.hh"
using namespace std;
void getLeaf(TString branch = "mul", TString leaf = "vqwk_04_0ch0",TString subleaf = "hw_sum"){
  TString data_id = leaf+"."+subleaf;
  TChain *tree = getBranch_h(branch);
  Int_t numEntries = tree->GetEntries();
  printf("NumEntries = %d\n",numEntries);
  TTreeReader reader(branch,&tree);
  TTreeReaderValue<float> data2(reader,data_id);
  Double_t data = 0;
  tree->SetBranchAddress( data_id, &data );
  for (int j = 0; j < numEntries/100; j++) {
    // Loop over the input file's entries and fill the new tree with the results of one of the leaves
    tree->GetEntry();
    printf( "%s at event %d = %f\n", (const char*) data_id, j, data );
    printf( "%s at event %d = %f\n", (const char*) data_id, j, data2 );
    //patternNoises   = patternAsyms.hw_sum;
    //patternBranches = fillPATTERNASYMS(patternAsyms);
    //newPatternTree->Fill();
  }
}
