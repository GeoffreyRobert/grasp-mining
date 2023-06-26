/*----------------------------------------------------------------------
  File     : data.h
  Contents : data set management
----------------------------------------------------------------------*/
#ifndef _DATA_CLASS
#define _DATA_CLASS

#include <cstdio>
#include <cstdlib>
#include <vector>

#define TransLen 50

class Transaction
{
public:
	Transaction() {};
	Transaction(std::vector<int>&&);
  
  std::vector<int> item_vec;
	int *t;
	int length;
};

class Data
{
public:
  virtual ~Data() {};
	virtual const Transaction* getNextTransaction() = 0;
};

class FileData : public Data
{
 public:
	
	FileData(char *filename);
	~FileData() override;
	int isOpen();

	const Transaction* getNextTransaction() override;
  
 private:
	Transaction Trans;
	FILE *in;
};

class VectorData : public Data
{
public:
  VectorData(std::vector<Transaction>&&);
	const Transaction* getNextTransaction() override;

private:
  std::vector<Transaction> _transactions;
  std::vector<Transaction>::iterator _iter;
};

#endif

