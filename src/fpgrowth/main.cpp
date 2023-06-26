#include <cstdlib>
#include <iostream>

#include "fpmax.h"
#include "data.h"
#include "fsout.h"

using namespace std;

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    cout << "usage: fmi <infile> <MINSUP> [<outfile>]\n";
    exit(1);
  }

  FileData filedata(argv[1]);
  if(!filedata.isOpen()) {
    cerr << argv[1] << " could not be opened!" << endl;
    exit(2);
  }

  int threshold = atoi(argv[2]);
  FSout* fout = NULL;
  if(argc == 4)
  {
    fout = new FSout(argv[3]);
  }

  return fpmax(filedata, threshold, fout);
}
