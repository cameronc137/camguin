#include <utility>
#include <vector>
#include <TString.h>
#include "TCut.h"
#include <list>
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;
Int_t getRunNumber_h(Int_t runNumber = 0){
// Get environment variable run number
  if ( runNumber == 0 ) 
  { 
    TString run = gSystem->Getenv("RUNNUM");
    runNumber = run.Atoi();
  }
  //Printf("Run number: %d\n",runNumber);
  return runNumber;
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

std::vector<TString> getAggregateVars_h(TTree * rootTree) 
{

  // Utility to find all of the variables (leaf's/branches) within a
  // Specified TTree and put them within the aggVars vector.
  // It is possible to generalize this up a level to loop over 
  // a vector full of TTrees, but for now lets assume just one tree
  std::vector<TString> aggVars;
  aggVars.clear();

  TObjArray *branchList = rootTree->GetListOfBranches();
  TIter next(branchList);
  TBranch *brc;

  while((brc=(TBranch*)next())!=0) {
    TString found = brc->GetName();
    // Not sure if the line below is so smart...
    aggVars.push_back(found);
  }

  //for(Int_t iBranch = 0; iBranch < aggVars.size(); iBranch++) {
    //Printf("In branch %d : %s\n",iBranch,(const char*)aggVars[iBranch]);
  //}
  return aggVars;
}
      
TChain * getTree_h(TString tree = "mul", Int_t runNumber = 0, Int_t n_runs = -1, TString filenamebase = "Rootfiles/"){

  TString filename = "NULL";
  runNumber = getRunNumber_h(runNumber);
  n_runs    = getNruns_h(n_runs);
  filenamebase = gSystem->Getenv("QW_ROOTFILES");
  TString fileNameBase  = filenamebase; // placeholder string
  TChain *chain = new TChain(tree);

  for(Int_t i = 0; i < (n_runs); i++){

    TString daqConfigs[4] = {"CH","INJ","ALL","_tedf"};
    for(Int_t j=0;j<4;j++){
      filenamebase = Form("%s/prex%s_%d.root",(const char *)fileNameBase,(const char *)daqConfigs[j],runNumber+i);
      //Printf("Trying file name: %s\n",(const char*)filenamebase);
      if ( !gSystem->AccessPathName(filename.Data()) ) {
        //Printf("Found file name: %s\n",(const char*)filenamebase);
        j=5; // Exit loop
      }
    }
    //filenamebase = getRootFile_h(runNumber+i);
    filename     = filenamebase;
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
TBranch * getBranch_h(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0", Int_t runNumber = 0, Int_t nRuns = -1, TString filenamebase = "Rootfiles/"){
  //Printf("Found leaf: \"%s\"\n",(const char*)(tree+"."+branch+"."+leaf));
  runNumber = getRunNumber_h(runNumber);
  nRuns     = getNruns_h(nRuns);
  TChain  * Chain   = getTree_h(tree, runNumber, nRuns, filenamebase);
  TBranch * Branch  = Chain->GetBranch(branch);
  return Branch;
}

TLeaf * getLeaf_h(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0",TString leaf = "hw_sum", Int_t runNumber = 0, Int_t nRuns = -1, TString filenamebase = "Rootfiles/"){
  //Printf("Found leaf: \"%s\"\n",(const char*)(tree+"."+branch+"."+leaf));
  runNumber = getRunNumber_h(runNumber);
  nRuns     = getNruns_h(nRuns);
  TChain  * Chain   = getTree_h(tree, runNumber, nRuns, filenamebase);
  TBranch * Branch  = Chain->GetBranch(branch);
  TLeaf   * Leaf    = Branch->GetLeaf(leaf);
  return Leaf;
}

void writeFile_h(TString valueName = "value", Double_t new_value = 0.0, Int_t new_runNumber = 0, Int_t new_nRuns = -1){
  // Store all trees
  TString aggregatorFileName = "run_aggregator.root";
  TString pwd                = gSystem->Getenv("PWD");
  Bool_t bNewFile            = gSystem->AccessPathName(pwd+"/"+aggregatorFileName); // Opposite return convention
  TFile *aggregatorFile      = new TFile(aggregatorFileName,"UPDATE");
  aggregatorFile->cd();
  TTree *oldTree;
  TTree *newTree;
  //TTree *copyTree;

  // Get environment variables
  new_runNumber = getRunNumber_h(new_runNumber);
  new_nRuns     = getNruns_h(new_nRuns);

  // Placeholder variables for reading of root file
  Int_t    old_runNumber = 0;
  Int_t    old_nRuns     = -1;
  Double_t old_value     = -888888.1;

  // Store user passed values for testing and saving later
  Int_t runNumber = new_runNumber;
  Int_t nRuns     = new_nRuns;
  Double_t value  = new_value;
  std::vector<Double_t> oldOtherValues;
  std::vector<Double_t> newOtherValues;
  std::vector<TBranch*> new_otherValuesBranch;
  std::vector<TString> branchList;
  oldOtherValues.clear();
  newOtherValues.clear();
  new_otherValuesBranch.clear();
  branchList.clear();
  Int_t newBranch = 0;

  if (bNewFile) {
    // Write a new file
    oldTree = new TTree("agg","Aggregator Tree");
    Printf("Making new aggregator tree\n");
    newTree = new TTree("agg","Aggregator Tree");
  }
  else {
    // Open existing file and copy the existing branches that aren't being
    // updated so that we can preserve the tree structure
    oldTree    = (TTree*) aggregatorFile->Get("agg");
    branchList = getAggregateVars_h(oldTree);
    if (!(std::find(branchList.begin(),branchList.end(),valueName)!=branchList.end())){
      Printf("User adding new branch: %s\n",(const char*)valueName);
      branchList.push_back(valueName);
      newBranch = 1;
    }
  //  else{
  //    Printf("User adding new test branch\n");
  //    branchList.push_back("test");
  //    newBranch = 0;
  //  }
    //branchList.push_back("test");
    Printf("Appending to aggregator tree\n");
    //copyTree = oldTree->CloneTree();
    //copyTree->SetBranchStatus("*",1); // FIXME This will cause new entries of unedited branches to = previous entry... ok, but be careful!!
    //copyTree->SetBranchStatus("run_number",0);
    //copyTree->SetBranchStatus("n_runs",0);
    //copyTree->SetBranchStatus(valueName,0);
    newTree = new TTree("agg","Aggregator Tree");
    //newTree = copyTree->CloneTree(0);

    for (Int_t k = 0; k < branchList.size(); k++){
    //for (std::vector::iterator it = branchList.begin(); it!= branchList.end(); ++it){
    //for (Double_t entry : otherValues){
Printf("test 1");
      newOtherValues.push_back(-899999.1);
      oldOtherValues.push_back(-900000.1); // Vectors have to be initialized, and I don't know how many entries will come, so go for all of them
      //if ((newBranch == 1 && branchList.at(k) == valueName) || ((branchList.at(k) != "run_number") && (branchList.at(k) != "n_runs") && (branchList.at(k) != valueName)))
Printf("test 2");
      //if ((*it != "run_number") && (*it != "n_runs"))
      if ((branchList[k] != "run_number") && (branchList[k] != "n_runs"))
      {
        if (branchList[k] != valueName) {
          Printf("Assigning other values to be saved, Iteration %d, branch name: %s, value: %f\n",k,(const char*) branchList[k],newOtherValues[k]);
          //if ((newBranch == 1) && (branchList.at(k) != valueName)) {
Printf("test 3");
          oldTree->SetBranchAddress(branchList[k],&oldOtherValues[k]);
          new_otherValuesBranch.push_back(newTree->Branch(branchList.at(k),&newOtherValues.at(k)));
Printf("test 4");
          //newTree->Branch(branchList[k],&newOtherValues[k]);
        }
Printf("test 5");
        //else {
        //  TBranch* aNewBranch = newTree->Branch(valueName, &newOtherValues.at(k));
        //  new_otherValuesBranch.push_back(aNewBranch);
        //}
      }
    }
    oldTree->SetBranchAddress("run_number",&old_runNumber);
    oldTree->SetBranchAddress("n_runs",&old_nRuns);
    oldTree->SetBranchAddress(valueName,&old_value);
  }
  //newTree->Branch("run_number",&new_runNumber);
  //newTree->Branch("n_runs",    &new_nRuns);
  //newTree->Branch(valueName,   &new_value);
  TBranch *new_runNumberBranch = newTree->Branch("run_number",&new_runNumber);
  TBranch *new_nRunsBranch     = newTree->Branch("n_runs",    &new_nRuns);
  TBranch *new_dataBranch      = newTree->Branch(valueName,   &new_value);
  newTree->SetBranchStatus("*",1);
  oldTree->SetBranchStatus("*",1);


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

   /* for (Int_t l = 0; l < branchList.size(); l++)
    { // Loop over all entries and set to placeholder -999999.0, so the user notices missed entries
      if ((branchList.at(l) != "run_number") && (branchList.at(l) != "n_runs") && (branchList.at(l) != valueName)) { // Just checking for paranoia
        otherValues.at(l)=-999999.0;
      }
    }*/

    if (old_runNumber == runNumber && old_nRuns == nRuns){ // overwriting original entry if duplicate input 
      Printf("A: Overwriting runNumber %d, old value %f, new value %f\n",runNumber,old_value,value);
      newN = false;
      numEntries = numEntries-1; // No new entry will be added
      // Overwrite prior file entry with new results
      new_value     = value;
      new_nRuns     = nRuns;
      new_runNumber = runNumber;
    }
    else if (entryN == numEntries && newN == true){ // Adding new entrys to the numEntries+1 entry then break out of loop
      Printf("B: Adding new runNumber %d, new value %f, new nRuns %d\n",runNumber,value,nRuns);
      added = true; // End Loop
      // Add new results
      old_value     = value; // FIXME unnecessary?
      old_nRuns     = nRuns;
      old_runNumber = runNumber;
      new_value     = value;
      new_nRuns     = nRuns;
      new_runNumber = runNumber;
    }
    else {
      Printf("C: Copying old runNumber %d, old value %f, old nRuns %d\n",old_runNumber,old_value,old_nRuns);
      // Loop through and save the other prior results from the root file into the new copy of it
      if (newBranch == 1){
        new_value = -909090.1;
      }
      else {
        new_value = old_value;
      }
      new_nRuns = old_nRuns;
      new_runNumber = old_runNumber;
    }
    for (Int_t l = 0; l < branchList.size(); l++){
      //if ((branchList.at(l) != "run_number") && (branchList.at(l) != "n_runs") && (branchList.at(l) != valueName)) { // Just checking for paranoia
      if ((branchList[l] != "run_number") && (branchList[l] != "n_runs")) { // Just checking for paranoia
        if ((added == true) && (branchList[l] != valueName)) {
          Printf("A1, Storing new runnumber %d placeholder -999999 branch %d of other branch name %s\n",new_runNumber,l,(const char*)branchList[l]);
          Printf("A2, Adding placeholder new run values to other variables already known about\n");
          //newOtherValues.at(l) = -666666.0; // Placeholder values of other variables not explicitly being set during this pass, go ahead and write them down if this is a fresh "new" run number 
          newOtherValues[l] = -666666.1;
        }
        else if (branchList[l] != valueName) {
          Printf("B1, Storing old run %d unmodified %d branch %s\n",old_runNumber,l,(const char*)branchList[l]);
          //BROKEN FIXME this seems to not be saving the old extra branches to this vector, has to do with vector set address of newStorage
          newOtherValues[l] = oldOtherValues[l]; // Just copy the prior value
          //newOtherValues.at(l) = -555555.1;
          Printf("B2, Prior saved value = %f\n",oldOtherValues[l]);
          Printf("B3, New saved value = %f\n",newOtherValues[l]);
        }
        if ((newBranch == 1) && (branchList[l] == valueName)) {
          Printf("C1, Storing new branch for branch %d prior run placeholder -999999 branch name %s\n",l,(const char*)branchList[l]);
          Printf("C2, Overwriting default initialized blank 0 with -999999 for run %d, branch %s\n",old_runNumber,(const char*)valueName);
          newOtherValues[l] = -777777.1; // Placeholder values of other variables which were previously left unset, loop through and write them when a "new" value appears in the root file, overwrite ^ else
        }
      }
    }
    newTree->Fill();
    if (entryN == numEntries && newN == false){ 
      Printf("End loop\n");
      added = true; // End Loop
    }
    entryN++;
  }
  if (bNewFile) {
    newTree->Write("agg");
  }
  else {
    newTree->Write("agg",TObject::kWriteDelete,0);
  }
  aggregatorFile->Close();
}
