using namespace std;
void getAvg_Loop_h(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0", TString leaf = "hw_sum", Int_t runNumber = 0, Int_t nRuns = -1){
  runNumber = getRunNumber_h(runNumber);
  nRuns     = getNruns_h(nRuns);
  TString channel = tree + "_" + branch + "_" + leaf;
  // Make an instance of the relevant data source 
  TLeaf   *Leaf   = getLeaf_h(tree,branch,leaf,runNumber,nRuns);
  TBranch *Branch = Leaf->GetBranch();
  TTree   *Tree   = Branch->GetTree();
  Int_t    numEntries = Tree->GetEntries();

  Double_t data     = 0.0;
  Int_t    n_data   = 0;
  Double_t data_avg = 0.0;
  TString  analysis = "avg_"+channel;
  TBranch *dataBranch;

  for (int j = 0; j < numEntries; j++) 
  { // Loop over the input file's entries
    Tree->GetEntry(j);
    data+=Leaf->GetValue(0);
    n_data+=1;
  }
  data_avg = data/(1.0*n_data);
  writeFile_h(analysis,data_avg,runNumber,nRuns);
}

void getAvg_leafHist_h(TString mode = "default", TString tree = "mul", TString branch = "asym_vqwk_04_0ch0", TString leaf = "hw_sum", Int_t runNumber = 0, Int_t nRuns = -1){
  runNumber = getRunNumber_h(runNumber);
  nRuns     = getNruns_h(nRuns);

  // Make an instance of the relevant data source 
  Printf("test1");
  TLeaf   *Leaf   = getLeaf_h(tree,branch,leaf,runNumber,nRuns);
  TString leafName = branch+"."+(TString)Leaf->GetName();
  TBranch *Branch = Leaf->GetBranch();
  TTree   *Tree   = Branch->GetTree();
  Int_t    numEntries = Tree->GetEntries();

  TString channel = tree + "_" + branch + "_" + leaf;
  Double_t data_avg = 0.0;
  Double_t data_rms = 0.0;
  TString  avg = "avg_"+channel;
  TString  rms = "rms_"+channel;

  gROOT->SetBatch(kTRUE);
  Printf("Leaf name: %s",(const char*)leafName);
  Tree->Draw(Form("%s>>h1",(const char*)leafName));
  TH1 *h1 = (TH1*)gDirectory->Get("h1");
  
  if (mode == "default"){
	  data_avg = h1->GetMean();
	  data_rms = h1->GetRMS();
  }
  else if (mode == "clean" || mode == "manual"){
    TH1 *h2 = rebinTH1(h1,mode,2,1,1000); // example use case of rebinTH1 method
    TString h2_name = h2->GetName();
    Tree->Draw(Form("%s>>%s",(const char*)leafName,(const char*)h2_name)); // Manual
	  data_avg = h2->GetMean();
	  data_rms = h2->GetRMS();
  }
  else if (mode == "auto" || mode == "loop"){
    TH1 *h2 = rebinTH1(h1,mode,2,1,1000); // example use case of rebinTH1 method
	  data_avg = h2->GetMean();
	  data_rms = h2->GetRMS();
  }

  Printf("Run %d avg %s: %f",runNumber,(const char*)avg,data_avg);
  Printf("Run %d rms %s: %f",runNumber,(const char*)rms,data_rms);
  writeFile_h(avg,data_avg,runNumber,nRuns);
  writeFile_h(rms,data_rms,runNumber,nRuns);
  //writeFile_h("test",1.0,runNumber,nRuns);
}
