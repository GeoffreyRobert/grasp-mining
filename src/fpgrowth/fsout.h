#ifndef _FSOUT_CLASS
#define _FSOUT_CLASS

#include <cstdio>
#include <vector>

class OutData
{
public:
  virtual ~OutData() {};
  virtual void printSet(int length, int *iset, int support) = 0;
};

class FSout : public OutData
{
public:
  FSout(char *filename);
  ~FSout();
  int isOpen();
  void printSet(int length, int *iset, int support) override;

private:
  FILE *out;
};

using std::vector; using std::pair;

class VectorOut : public OutData
{
public:
  void printSet(int length, int *iset, int support) override;
  vector<pair<int, vector<int>>> GetItemsets();

private:
  vector<pair<int, vector<int>>> iset_list;
};

#endif

