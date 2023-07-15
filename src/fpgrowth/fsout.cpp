#include "fsout.h"
#include "common.h"

FSout::FSout(char *filename)
{
  out = fopen(filename,"wt");
}

FSout::~FSout()
{
  if(out) fclose(out);
}

int FSout::isOpen()
{
  if(out) return 1;
  else return 0;
}

void FSout::printSet(int length, int *iset, int support)
{
//#ifdef shown
  for(int i=0; i<length; i++)
  {
    fprintf(out, "%d ", order_item[iset[i]]);
//	printf("%d ", order_item[iset[i]]);
  }
  fprintf(out, "(%d)\n", support);
//  printf("(%d)\n", support);
//#endif
}

void VectorOut::printSet(int length, int *iset, int support)
{
  std::vector<int> iset_vec(length);
  for(size_t i = 0; i < length; ++i)
  {
    iset_vec[i] = order_item[iset[i]];
  }
  iset_list.emplace_back(support, std::move(iset_vec));
}

vector<pair<int, vector<int>>> VectorOut::GetItemsets()
{
  return std::move(iset_list);
}
