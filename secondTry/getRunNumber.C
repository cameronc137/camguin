#include <string>
using namespace std;
void getRunNumber(Int_t runnumber = 0){
	ofstream file_out1;
	file_out1.open("run_number.csv",std::ofstream::app);
// Get environment variable run number
  if ( runnumber == 0 ) 
  { 
    TString run = gSystem->Getenv("RUNNUM");
    runnumber = run.Atoi();
  }
	file_out1<<runnumber<<std::endl;
	file_out1.close();
  Printf("Run number: %d\n",runnumber);
}
