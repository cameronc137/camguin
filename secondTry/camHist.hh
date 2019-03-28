using namespace std;
TH1 * rebinTH1_h(TH1 * h1, TString mode = "manual", Int_t lookAbove = 0, Int_t lookAround = 0, Int_t nBinsX = 0, Int_t sampleFactor = 0, Double_t lowBinValX = 0.0, Double_t highBinValX = 0.0){

  //gROOT->SetBatch(kTRUE);

  if (sampleFactor == 0){
    sampleFactor = 1;
  }
  if (nBinsX == 0){
    nBinsX = (Int_t)(h1->GetNbinsX()/sampleFactor);
  }
  if (lowBinValX == 0.0 && highBinValX == 0.0){
    lowBinValX  = h1->GetBinCenter(h1->FindFirstBinAbove(lookAbove,TH1::kXaxis) - lookAround);
    highBinValX = h1->GetBinCenter(h1->FindLastBinAbove(lookAbove,TH1::kXaxis)  + lookAround);
    //Printf(Form("Low: %f",lowBinValX));//  = h1->GetBinContent(h1->FindFirstBinAbove(lookAbove,1) - lookAround);
    //Printf(Form("high: %f",highBinValX));//highBinValX = h1->GetBinContent(h1->FindLastBinAbove(lookAbove,1)  + lookAround);
  }

  TString title    = h1->GetTitle();
  TString xTitle   = h1->GetXaxis()->GetTitle();
  TString yTitle   = h1->GetYaxis()->GetTitle();
  TString name     = h1->GetName();
  Int_t old_nBinsX = h1->GetNbinsX();

  TH1 *h2 = new TH1F((const char*)(name+"_rebin"),(const char*)("Rebinned: "+title),nBinsX,lowBinValX,highBinValX);
  //Printf(Form("name = %s_rebin, title = Rebinned: %s, nBinsX = %d, lowBinValX = %f, highBinValX = %f",(const char*)name,(const char*)title,nBinsX,lowBinValX,highBinValX));
  h2->GetXaxis()->SetTitle(xTitle);h2->GetXaxis()->CenterTitle();
  h2->GetYaxis()->SetTitle(yTitle);h2->GetYaxis()->CenterTitle();

  // Generate empty Hist
  if (mode == "clean"){
    //Printf("clean\n");
    h2->GetXaxis()->SetCanExtend(false);
  }
  else if (mode == "manual"){
    //Printf("manual\n");
    h2->GetXaxis()->SetCanExtend(true);
  }
  // Loop rebin, down or upsample
  else if (mode == "loop"){
    //Printf("loop\n");
    for (Int_t bin = 0; bin < old_nBinsX; bin++) {
      h2->Fill(h1->GetBinCenter(bin));
    }
  }
  // Auto rebin, down sample by factor sampleFactor
  else if (mode == "auto"){
    //Printf("auto\n");
    h2 = h1->Rebin(sampleFactor,name+"_rebin");
  }

  return h2;
}

TH1 * getHistogram_h(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0", TString leaf = "hw_sum", TString mode = "default", Int_t runNumber = 0, Int_t nRuns = -1){
  runNumber           = getRunNumber_h(runNumber);
  nRuns               = getNruns_h(nRuns);
  TString channel     = tree + "_" + branch + "_" + leaf;
  // Make an instance of the relevant data source 
  TLeaf   *Leaf       = getLeaf_h(tree,branch,leaf,runNumber,nRuns);
  if (!Leaf){
    return 0;
  }
  TString leafName    = branch+"."+(TString)Leaf->GetName();
  TBranch *Branch     = Leaf->GetBranch();
  TTree   *Tree       = Branch->GetTree();
  Int_t    numEntries = Tree->GetEntries();

  gROOT->SetBatch(kTRUE);
  //Printf("Leaf name: %s",(const char*)leafName);
  Tree->Draw(Form("%s>>h1",(const char*)leafName));
  TH1 *h1 = (TH1*)gDirectory->Get("h1");
  TH1 *h2 = new TH1F();
  
  if (mode == "default"){
    return h1;
  }
  else if (mode == "clean" || mode == "manual"){
    h2 = rebinTH1_h(h1,mode,2,1,1000); // example use case of rebinTH1_h method
    TString h2_name = h2->GetName();
    Tree->Draw(Form("%s>>%s",(const char*)leafName,(const char*)h2_name)); // Manual
  }
  else if (mode == "auto" || mode == "loop"){
    h2 = rebinTH1_h(h1,mode,2,1,1000); // example use case of rebinTH1_h method
  }

  //Printf("Run %d histogram of branch %s returned",runNumber,(const char*)leafName);
  return h2; 
}

void writeInt_leafHist_h(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0", TString leaf = "hw_sum", TString mode = "default", Int_t runNumber = 0, Int_t nRuns = -1){
  TString integral = "integral_" + branch + "_" + leaf;
  Double_t data_integral = 0.0;
	data_integral = getHistogram_h(tree,branch,leaf,mode,runNumber,nRuns)->Integral();

  //Printf("Run %d integral %s: %f",runNumber,(const char*)integral,data_integral);
  writeFile_h(integral,data_integral,runNumber,nRuns);
}

void writeMean_leafHist_h(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0", TString leaf = "hw_sum", TString mode = "default", Int_t runNumber = 0, Int_t nRuns = -1){
  TString mean = "mean_" + branch + "_" + leaf;
  Double_t data_mean = 0.0;
  data_mean = getHistogram_h(tree,branch,leaf,mode,runNumber,nRuns)->GetMean();

  //Printf("Run %d mean %s: %f",runNumber,(const char*)mean,data_integral);
  writeFile_h(mean,data_mean,runNumber,nRuns);
}

void writeRMS_leafHist_h(TString tree = "mul", TString branch = "asym_vqwk_04_0ch0", TString leaf = "hw_sum", TString mode = "default", Int_t runNumber = 0, Int_t nRuns = -1){
  TString  rms = "rms_" + branch + "_" + leaf;
  Double_t data_rms = 0.0;
	data_rms = getHistogram_h(tree,branch,leaf,mode,runNumber,nRuns)->GetRMS();

  //Printf("Run %d RMS %s: %f",runNumber,(const char*)rms,data_integral);
  writeFile_h(rms,data_rms,runNumber,nRuns);
}
