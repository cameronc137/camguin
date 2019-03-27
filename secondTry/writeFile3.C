#include "camguin.hh"
void writeFile3(TString valueName = "value", Double_t new_value = 0.0, Int_t new_runNumber = 0, Int_t new_nRuns = -1){
  // Store all trees
  TString aggregatorFileName = "run_aggregator.root";
  TString pwd                = gSystem->Getenv("PWD");
  Bool_t bNewFile            = gSystem->AccessPathName(pwd+"/"+aggregatorFileName); // Opposite return convention
  TFile *aggregatorFile      = new TFile(aggregatorFileName,"UPDATE");
  aggregatorFile->cd();
  TTree *oldTree;
  //TTree *copyTree;

  // Get environment variables
  new_runNumber = getRunNumber_h(new_runNumber);
  new_nRuns     = getNruns_h(new_nRuns);

  // Placeholder variables for reading of root file
  //Int_t    old_runNumber = 0;
  //Int_t    old_nRuns     = -1;
  Double_t old_runNumber = 0;
  Double_t old_nRuns     = -1;
  Double_t old_value     = -888888.1;

  // Store user passed values for testing and saving later
  //Int_t runNumber = new_runNumber;
  //Int_t nRuns     = new_nRuns;
  Double_t runNumber = (Double_t)new_runNumber;
  Double_t nRuns     = (Double_t)new_nRuns;
  Double_t value     = new_value;
  std::vector<Double_t> oldOtherValues;
  //std::vector<TBranch*> new_otherValuesBranch;
  std::vector<TString> branchList;
  //oldOtherValues.clear();
  //new_otherValuesBranch.clear();
  //branchList.clear();
  Int_t newBranch = 0;

  if (bNewFile) {
    // Write a new file
    oldTree = new TTree("agg","Aggregator Tree");
    Printf("Making new aggregator tree\n");
  }
  else {
    // Open existing file and copy the existing branches that aren't being
    // updated so that we can preserve the tree structure
    oldTree = (TTree*) aggregatorFile->Get("agg");
    Printf("Reading tree %s\n",(const char*)oldTree->GetName());
    TObjArray *aggVars = oldTree->GetListOfBranches();
    Printf("N entries = %d\n",aggVars->GetEntries());
    for ( Int_t b = 0; b<aggVars->GetEntries(); b++){
      TString found = (TString)(((TBranch*)(aggVars->At(b)))->GetName());
      branchList.push_back(found);
      Printf("In branch %s\n",(const char*)found);
      oldOtherValues.push_back(-349898.0);
    }

    for(Int_t iBranch = 0; iBranch < branchList.size(); iBranch++) {
      Printf("In branch %d : %s\n",iBranch,(const char*)branchList[iBranch]);
    }
    Printf("Got agg contents\n");
  }
  //getAggregateVars_h(oldTree,&branchList,&oldOtherValues); User method
  if (!(std::find(branchList.begin(),branchList.end(),valueName)!=branchList.end())){
    Printf("User adding new branch: %s\n",(const char*)valueName);
    addAggregateVars_h(valueName,&branchList,&oldOtherValues);
    //branchList.push_back(valueName);
    //oldOtherValues.push_back(-900000.1); // Vectors have to be initialized, and I don't know how many entries will come, so go for all of them
    newBranch = 1;
  }
  if (bNewFile) {
    addAggregateVars_h("run_number",&branchList,&oldOtherValues);
    //branchList.push_back("run_number");
    //oldOtherValues.push_back(-900000.1); // Vectors have to be initialized, and I don't know how many entries will come, so go for all of them
    addAggregateVars_h("n_runs",&branchList,&oldOtherValues);
    //branchList.push_back("n_runs");
    //oldOtherValues.push_back(-900000.1); // Vectors have to be initialized, and I don't know how many entries will come, so go for all of them
  }
  //  else{
  //    Printf("User adding new test branch\n");
  //    branchList.push_back("test");
  //    newBranch = 0;
  //  }
  //branchList.push_back("test");
  Printf("Appending to aggregator tree\n");

  for (Int_t k = 0; k < branchList.size(); k++){
  //for (std::vector::iterator it = branchList.begin(); it!= branchList.end(); ++it){
  //for (Double_t entry : otherValues){
    //if ((newBranch == 1 && branchList.at(k) == valueName) || ((branchList.at(k) != "run_number") && (branchList.at(k) != "n_runs") && (branchList.at(k) != valueName)))
    //if ((*it != "run_number") && (*it != "n_runs"))
    //if ((branchList[k] != "run_number") && (branchList[k] != "n_runs"))
    //{
      //if (branchList[k] != valueName) {
        Printf("Assigning other values to be saved, Iteration %d, branch name: %s, value: %f\n",k,(const char*) branchList[k],oldOtherValues[k]);
        //if ((newBranch == 1) && (branchList.at(k) != valueName)) {
        if (bNewFile){
Printf("test 3.1");
          oldTree->Branch(branchList[k],&oldOtherValues[k]);
        }
        else if (newBranch != 1){
Printf("test 3.2");
          oldTree->SetBranchAddress(branchList[k],&oldOtherValues[k]);
        }
        else {
          oldTree->Branch(branchList[k],&oldOtherValues[k]);
        }

Printf("test 4");
Printf("test 5");
  }
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

      //for (Int_t l = 0; l < branchList.size(); l++){

   /* for (Int_t l = 0; l < branchList.size(); l++)
    { // Loop over all entries and set to placeholder -999999.0, so the user notices missed entries
      if ((branchList.at(l) != "run_number") && (branchList.at(l) != "n_runs") && (branchList.at(l) != valueName)) { // Just checking for paranoia
        otherValues.at(l)=-999999.0;
      }
    }*/

    /*if (old_runNumber == runNumber && old_nRuns == nRuns){ // overwriting original entry if duplicate input 
      Printf("A: Overwriting runNumber %d, old value %f, new value %f\n",runNumber,old_value,value);
      newN = false;
      numEntries = numEntries-1; // No new entry will be added
      // Overwrite prior file entry with new results
      new_value     = value;
      new_nRuns     = nRuns;
      new_runNumber = runNumber;
    }*/
    //else if (entryN == numEntries && newN == true){ // Adding new entrys to the numEntries+1 entry then break out of loop
    if (entryN == numEntries && newN == true){ // Adding new entrys to the numEntries+1 entry then break out of loop
      Printf("B: Adding new runNumber %f, new value %f, new nRuns %f\n",runNumber,value,nRuns);
      added = true; // End Loop
      
      // Add new results
      //old_value     = value; // FIXME unnecessary?
      //old_nRuns     = nRuns;
      //old_runNumber = runNumber;
      //new_value     = value;
      //new_nRuns     = nRuns;
      //new_runNumber = runNumber;
      for (Int_t l = 0; l < branchList.size(); l++){
        if (branchList[l] == "run_number") oldOtherValues[l] = runNumber;
        if (branchList[l] == "n_runs")     oldOtherValues[l] = nRuns;
        if (branchList[l] == valueName)    oldOtherValues[l] = value;
      }
    /*}
    else {
      Printf("C: Copying old runNumber %d, old value %f, old nRuns %d\n",old_runNumber,old_value,old_nRuns);
      // Loop through and save the other prior results from the root file into the new copy of it
      if (newBranch == 1){
        new_value = -909090.1;
      }
      else {
        //new_value = old_value;
        value = old_value;
      }
      //new_nRuns = old_nRuns;
      //new_runNumber = old_runNumber;
      nRuns = old_nRuns;
      runNumber = old_runNumber;
    */
    }
    for (Int_t l = 0; l < branchList.size(); l++){
      //if ((branchList.at(l) != "run_number") && (branchList.at(l) != "n_runs") && (branchList.at(l) != valueName)) { // Just checking for paranoia
      if ((branchList[l] != "run_number") && (branchList[l] != "n_runs")) { // Just checking for paranoia
        if ((added == true) && (branchList[l] != valueName)) {
          Printf("A1, Storing new runnumber %d placeholder -999999 branch %d of other branch name %s\n",new_runNumber,l,(const char*)branchList[l]);
          Printf("A2, Adding placeholder new run values to other variables already known about\n");
          // Placeholder values of other variables not explicitly being set during this pass, go ahead and write them down if this is a fresh "new" run number 
          oldOtherValues[l] = -666666.1;
        }
        else if (branchList[l] != valueName) {
          Printf("B1, Storing old run %f unmodified %d branch %s\n",old_runNumber,l,(const char*)branchList[l]);
          //BROKEN FIXME this seems to not be saving the old extra branches to this vector, has to do with vector set address of newStorage
          oldOtherValues[l] = oldOtherValues[l]; // Just copy the prior value
          Printf("B2, Saved value = %f\n",oldOtherValues[l]);
        }
        if ((newBranch == 1) && (branchList[l] == valueName)) {
          Printf("C1, Storing new branch for branch %d prior run placeholder -999999 branch name %s\n",l,(const char*)branchList[l]);
          Printf("C2, Overwriting default initialized blank 0 with -999999 for run %f, branch %s\n",old_runNumber,(const char*)valueName);
          if (added == 0){
            oldOtherValues[l] = -777777.1; // Placeholder values of other variables which were previously left unset, loop through and write them when a "new" value appears in the root file, overwrite ^ else
          }
          else {
            oldOtherValues[l] = oldOtherValues[l];
          }
        }
      }
      else if (branchList[l] == "run_number"){
        oldOtherValues[l] = oldOtherValues[l];
      }
      else if (branchList[l] == "n_runs"){
        oldOtherValues[l] = oldOtherValues[l];
      }
    }
    if (added == true){
      oldTree->Fill();
    }
    if (entryN == numEntries && newN == false){ 
      Printf("End loop\n");
      added = true; // End Loop
    }
    entryN++;
  }
  if (bNewFile) {
    oldTree->Write("agg");
  }
  else {
    //oldTree->Write("agg",TObject::kWriteDelete,0);
    oldTree->Write("agg",TObject::kOverwrite,0);
  }
  aggregatorFile->Close();
}

