void testTree(){//Int_t entry){
  TString aggregatorFileName = "run_aggregator.root";
  TString pwd                = gSystem->Getenv("PWD");
  Bool_t bNewFile            = gSystem->AccessPathName(pwd+"/"+aggregatorFileName);
  TFile *aggregatorFile      = new TFile(aggregatorFileName,"UPDATE");
  aggregatorFile->cd();
  TTree* oldTree;
  TTree* newTree;
  std::vector<TString> branchList;
  if (bNewFile) {
    oldTree = new TTree("agg","Aggregator Tree");
    Printf("Making new aggregator tree\n");
  }
  else {
    oldTree = (TTree*) aggregatorFile->Get("agg");
    //oldTree->SetName("old_agg");
    //newTree = oldTree->CloneTree();
    //oldTree->CopyAddresses(newTree, kFALSE);
  newTree = (TTree*) aggregatorFile->Get("agg");
    Printf("Reading tree %s\n",(const char*)oldTree->GetName());
    TObjArray *aggVars = oldTree->GetListOfBranches();
    Printf("N entries = %d\n",aggVars->GetEntries());//oldTree->GetListOfBranches()->GetEntries());
    for (Int_t b = 0; b<aggVars->GetEntries(); b++){
      Printf("Try %d\n",b);
      TString found = (TString)(((TBranch*)(aggVars->At(b)))->GetName());//oldTree->GetListOfBranches()->At(entry)->GetName();
      branchList.push_back(found);
      Printf("Found (%d) %s\n",b,(const char*)found);
    }
  }
}

