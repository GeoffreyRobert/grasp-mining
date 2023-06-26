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
	
	Transaction():maxlength(TransLen), length(0){ t = new int[maxlength];}
	void DoubleTrans(int);				// if current item is greater than current longest transaction, we change the length of t as 2*item
	~Transaction(){delete []t;}
  
	int maxlength;
	int length;
	int *t;
};

class Data
{
public:
  virtual ~Data() {};
	virtual Transaction *getNextTransaction(Transaction* Trans) = 0;
};

class FileData : public Data
{
 public:
	
	FileData(char *filename);
	~FileData() override;
	int isOpen();
	void close(){if(in)fclose(in);}

	Transaction *getNextTransaction(Transaction* Trans) override;
  
 private:
  
	FILE *in;
};

class VectorData : public Data
{
public:
  VectorData(std::vector<Transaction>&&);
	Transaction *getNextTransaction(Transaction* Trans) override;

private:
  std::vector<Transaction> _transactions;
  std::vector<Transaction>::iterator _iter;
};

#endif

