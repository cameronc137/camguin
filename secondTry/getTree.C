#include <stdlib.h>
#include <iostream>
#include "camguin.hh"
using namespace std;
void getTree(TString tree = "mul", TString filenamebase = "Rootfiles/", Int_t runNumber = 0, Int_t n_runs = 0){

  TString filename = "NULL";
  runNumber = getRunNumber_h(runNumber);
  n_runs    = getNruns_h(n_runs);
  filenamebase = gSystem->Getenv("QW_ROOTFILES");
  TString fileNameBase  = filenamebase; // placeholder string
  TChain *chain = new TChain(tree);
  Bool_t foundFile = false;

  for(Int_t i = 0; i < (n_runs); i++){

    TString daqConfigs[4] = {"CH","INJ","ALL","_tedf"};
    for(Int_t j=0;j<4;j++){
      filenamebase = Form("%s/prex%s_%d.root",(const char *)fileNameBase,(const char *)daqConfigs[j],runNumber+i);
      filename=filenamebase;
      //Printf("Trying file name: %s\n",(const char*)filenamebase);
      if ( !gSystem->AccessPathName(filename.Data()) ) {
        //Printf("Found file name: %s\n",(const char*)filenamebase);
        foundFile = true;
        j=5; // Exit loop
      }
    }
    filename=filenamebase;
    filenamebase.Remove(filenamebase.Last('.'),5);
    
    int split = 0;
    while ( !gSystem->AccessPathName(filename.Data()) ) {
      //Printf("File added to chain: \"%s\"\n",(const char*)filename);
      chain->Add(filename);
      split++;
      filename = filenamebase + "_" + split + ".root";
    }
  }
  if (!foundFile){
    Printf("Rootfile not found in %s with runs from %d to %d, check your config and rootfiles",(const char*)fileNameBase,runNumber,runNumber+n_runs-1);
    return 0;
  }
  printf("N Entries: %d\n",(int)chain->GetEntries());
}
