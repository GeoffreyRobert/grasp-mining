#ifndef _FSOUT_CLASS
#define _FSOUT_CLASS

#include <cstdio>
#include <vector>

class OutData
{
public:
  virtual ~OutData() {};
  virtual void printset(int length, int *iset) = 0;
  virtual void printSet(int length, int *iset, int support) = 0;
};

class FSout : public OutData
{
 public:

  FSout(char *filename);
  ~FSout();

  int isOpen();

  void printset(int length, int *iset) override;
  void printSet(int length, int *iset, int support) override;

 private:
  FILE *out;
};

class VectorOut : public OutData
{
public:
  void printset(int length, int *iset) override;
  void printSet(int length, int *iset, int support) override;

private:
  std::vector<std::vector<int>> iset_list;
  std::vector<int> supp_list;
};

#endif

