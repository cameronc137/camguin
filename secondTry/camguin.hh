#include <string>
using namespace std;
Int_t getRunNumber_h(Int_t runnumber = 0){
// Get environment variable run number
  if ( runnumber == 0 ) 
  { 
    TString run = gSystem->Getenv("RUNNUM");
    runnumber = run.Atoi();
  }
  //Printf("Run number: %d\n",runnumber);
  return runnumber;
}

Int_t getNruns_h(Int_t n_runs = -1){
// Get environment variable run number
  if ( n_runs == -1 ) 
  { 
    TString nRuns = gSystem->Getenv("NRUNS");
    n_runs = nRuns.Atoi();
  }
  //Printf("Number of Runs: %d\n",n_runs);
  return n_runs;
}

TChain * getTree_h(TString tree = "mul", TString filenamebase = "Rootfiles/", Int_t runnumber = 0, Int_t n_runs = 0){

  TString filename = "NULL";
  runnumber = getRunNumber_h();
  //n_runs    = getNruns_h();
  filenamebase = gSystem->Getenv("QW_ROOTFILES");
  TChain *chain = new TChain(tree);

  for(Int_t i = 0; i <= (n_runs); i++){

    filenamebase = Form("%s/prex_tedf_%d.root",(const char *) filenamebase,runnumber+i);
    filename=filenamebase;
    
    filenamebase.Remove(filenamebase.Last('.'),5);
    
    int split = 0;
    while ( !gSystem->AccessPathName(filename.Data()) ) {
      //Printf("File added to Chain: \"%s\"\n",(const char*)filename);
      chain->Add(filename);
      split++;
      filename = filenamebase + "_" + split + ".root";
    }
  }
  //printf("N Entries: %d\n",(int)chain->GetEntries());
  return chain;
}
TLeaf * getLeaf_h(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0",TString leaf = "hw_sum"){
  //Printf("Found leaf: \"%s\"\n",(const char*)(tree+"."+branch+"."+leaf));
  TChain  * Chain   = getTree_h();
  TBranch * Branch  = Chain->GetBranch(branch);
  TLeaf   * Leaf    = Branch->GetLeaf(leaf);
  return Leaf;
}
