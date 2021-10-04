#include <stdio.h>
#include <math.h>
#include <stdlib.h>
// c++ slicetime.cpp -o slicetime

static const int kMaxEPI3D = 1024; //maximum number of EPI images in Siemens Mosaic

struct TCSAdata {
	float sliceTiming[kMaxEPI3D];
};

struct TDICOMdata {
	struct TCSAdata CSA;
};

void sliceTimeGE (struct TDICOMdata * d, int mb, int dim3, float TR, bool isInterleaved, float geMajorVersion, bool is27r3, float groupDelaysec) {
//mb : multiband factor
//dim3 : number of slices in volume
//TRsec : repetition time in seconds
//isInterleaved : interleaved or sequential slice order
//geMajorVersion: version, e.g. 29.0
//is27r3 : software release 27.0 R03 or later
	float sliceTiming[kMaxEPI3D];
	//multiband can be fractional! 'extra' slices discarded
	int nExcitations = ceil(float(dim3) / float(mb));		
	if ((mb > 1) && (!is27r3) && ((nExcitations % 2) == 0) ) {//number of slices divided by MB factor should is Even
			nExcitations ++; //https://osf.io/q4d53/wiki/home/; Figure 3 of https://pubmed.ncbi.nlm.nih.gov/26308571/
		}
	int nDiscardedSlices = (nExcitations * mb) - dim3;
	float secPerSlice = (TR - groupDelaysec) / (nExcitations);
	if (!isInterleaved) {
		for (int i = 0; i < nExcitations; i++)
			sliceTiming[i] = i * secPerSlice;
	} else {
		int nOdd = (nExcitations - 1) / 2;
		for (int i = 0; i < nExcitations; i++) {
			if (i % 2 == 0) //ODD slices since we index from 0!
				sliceTiming[i] = (i/2) * secPerSlice;
			else
				sliceTiming[i] = (nOdd+((i+1)/2)) * secPerSlice;
		} //for each slice
		if ((mb > 1) && (is27r3) && (isInterleaved) && (nExcitations > 2) && ((nExcitations % 2) == 0)) {
			float tmp = sliceTiming[nExcitations - 1];
			sliceTiming[nExcitations - 1] = sliceTiming[nExcitations - 3];
			sliceTiming[nExcitations - 3] = tmp;
			//printf("SWAP!\n");
		}
	} //if interleaved
	for (int i = 0; i < dim3; i++)
		sliceTiming[i] = sliceTiming[i % nExcitations];
	for (int i = 0; i < dim3; i++)
		d->CSA.sliceTiming[i] = sliceTiming[i];
} // sliceTimeGE()


int main(int argc, char *argv[] ) {
	// printf() displays the string inside quotation
	struct TDICOMdata * d, dcm;
	d = &dcm; 
	int mb = 2;
	int dim3 = 10;
	float TRsec = 1.0;
	bool isInterleaved = true;
	bool isDescending = false;
	float geMajorVersion = 28.0;
	bool is27v3 = true;
	float groupDelay = 0;
	if( argc > 1 ) mb = atoi(argv[1]);  //multiband
	if( argc > 2 ) dim3 = atoi(argv[2]);  //slices
	if( argc > 3 ) TRsec = atof(argv[3]);  //TR
	if( argc > 4 ) isInterleaved = atoi(argv[4]);  //isInterleaved
	if( argc > 5 ) isDescending = atoi(argv[5]);  //isDescending
	if( argc > 6 ) geMajorVersion = atoi(argv[6]);  //isDescending
	if( argc > 7 ) is27v3 = atoi(argv[7]);  //is27v3
	if( argc > 8 ) groupDelay = atof(argv[8]);  //TR
	bool isVerbose = true;
	if (isVerbose) {
		printf("slicetime <mb> <slices> <tr> <isInterleaved> <isDescending> <geMajorVersion> <is27v3> <GroupDelay>\n");
		printf("Example:\n");
		printf(" slicetime %d %d %g %d %d %g %d %g\n", mb, dim3, TRsec, isInterleaved, isDescending, geMajorVersion, is27v3, groupDelay);
		printf("multiband: %d\n", mb);
		printf("slices: %d\n", dim3);
		printf("TR(sec): %g\n", TRsec);
		printf("isInterleaved: %d\n", isInterleaved);
		printf("isDescending: %d\n", isDescending);
		printf("geMajorVersion: %g\n", geMajorVersion);
		printf("is27v3: %d (GE Software 27.0_R03 or later)\n", is27v3);
		printf("groupDelay(sec): %g\n", groupDelay);
	}
	sliceTimeGE(d, mb, dim3, TRsec, isInterleaved, geMajorVersion, is27v3, groupDelay);
	if (isDescending) {
		float swap;
		for (int i = 0; i < (dim3 / 2); i++) {
			swap = dcm.CSA.sliceTiming[i];
			dcm.CSA.sliceTiming[i] = dcm.CSA.sliceTiming[dim3 - 1 - i];
			dcm.CSA.sliceTiming[dim3 - 1 - i] = swap;
		}
	}
	if ((mb > 1) && (geMajorVersion < 26.0)) {
		printf("WARNING: Unable to determine slice times for early GE HyperBand (version: %g)\n", geMajorVersion);
		return 0;
	}
	for (int i = 0; i < dim3; i++)
		printf("%g\n", dcm.CSA.sliceTiming[i]); 
	return 0;
	
}