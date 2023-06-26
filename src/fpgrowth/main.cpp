#include <cstdlib>
#include <iostream>

#include "fpmax.h"
#include "data.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 3)
	{
	  cout << "usage: fmi <infile> <MINSUP> [<outfile>]\n";
	  exit(1);
	}

	FileData fdat(argv[1]);
	if(!fdat.isOpen()) {
		cerr << argv[1] << " could not be opened!" << endl;
		exit(2);
	}

	int threshold = atoi(argv[2]);
  char* outfile = NULL;
	if(argc == 4)
    outfile = argv[3];

  return fpmax(fdat, threshold, outfile);
}
