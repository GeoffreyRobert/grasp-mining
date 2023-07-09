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

void FSout::printset(int length, int *iset)
{
//#ifdef shown
  for(int i=0; i<length; i++)
    fprintf(out, "%d ", order_item[iset[i]]);
//#endif
}

void VectorOut::printSet(int length, int *iset, int support)
{
  std::vector<int> iset_vec(length);
  for(size_t i = 0; i < length; ++i)
  {
    iset_vec[i] = order_item[iset[i]];
  }
  iset_list.push_back(std::move(iset_vec));
  supp_list.push_back(support);
}

void VectorOut::printset(int length, int *iset)
{
  std::vector<int> iset_vec(length);
  for(size_t i = 0; i < length; ++i)
  {
    iset_vec[i] = order_item[iset[i]];
  }
  iset_list.push_back(std::move(iset_vec));
}

std::vector<std::vector<int>> VectorOut::GetItemsets()
{
  supp_list.clear();
  return std::move(iset_list);
}
