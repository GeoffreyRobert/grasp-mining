#include <cstdlib>
#include <iostream>

#include "fpmax.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 3)
	{
	  cout << "usage: fmi <infile> <MINSUP> [<outfile>]\n";
	  exit(1);
	}
  char* infile = argv[1];
	int threshold = atoi(argv[2]);
  char* outfile = NULL;
	if(argc == 4)
    outfile = argv[3];

  return fpmax(infile, threshold, outfile);
}
