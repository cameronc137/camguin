#include "camguin.hh"
using namespace std;
void camguin(){
  TH1* h1 = getHistogram_h("mul", "asym_sam8", "hw_sum", "default");
  h1->Draw();
  writeInt_leafHist_h( "evt", "bcm_an_ds3", "hw_sum", "default");
  writeMean_leafHist_h("evt", "bcm_an_ds3", "hw_sum", "default");
  writeRMS_leafHist_h( "evt", "bcm_an_ds3", "hw_sum", "default");
  writeMean_leafHist_h("mul", "asym_bcm_an_ds3", "hw_sum", "default");
  writeMean_leafHist_h("mul", "asym_sam1",  "hw_sum", "default");
  writeMean_leafHist_h("mul", "asym_sam2",  "hw_sum", "default");
  writeMean_leafHist_h("mul", "asym_sam3",  "hw_sum", "default");
  writeMean_leafHist_h("mul", "asym_sam4",  "hw_sum", "default");
  writeMean_leafHist_h("mul", "asym_sam5",  "hw_sum", "default");
  writeMean_leafHist_h("mul", "asym_sam6",  "hw_sum", "default");
  writeMean_leafHist_h("mul", "asym_sam7",  "hw_sum", "default");
  writeMean_leafHist_h("mul", "asym_sam8",  "hw_sum", "default");
}
