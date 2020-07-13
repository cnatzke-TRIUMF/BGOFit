//g++ bgo_fit.cxx -std=c++0x `root-config --cflags --libs` -lTreePlayer -lMathMore -lSpectrum -lMinuit -lPyROOT -o BGOFit

#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraph.h"
#include "TFile.h"
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

string gain_match_option[] = {
	"a", // A channels
	"b", // B channels
	"both" // Both
};

int search_array(string array[], string search, int len) {
	for (int i = 0; i < len; i++) {
		if (search == array[i]) {
			return i;
		} //if
	} //for
	return -1;
} //search_array

bool fitbgo(const char* inFile, const char* exp, const char* channels){

	string experiment; // To see if TIGRESS or GRIFFIN
	if( (strcmp(exp,"tigress") == 0 ) || (strcmp(exp,"Tigress") == 0 ) || (strcmp(exp,"TIGRESS") == 0 ) ) experiment = "tigress";
	else if( (strcmp(exp,"griffin") == 0 ) || (strcmp(exp,"Griffin") == 0 ) || (strcmp(exp,"GRIFFIN") == 0 ) ) experiment = "griffin";
	else {
		printf("Experiment Not Recognised, options are 'griffin' or 'tigress' \n");
		return false;
	}
	string chan;
	if( (strcmp(channels,"a") == 0 ) || (strcmp(channels,"A") == 0 ) ) chan = "a";
	else if( (strcmp(channels,"b") == 0 ) || (strcmp(channels,"B") == 0 ) ) chan = "b";
	else if( (strcmp(channels,"both") == 0 ) || (strcmp(channels,"Both") == 0 ) ) chan = "both";
	else {
		printf("Don't know which channels to gain match, options are 'a', 'b' or 'both' \n");
		return false;
	}

	int option = search_array(gain_match_option, chan, 3); // Checks whether a/b/both

	TH2F *g_sing[64][6];
	TH1D *g_pro[64][6];
	TFile *f = new TFile(inFile,"READ"); // Fits will go here
	f->cd();
	ofstream hv;
	hv.open("HV_Change.txt"); // Changes to HV go here.
	TList * list = new TList;
	switch(option) {
	case 0: // A channels
		for(int z=0; z<64; z++) {
			for(int i=0; i<5; i++) {
				int k = 1 + z/4;                        // Detector Number
				int cryNum = ( z % 4);
				char colour[1];
				if (cryNum == 0) sprintf(colour, "B");
				else if (cryNum == 1) sprintf(colour, "G");
				else if (cryNum == 2) sprintf(colour, "R");
				else if (cryNum == 3) sprintf(colour, "W");

				g_sing[z][i] = (TH2F*)f->Get(Form("h_%d_%d_%d",k,z,i));
				if(g_sing[z][i]->Integral(0,600,0,600) < 50) continue;      // If empty don't do anything
				g_pro[z][i] = g_sing[z][i]->ProjectionY();
				double max = g_pro[z][i]->GetMaximum()/100;
				if(max < 4) max = 4;                        // Works better with lower threshold to search
				int yAxis = g_pro[z][i]->GetXaxis()->GetBinCenter(g_pro[z][i]->FindLastBinAbove(max));
				int voltage = -(yAxis - 330)/66;
				if(voltage>8) voltage = 8;                  // Stops you changing the voltage too much as linearity
				if(voltage<-8) voltage = -8;                    // is only assumed over short range
				char var [64];
				if( experiment.compare("tigress") == 0) {
					cout << cryNum << "\t" << colour << endl;
					sprintf(var,"TIS%2.2i%sN%2.2iA",k,colour, (i+1));
				}
				else if ( experiment.compare("griffin") == 0) {
					sprintf(var,"GRS%2.2i%sN%2.2iA",k,colour, (i+1));
				}
				hv << var << "\t" << voltage*10 << endl;
				list->Add(g_pro[z][i]);
			}
		}
		break;
	case 1: // B channels
		for(int z=0; z<64; z++) {
			for(int i=0; i<5; i++) {
				int k = 1 + z/4;                        // Detector Number
				int cryNum = ( z % 4 );
				char colour[1];
				if (cryNum == 0) sprintf(colour, "B");
				else if (cryNum == 1) sprintf(colour, "G");
				else if (cryNum == 2) sprintf(colour, "R");
				else if (cryNum == 3) sprintf(colour, "W");

				g_sing[z][i] = (TH2F*)f->Get(Form("h_%d_%d_%d",k,z,i));
				if(g_sing[z][i]->Integral(0,600,0,600) < 50) continue;      // If empty don't do anything
				g_pro[z][i] = g_sing[z][i]->ProjectionY();
				double max = g_pro[z][i]->GetMaximum()/100;
				if(max < 4) max = 4;                        // Works better with lower threshold to search
				int yAxis = g_pro[z][i]->GetXaxis()->GetBinCenter(g_pro[z][i]->FindLastBinAbove(max));
				int voltage = -(yAxis - 330)/66;
				if(voltage>8) voltage = 8;                  // Stops you changing the voltage too much as linearity
				if(voltage<-8) voltage = -8;                    // is only assumed over short range
				char var [64];
				if( experiment.compare("tigress") == 0) {
					sprintf(var,"TIS%2.2i%sN%2.2iB",k,colour, (i+1));
				}
				else if ( experiment.compare("griffin") == 0) {
					sprintf(var,"GRS%2.2i%sN%2.2iB",k,colour, (i+1));
				}
				hv << var << "\t" << voltage*10 << endl;
				list->Add(g_pro[z][i]);
			}
		}
		break;
	case 2: // Both channels
		for(int z=0; z<64; z++) {
			for(int i=0; i<5; i++) {
				int k = 1 + z/4;                        // Detector Number
				int cryNum = ( z % 4);
				char colour[1];
				if (cryNum == 0) sprintf(colour, "B");
				else if (cryNum == 1) sprintf(colour, "G");
				else if (cryNum == 2) sprintf(colour, "R");
				else if (cryNum == 3) sprintf(colour, "W");

				g_sing[z][i] = (TH2F*)f->Get(Form("h_%d_%d_%d",k,z,i));
				if(g_sing[z][i]->Integral(0,600,0,600) < 50) continue;      // If empty don't do anything
				g_pro[z][i] = g_sing[z][i]->ProjectionY();
				double max = g_pro[z][i]->GetMaximum()/100;
				if(max < 4) max = 4;                        // Works better with lower threshold to search
				int yAxis = g_pro[z][i]->GetXaxis()->GetBinCenter(g_pro[z][i]->FindLastBinAbove(max));
				int voltage = -(yAxis - 660)/66;
				if(voltage>8) voltage = 8;                  // Stops you changing the voltage too much as linearity
				if(voltage<-8) voltage = -8;                    // is only assumed over short range
				voltage *= 5;                               // Changes both channels the same amount
				char var [64];
				if( experiment.compare("tigress") == 0) {
					sprintf(var,"TIS%2.2i%sN%2.2iA",k,colour, (i+1));
				}
				else if ( experiment.compare("griffin") == 0) {
					sprintf(var,"GRS%2.2i%sN%2.2iA",k,colour, (i+1));
				}
				hv << var << "\t" << voltage << endl;
				if( experiment.compare("tigress") == 0) {
					sprintf(var,"TIS%2.2i%sN%2.2iB",k,colour, (i+1));
				}
				else if ( experiment.compare("griffin") == 0) {
					sprintf(var,"GRS%2.2i%sN%2.2iB",k,colour, (i+1));
				}
				hv << var << "\t" << voltage << endl;
				list->Add(g_pro[z][i]);
			}
		}
		break;

	default:
		printf("Option not recognised\nKnown Options are: a, b, both\n");
		return false;
	}

	TFile *g = new TFile("BGO-Fits.root", "RECREATE"); // Fits go here
	g->cd();
	list->Write();
	g->Close();
	return true;
}
int main(int argc, char **argv){
	char const *inFile;
	char const *channel;
	char const *exp;

	if (argc < 4) {
		cout << "Insufficient arguments, please provide histograms, experiment name and channels to calibrate (A/B/Both)" << endl;
		return EXIT_FAILURE;
	} if (argc == 4) {
		inFile = argv[1];
		exp = argv[2];
		channel = argv[3];
	} else{
		cout << "Too many arguments, please provide histograms, experiment name and channels to calibrate (A/B/Both)" << endl;
	}
	printf("Input file:%s\nExperiment: %s\nChannels to gain Match: %s\n",inFile,exp,channel);
	fitbgo(inFile, exp, channel);
	return EXIT_SUCCESS;
} // end main
