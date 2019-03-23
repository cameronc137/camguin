#include <string>
using namespace std;
void getRunNumber(int runnumber = 0){
// Get environment variable run number
  std::cout<<"test 1"<<std::endl;
  //TString run = gSystem->Getenv("RUNNUM");
  //runnumber = run.Atoi();
  string run = gSystem->Getenv("RUNNUM");
  runnumber = run.Atoi();
  std::cout<<"test 2"<<std::endl;
  std::cout<<"Run Number = "<<runnumber<<std::endl;
  std::cout<<"test 3"<<std::endl;
}
