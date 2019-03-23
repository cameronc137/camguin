#include <stdlib.h>
#include "Riostream.h"
#include "TGraphErrors.h"
#include <stdio.h>
#include "TROOT.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

vector<vector<string>> CSVParse(TString fileName)
{
	//in.open(Form("%sbasic.dat",fileName.Data()));
	//ifstream in("Lab8Data.csv");
	ifstream in(fileName.Data());
	string line, field1, field2, field3, field4, field5;

	vector<vector<string> > array;  // the 2D array
	vector<string> v;                // array of values for one line only

	//getline(in,line); //skip header line

	while ( getline(in,line) )    // get next line in file
	{
		v.clear();
		stringstream ss(line);

		getline(ss,field1,',');  // break line into comma delimitted fields
		getline(ss,field2,',');
		getline(ss,field3,',');
		getline(ss,field4,',');
		ss << "\n";
		getline(ss,field5,'\n');
		//field5 = ss.str();

		//field1 = atof(field.c_str());
		v.push_back(field1);  // add each field to the 1D array
		v.push_back(field2);
		v.push_back(field3);
		v.push_back(field4);
		v.push_back(field5);

		array.push_back(v);  // add the 1D array to the 2D array
	}
	in.close();
	return array;
}
TCanvas * c1 = new TCanvas();
TMultiGraph *mg = new TMultiGraph();
int alreadyDrew = 0;
void plotConfigRatiosHelper(string file = "test.csv") {

	// Format: thickness,mm,5,0.99041,0.0301189
	vector<vector<string>> data = CSVParse(Form("%s",file.c_str()));
	int len = data.size();
	// Plot data
	//   for each entry in the vector, open the sub-vector and print the modifier to the plot name (with the units)
	//     plot the rest of the entries and error vs the number parameter
	//   done

	std::cout<<"length of vector = "<<len<<std::endl;
	double *runnumber = new double[len];
	double *value = new double[len];
	double *error = new double[len];
	/*for (int i=0; i<len; i++){
		std::stringstream iss3(data[i][2]);
		iss3 >> number[i];
		modifiers[i]=data[i][0];
		units[i]=data[i][1];
		std::stringstream iss4(data[i][3]);
		iss4 >> value[i];	
		std::stringstream iss5(data[i][4]);
		iss5 >> error[i];
	}*/
	for (int i=0; i<len; i++){
		std::stringstream iss3(data[i][0]);
		iss3 >> runnumber[i];
		std::stringstream iss4(data[i][1]);
		iss4 >> value[i];	
		std::stringstream iss5(data[i][2]);
		iss5 >> error[i];	
	}

	for (int j=0;j<len;j++){
		printf("%f,%f \n",runnumber[j],value[j]);
	}
	

		TGraphErrors * graph = new TGraphErrors(len,runnumber,value,0,error);
		//graph->SetTitle(Form("Plot of %s %s configurations, size %.1f to %.1f %s",fileName.c_str(),newModifiers[0].c_str(),numberMin,numberMax,newUnits[0].c_str()));
		graph->SetTitle(Form("Target stability per runs"));
		graph->SetMarkerColor(alreadyDrew+1);
		graph->SetMarkerStyle(21);
        //graph->Fit("pol1");
        //graph->SetLineColor(alreadyDrew+1);
        //graph->GetFunction("pol1")->SetLineColor(alreadyDrew+1);
        //gStyle->SetOptFit(0111);
		//graph->SetMarkerSize(.4);
		mg->Add(graph);
		mg->Draw("APE");
        mg->GetXaxis()->SetTitle(Form("Run Number"));//" %s (%s)",newModifiers[0].c_str(),newUnits[0].c_str()));
        mg->GetYaxis()->SetTitle(Form("T2s per uA "));
	mg->SetTitle(Form("Target Stability per run"));
	    //mg->GetXaxis()->SetTitle(Form("cylindrical can thickness (mils)"));//" %s (%s)",newModifiers[0].c_str(),newUnits[0].c_str()));
	    //mg->GetYaxis()->SetTitle("Relative to no SAMs, new E, new target position     ");
		alreadyDrew++;
        //c1->BuildLegend(0.125,0.8,0.5,0.9);
	//	c1->Update();
        //TPaveStats * stats = (TPaveStats*)graph->GetListOfFunctions()->FindObject("stats");
        //stats->SetTextColor(alreadyDrew);
        //stats->SetX1NDC(0.333*(alreadyDrew-1)); 
        //stats->SetX2NDC(0.333*alreadyDrew);
        //stats->SetY1NDC(0.9);
        //stats->SetY2NDC(1.0);
//		c1->Update();
		c1->SaveAs(Form("Plot_target_stability.pdf"));
	delete[] runnumber;
	delete[] value;
}

void plotTargetStability(int numLines = 0, string file1 = "thin", string file2 = "thick", string file3 = "allthick", string file4 = "allthick20", string file5 = "allthick30") {
    if (numLines==0){
        printf("USAGE: .x plotMultConfigRatios(int number of files,\"file1.csv\",\"file2.csv\"\n, etc.");
    }
    else{
        plotConfigRatiosHelper(file1);
        if (numLines>1){
            plotConfigRatiosHelper(file2);
        }
        if (numLines>2){
            plotConfigRatiosHelper(file3);
        }
        if (numLines>3){
            plotConfigRatiosHelper(file4);
        }
        if (numLines>4){
            plotConfigRatiosHelper(file5);
        }
    }
}
