#include "camguin.hh"
void writeFile4(TString valueName = "value", Double_t new_value = 0.0, Int_t new_runNumber = 0, Int_t new_nRuns = -1){
  // Store all trees
  TString aggregatorFileName = "run_aggregator.root";
  TString pwd                = gSystem->Getenv("PWD");
  Bool_t newFile             = gSystem->AccessPathName(pwd+"/"+aggregatorFileName); // Opposite return convention
  TFile *aggregatorFile      = new TFile(aggregatorFileName,"UPDATE");
  aggregatorFile->cd();
  TTree *oldTree;
  TTree *newTree;

  // If the user adds an entry or branch then behave differently
  Bool_t newBranch = false;

  // Get environment variables
  new_runNumber = getRunNumber_h(new_runNumber);
  new_nRuns     = getNruns_h(new_nRuns);

  // Placeholder variables for reading of root file
  // Store the existing data in here per event
  std::vector<Double_t> tempValues;
  // Variables to be assigned to branch addresses
  std::vector<Double_t> oldValues;
  std::vector<Double_t> newValues;
  // List of branch names, append with new user additions
  std::vector<TString> branchList;
  std::vector<TBranch*>newBranches;

  if (newFile) {
    // Write a new file
    oldTree = new TTree("agg","Aggregator Tree");
    Printf("Making new aggregator tree");
    branchList.push_back("run_number");
    branchList.push_back("n_runs");
    newValues.push_back(-111000.1); // Vectors have to be initialized, and I don't know how many entries will come, so go for all of them
    newValues.push_back(-112000.1);
    tempValues.push_back(-112100.1);
    oldValues.push_back(-121000.1); 
    oldValues.push_back(-122000.1); 
    tempValues.push_back(-122100.1); 
  }
  else {
    // Open existing file 
    oldTree = (TTree*) aggregatorFile->Get("agg");
	  //if (oldTree->IsZombie()) { Printf("ERROR, tree agg is dead");}
	  if (!oldTree) {
      Printf("ERROR, tree agg is dead");
    }
    Printf("Making new aggregator tree");
    Printf("Reading tree %s",(const char*)oldTree->GetName());
    TObjArray *aggVars = oldTree->GetListOfBranches();
    Printf("N entries = %d",aggVars->GetEntries());
    for ( Int_t b = 0; b<aggVars->GetEntries(); b++){
      TString found = (TString)(((TBranch*)(aggVars->At(b)))->GetName());
      Printf("In branch %s",(const char*)found);
      branchList.push_back(found);
      newValues.push_back(-213000.0);
      oldValues.push_back(-223000.0);
      tempValues.push_back(-223100.0);
    }
    for(Int_t iBranch = 0; iBranch < branchList.size(); iBranch++) {
      Printf("In branch %d : %s",iBranch,(const char*)branchList[iBranch]);
    }
    Printf("Got agg contents");
  }

  // Maybe do this here...
  //newTree = (TTree*) aggregatorFile->Get("agg");
  newTree = new TTree("agg","Aggregator Tree");

  // Check to see if the value passed by the user to store exists yet, if not add it
  if (!(std::find(branchList.begin(),branchList.end(),valueName)!=branchList.end())){
    Printf("User adding new branch: %s",(const char*)valueName);
    //addAggregateVars_h(valueName,&branchList,&newValues,&oldValues);
    branchList.push_back(valueName);
    newValues.push_back(-314000.0);
    oldValues.push_back(-324000.0);
    tempValues.push_back(-324100.0);
    newBranch = true;
  }
  // Loop over branches and assign their addresses to old and new tree
  for (Int_t k = 0; k < branchList.size(); k++){
    Printf("Assigning values to be saved, Iteration %d, branch name: %s, manual blank initialization value: %f",k,(const char*) branchList[k],oldValues[k]);
  	// If this is a new file then generate new branches for old and new
  	Printf("Test 0.1, new value = %s",(const char*)valueName);
  	if (newFile || (newBranch && (branchList[k]==valueName))){
      oldTree->Branch(          branchList[k],&oldValues[k]); // Initialize a new branch, for placeholder purposes
  	}
  	else {
  	  oldTree->SetBranchAddress(branchList[k],&oldValues[k]); // Set old branch addresses
  	}
    newTree->Branch(          branchList[k],&newValues[k]); // Mane new branches
  }
  newTree->SetBranchStatus("*",1);
  oldTree->SetBranchStatus("*",1);

  // Loop over all entries so far, find the one that I am adding if it is there, append otherwise, print all prior values in their respective places with appropriate placeholder values
  Int_t  numEntries    = oldTree->GetEntries();
  Int_t  entryN        = 0;     // Looping variable
  // List of conditions
  Bool_t userAddedNewEntry  = true; // Assume we are adding a new entry
  Bool_t userEdittedOldEntry= false;
  Bool_t writeNewEntry      = false;
  Bool_t userAddedNewBranch = newBranch;
  Bool_t copyOldEntry       = false;
  Bool_t loopEnd            = false;

  // 2) If userEdittedOldEntry then numEntries--
  //      How to tell: if branchName == user branch name
  //      Also means user didn't add a new entry
  // 1) If userAddedNewEntry then numEntries..
  //      either case write userPassedVariables to newValues[]
  //      for non-userPassedVariables pass oldValues[] to newValues[]
  // 3) If copyOldEntry then continue
  //      for all variables copy oldValues[] to newValues[]

  Printf("Looking at %d entries",numEntries);
  while (entryN<=numEntries) {
    Printf("Examining Entry Number %d",entryN);
	  oldTree->GetEntry(entryN);
	  //newTree->GetEntry(entryN);
    
	  // Loop over all branches (FIXME (A) for the "new" user added value maybe initialize it differently?)
	  // Set the "old" values to placeholder values
    for (Int_t l = 0; l < branchList.size(); l++){
      Printf("NOTE: Examining branch %s = %f (old value)",(const char*) branchList[l],oldValues[l]);
	    if (userAddedNewEntry && entryN==numEntries) {
	      // Case 1
	  	  // We are appending a new value to the end, or initializing an empty new root file
        Printf("User adding new value to root file: branch %s, value (new = %f, old = %f) runnumber %d",(const char*)valueName,new_value,oldValues[l],new_runNumber);
        userEdittedOldEntry = false;
        copyOldEntry        = false;
  		  writeNewEntry       = true;
  	  }
	    // Check to see if we are on the requested new_runNumber, and if it is unique then behave differently
	    if ( (branchList[l] == "run_number") && (oldValues[l]==(Double_t)new_runNumber) ){
	    	// Case 2
        // We are replacing a prior entry
        Printf("User editting value in root file: branch %s, value (new = %f, old = %f) runnumber %d",(const char*)valueName,new_value,oldValues[l],new_runNumber);
		    userAddedNewEntry   = false;
        copyOldEntry        = false;
		    writeNewEntry       = true;
		    numEntries--;
	    }
    }
    for (Int_t l = 0; l < branchList.size(); l++){
      // If the user is currently writing an entry then assume all other values besides run_number and n_runs are not specified and leave them as oldValues initialization
  	  if (writeNewEntry){
  	    if ( branchList[l] == "run_number" ) { 
          Printf("NOTE: RunNumber %d getting written by user",new_runNumber);
  	      tempValues[l] = (Double_t)new_runNumber;
  	    }
  	    else if ( branchList[l] == "n_runs" ) {
          Printf("NOTE: new_nRuns %d getting written by user",new_nRuns);
  	      tempValues[l] = (Double_t)new_nRuns;
  	    }
  	    else if ( branchList[l] == valueName ) {
          Printf("NOTE: %s branch = %f getting written by user",(const char*) valueName,new_value);
          tempValues[l] = (Double_t)new_value;
	      }
	  	  else {
          Printf("NOTE: %s branch = %f getting written by user",(const char*) branchList[l],oldValues[l]);
          tempValues[l] = oldValues[l];
  		  }
        Printf("Saving new values, Branch name %s, value %f",(const char*)branchList[l],oldValues[l]);
      }
      else {
        Printf("Saving old values, Branch name %s, value %f",(const char*)branchList[l],oldValues[l]);
	      // Otherwise just save the oldValues
        tempValues[l] = oldValues[l];
	    }
	    newValues[l] = tempValues[l];
      Printf("Saving %s = %f, overwriting %f",(const char*)branchList[l],tempValues[l],oldValues[l]);
	  }
    // Reset the triggers for writing
    writeNewEntry       = false; 
 	  // And then be done writing the user passed input
    if (newFile || entryN<=numEntries){
	    newTree->Fill();  
    }
	  entryN++;
  }
  if (newFile) {
    newTree->Write("agg");
  }
  else {
    newTree->Write("agg",TObject::kWriteDelete,0);
  }
  newTree->Scan();
  aggregatorFile->Close();
}
