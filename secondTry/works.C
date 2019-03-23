#include <string>
using namespace std;
void getRunNumber(std::string runnumber){
// Get environment variable run number
  std::cout<<"test 1"<<std::endl;
  const char * runN = "0";//runnumber.c_str();
  //if ( std::strcmp("0","0") != 0 ) {// runnumber = gSystem->Getenv("RUNNUM");
  std::cout<<"test 2"<<std::endl;//}
  //std::cout<<"Run Number = "<<runnumber<<std::endl;
  std::cout<<"test 3"<<std::endl;
}
