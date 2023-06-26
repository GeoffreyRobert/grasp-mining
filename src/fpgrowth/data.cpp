/*----------------------------------
File     : data.cpp
Contents : data set management
----------------------------------*/

#include <utility>

#include "data.h"
#include "common.h"

Transaction::Transaction(std::vector<int>&& t_vec)
  : item_vec(std::move(t_vec))
  , t(item_vec.data())
  , length(item_vec.size())
{}

FileData::FileData(char *filename)
{
#ifndef BINARY
  in = fopen(filename,"rt");
#else
  in = fopen(filename, "rb");
#endif
}

FileData::~FileData()
{
  if(in) fclose(in);
}

int FileData::isOpen()
{
  if(in) return 1;
  else return 0;
}

const Transaction *FileData::getNextTransaction()
{
  Trans.item_vec.clear();

  // read list of items
#ifndef BINARY
  char c;
  int item, pos;
  do {
    item=0;
    pos=0;
    c = getc(in);
    while((c >= '0') && (c <= '9')) {
      item *=10;
      item += int(c)-int('0');
      c = getc(in);
      pos++;
    }
    if(pos)
    {
      Trans.item_vec.push_back(item);
    }
  }while(c != '\n' && !feof(in));
  // if end of file is reached, rewind to beginning for next pass
  if(feof(in)){
    rewind(in);
    return 0;
  }
  // Note, also last transaction must end with newline,
  // else, it will be ignored
#else
  int i, nitem, *buffer=new int;
  fread((char*)buffer, sizeof(int), 1, in);
  if(feof(in))
  {
    rewind(in);
    return 0;
  }
  fread((char*)buffer, sizeof(int), 1, in);
  fread((char*)buffer, sizeof(int), 1, in);
  nitem=*buffer;
  for(i=0; i<nitem; i++)
  {
    fread((char*)buffer, sizeof(int), 1, in);

    if(Trans->length >= Trans->maxlength)
      Trans->DoubleTrans(Trans->length);

    Trans->t[Trans->length] = *buffer;
    Trans->length++;
  }
#endif

  Trans.t = Trans.item_vec.data();
  Trans.length = Trans.item_vec.size();

  return &Trans;
}


VectorData::VectorData(std::vector<Transaction>&& transactions)
  : _transactions(std::move(transactions))
  , _iter(_transactions.begin())
{ }


const Transaction* VectorData::getNextTransaction()
{
  if (_iter == _transactions.end())
  {
    _iter = _transactions.begin();
    return nullptr;
  }
  Transaction* ret = &(*_iter);
  ++_iter;
  return ret;
}
