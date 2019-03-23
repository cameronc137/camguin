#include <stdlib.h>
#include <iostream>
#include "camguin.hh"
using namespace std;
void getBranch(TString branch = "mul", TString filenamebase = "Rootfiles/", Int_t runnumber = 0, Int_t n_runs = 0){

  TString filename = "NULL";
  runnumber = getRunNumber_h();
  filenamebase = gSystem->Getenv("QW_ROOTFILES");
  TChain *tree = new TChain(branch);

  for(Int_t i = 0; i <= (n_runs); i++){

    filenamebase = Form("%s/prex_tedf_%d.root",(const char *) filenamebase,runnumber+i);
    filename=filenamebase;
    
    filenamebase.Remove(filenamebase.Last('.'),5);
    
    int split = 0;
    while ( !gSystem->AccessPathName(filename.Data()) ) {
      Printf("File added to Tree: \"%s\"\n",(const char*)filename);
      tree->Add(filename);
      split++;
      filename = filenamebase + "_" + split + ".root";
    }
  }
  printf("N Entries: %d\n",(int)tree->GetEntries());
}
