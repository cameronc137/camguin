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

TString getRootFile_h(Int_t runNumber = 0){
// Get environment variable rootfile name
  runNumber = getRunNumber_h(runNumber);
  TString rootfilename = " ";

  string fnmRoot="/adaq1/work1/apar/japanOutput";
  if(gSystem->Getenv("QW_ROOTFILES"))
    fnmRoot = gSystem->Getenv("QW_ROOTFILES");
  else
    //Printf("QW_ROOTFILES env variable was not found going with default: %s\n",fnmRoot.c_str());

  //Printf(" Looking for file with runNumber %d in %s\n",runNumber,fnmRoot.c_str());

  DIR *dirSearch;
  struct dirent *entSearch;
  const string daqConfigs[4] = {"CH","INJ","ALL","_tedf"};
  int found=0;
  string partialname = " ";
  if ((dirSearch = opendir (fnmRoot.c_str())) != NULL) {
    while ((entSearch = readdir (dirSearch)) != NULL) {
      for(int i=0;i<4;i++){
        partialname = Form("prex%s_%d.root",daqConfigs[i].c_str(),runNumber);
        //if(fMonitor)
        //  partialname = Form("prex%s_%d.adaq3",daqConfigs[i].c_str(),runNumber);
        std::string fullname = entSearch->d_name;
        if(fullname.find(partialname) != std::string::npos){
          rootfilename = fnmRoot + "/" + fullname;
          found++;
        }
      }
      if(found) break;
    }
    closedir (dirSearch);
  }

  if(found){
    //Printf("\t found file %s\n",(const char*)rootfilename);
    ////fRunNumber = runNumber;
  }else{
    //Printf("double check your configurations and files. Quitting\n");
  }
  return rootfilename;
}
      
TChain * getTree_h(TString tree = "mul", Int_t runNumber = 0, Int_t n_runs = -1, TString filenamebase = "Rootfiles/"){

  TString filename = "NULL";
  runNumber = getRunNumber_h(runNumber);
  n_runs    = getNruns_h(n_runs);
  filenamebase = gSystem->Getenv("QW_ROOTFILES");
  TChain *chain = new TChain(tree);

  for(Int_t i = 0; i < (n_runs); i++){

    //filenamebase = Form("%s/prex_tedf_%d.root",(const char *) filenamebase,runNumber+i);
    filenamebase = getRootFile_h(runNumber+i);
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

  // Get environment variables
  new_runNumber = getRunNumber_h(new_runNumber);
  new_nRuns     = getNruns_h(new_nRuns);

  // Placeholder variables for reading of root file
  Int_t    old_runNumber = 0;
  Int_t    old_nRuns     = -1;
  Double_t old_value     = 0.0;

  // Store user passed values for testing and saving later
  Int_t runNumber = new_runNumber;
  Int_t nRuns     = new_nRuns;
  Double_t value  = new_value;

  TString aggregatorFileName = "run_aggregator.root";
  TString pwd                = gSystem->Getenv("PWD");
  Bool_t bNewFile            = gSystem->AccessPathName(pwd+"/"+aggregatorFileName); // Opposite return convention
  TFile *aggregatorFile      = new TFile(aggregatorFileName,"UPDATE");
  aggregatorFile->cd();
  TTree *oldTree;
  TTree *newTree;
  TTree *copyTree;
  if (bNewFile) {
    // Write a new file
    oldTree = new TTree("agg","Aggregator Tree");
    //Printf("Making new aggregator tree\n");
    newTree = new TTree("agg","Aggregator Tree");
  }
  else {
    // Open existing file and copy the existing branches that aren't being
    // updated so that we can preserve the tree structure
    oldTree  = (TTree*) aggregatorFile->Get("agg");
    //Printf("Appending to aggregator tree\n");
    oldTree->SetBranchAddress("run_number",&old_runNumber);
    oldTree->SetBranchAddress("n_runs",&old_nRuns);
    oldTree->SetBranchAddress(valueName,&old_value);
    oldTree->SetBranchStatus("*",1);
    copyTree = oldTree->CloneTree();
    copyTree->SetBranchStatus("*",1); // FIXME This will cause new entries of unedited branches to = previous entry... ok, but be careful!!
    copyTree->SetBranchStatus("run_number",0);
    copyTree->SetBranchStatus("n_runs",0);
    copyTree->SetBranchStatus(valueName,0);
    newTree = new TTree("agg","Aggregator Tree");
    newTree = copyTree->CloneTree(0);
  }

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
    //Printf("Entry number %d\n",entryN);
    oldTree->GetEntry(entryN);
    newTree->GetEntry(entryN);
    if (old_runNumber == runNumber && old_nRuns == nRuns){ // overwriting original entry if duplicate input 
      //Printf("A: Overwriting runNumber %d, old value %f, new value %f\n",runNumber,old_value,value);
      newN = false;
      numEntries = numEntries -1; // No new entry will be added
      // Overwrite prior file entry with new results
      new_value     = value;
      new_nRuns     = nRuns;
      new_runNumber = runNumber;
      newTree->Fill();
    }
    else if (entryN == numEntries && newN == true){ // Adding new entryas the numEntries+1 entry then break out of loop
      //Printf("B: Adding new runNumber %d, new value %f, new nRuns %d\n",runNumber,value,nRuns);
      added = true; // End Loop
      // Add new results
      old_value     = value; // FIXME unnecessary?
      old_nRuns     = nRuns;
      old_runNumber = runNumber;
      new_value     = value;
      new_nRuns     = nRuns;
      new_runNumber = runNumber;
      newTree->Fill();
    }
    else {
      //Printf("C: Copying old runNumber %d, old value %f, old nRuns %d\n",old_runNumber,old_value,old_nRuns);
      // Loop through and save the other prior results from the root file into the new copy of it
      new_value = old_value;
      new_nRuns = old_nRuns;
      new_runNumber = old_runNumber;
      newTree->Fill();
    }
    if (entryN == numEntries && newN == false){ 
      //Printf("End loop\n");
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
