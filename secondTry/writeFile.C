#include <stdlib.h>
#include <iostream>
#include "camguin.hh"
using namespace std;
void writeFile(TString valueName = "value", Double_t new_value = 0.0, Int_t new_runNumber = 0, Int_t new_nRuns = -1){

  // Use camguin.hh methods to get environment variables
  new_runNumber = getRunNumber_h(new_runNumber);
  new_nRuns     = getNruns_h(new_nRuns);

  // Placeholder variables for reading of root file
  Int_t    old_runNumber = 0;
  Int_t    old_nRuns     = -1;
  Double_t old_value     = 0.0;

  // Store user passed values for testing and saving later
  Int_t runNumber = new_runNumber;
  Int_t nRuns     = new_nRuns;
  Double_t value     = new_value;

  TString aggregatorFileName = "run_aggregator.root";//_"+channel+".root";
  TString pwd                = gSystem->Getenv("PWD");
  Bool_t bNewFile            = gSystem->AccessPathName(pwd+"/"+aggregatorFileName); // Opposite return convention
  TFile *aggregatorFile      = new TFile(aggregatorFileName,"UPDATE");
  aggregatorFile->cd(); // FIXME how pe.cc does it. Is this necessary?
  TTree *oldTree;
  TTree *newTree;
  TTree *copyTree;
  if (bNewFile) {
    oldTree = new TTree("agg","Aggregator Tree");
    Printf("Making new aggregator tree\n");
    newTree = new TTree("agg","Aggregator Tree");
  }
  else {
    oldTree  = (TTree*) aggregatorFile->Get("agg");
    Printf("Appending to aggregator tree\n");
    oldTree->SetBranchAddress("run_number",&old_runNumber);
    oldTree->SetBranchAddress("n_runs",&old_nRuns);
    oldTree->SetBranchAddress(valueName,&old_value);
    oldTree->SetBranchStatus("*",1);
    copyTree = oldTree->CloneTree();
    copyTree->SetBranchStatus("*",1);
    copyTree->SetBranchStatus("run_number",0);
    copyTree->SetBranchStatus("n_runs",0);
    copyTree->SetBranchStatus(valueName,0);
    newTree = new TTree("agg","Aggregator Tree");
    newTree = copyTree->CloneTree(0);
  }
  ////////oldTree->SetName("old_agg");
  //TTree *newTree = oldTree->CloneTree(0);
  //newTree->AddFriend(oldTree);
  //newTree->CopyEntries(oldTree);


  TBranch *new_runNumberBranch = newTree->Branch("run_number",&new_runNumber);
  TBranch *new_nRunsBranch     = newTree->Branch("n_runs",    &new_nRuns);
  TBranch *new_dataBranch      = newTree->Branch(valueName,   &new_value);
  
  newTree->SetBranchStatus("*",1);

  // LOOP over all entries so far, find the one that I am adding if it is there, append otherwise, print all prior values in their respective places
  Int_t    numEntries = oldTree->GetEntries(); // Assume we add an entry
  Bool_t   added      = false;
  Int_t    entryN     = 0;
  Bool_t   newN       = true;
  //for (int i = 0; i < numEntries; i++) 
  while (added==false)
  { // Loop over the input file's entries and fill the new tree with the results of one of the leaves
    Printf("Entry number %d\n",entryN);
    oldTree->GetEntry(entryN);
    newTree->GetEntry(entryN);
    if (old_runNumber == runNumber && old_nRuns == nRuns){ // overwriting original entry or its the numEntries+1 entry (i.e. a new entry) then break out of loop
      Printf("A: Overwriting runNumber %d, old value %f, new value %f\n",runNumber,old_value,value);
      newN = false;
      numEntries = numEntries -1; // No new entry will be added
      // Overwrite prior file entry with new results
      new_value     = value;
      new_nRuns     = nRuns;
      new_runNumber = runNumber;
      newTree->Fill();
    }
    else if (entryN == numEntries && newN == true){ // overwriting original entry or its the numEntries+1 entry (i.e. a new entry) then break out of loop
      Printf("B: Adding new runNumber %d, new value %f, new nRuns %d\n",runNumber,value,nRuns);
      added = true; // End Loop
      // Add new results
      old_value     = value;
      old_nRuns     = nRuns;
      old_runNumber = runNumber;
      new_value     = value;
      new_nRuns     = nRuns;
      new_runNumber = runNumber;
      newTree->Fill();
    }
    else {
      Printf("C: Copying old runNumber %d, old value %f, old nRuns %d\n",old_runNumber,old_value,old_nRuns);
      // Loop through and save the prior results from the root file into the new copy of it
      new_value = old_value;
      new_nRuns = old_nRuns;
      new_runNumber = old_runNumber;
      newTree->Fill();
    }
    if (entryN == numEntries && newN == false){ 
      Printf("End loop\n");
      added = true; // End Loop
    }
    //old_runNumber = runNumberBranch->GetEntry(i);
    //tempNRuns = nRunsBranch->GetEntry(i);
    //tempValue = dataBranch->GetEntry(i);
    //new_runNumberBranch->Fill();
    //new_nRunsBranch->Fill();
    //new_dataBranch->Fill();
    entryN++;
  }

  // aggregatorFile = plotsTree->GetCurrentFile(); // FIXME how pe.cc does it
  if (bNewFile) {
      newTree->Write("agg");
  }
  else {
      newTree->Write("agg",TObject::kWriteDelete,0);
  }
  //TCanvas * c1 = new TCanvas(); // for displaying and saving some results
    newTree->Draw(valueName);
  //c1->SaveAs(valueName+".pdf");
  aggregatorFile->Close();
  //delete c1;
  //delete newTree;
  //delete aggregatorFile;
}
