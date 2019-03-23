#include <stdlib.h>
#include <iostream>
#include "camguin.hh"
using namespace std;
void getLeaf(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0",TString leaf = "hw_sum"){
  Double_t  data    = 0;
  TString   dataID  = branch+"."+leaf;
  TChain  * Chain   = getTree_h();
  TBranch * Branch  = Chain->GetBranch(branch);
  TLeaf   * Leaf    = Branch->GetLeaf(leaf);
  Int_t  numEntries = Chain->GetEntries();
  printf("NumEntries = %d\n",numEntries);
  Printf("Found leaf: \"%s\"\n",(const char*)(tree+"."+branch+"."+leaf));
  for (int i = 0; i < numEntries; i++) {
    Chain->GetEntry(i);
    data = Leaf->GetValue(0);
    printf( "%s at event %d = %f\n", (const char*) dataID, i, data );
    //patternNoises   = patternAsyms.hw_sum;
    //patternBranches = fillPATTERNASYMS(patternAsyms);
    //newPatternTree->Fill();
  }
}
