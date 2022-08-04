#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <numeric>

#include "problem.h"

/*
    Fichier incluant la m�thode de stockage des informations du probl�me du JOB SHOP dans la structure Data
    Les donn�es sont extraites des fichiers du Job Shop Financier
                    - � modifier pour rendre plus generique
*/

Problem::Problem(int nJob, int nMac, int lowerBound, std::vector<std::pair<int , int>> operationsSpecs)
  : size(nJob * nMac)
  , nJob(nJob)
  , nMac(nMac)
  , lowerBound(lowerBound)
  , minTime(std::numeric_limits<int>::max())
  , maxTime(0)
{
  // Initialisation des structures de donn�es
  operationNumber.resize(nJob);
  for (vector<int>& v : operationNumber)
    v.resize(nMac);
  prevOperation.resize(size);
  nextOperation.resize(size);
  machineNumber.resize(size);
  timeOnMachine.resize(size);
  opToJob.resize(size);
  opToRank.resize(size);

  // Initialisation des ant�c�dents / successeurs
  std::iota(prevOperation.begin(), prevOperation.end(), -1);
  std::iota(nextOperation.begin(), nextOperation.end(), 1);

  // Lecture des gammes+dur�es
  int id = 0; // id unique pour chaque op�ration
  for (int jid = 0; jid < nJob; jid++) {
    prevOperation[id] = -1; // 1ere op. du job sans ant�c�dent
    for (int oid = 0; oid < nMac; oid++, id++) {
      operationNumber[jid][oid] = id;
      opToJob[id] = jid;
      opToRank[id] = oid;
      auto operationSpecs = operationsSpecs[id];
      machineNumber[id] = operationSpecs.first;
      timeOnMachine[id] = operationSpecs.second;
      if (timeOnMachine[id] > maxTime)
        maxTime = timeOnMachine[id];
      if (timeOnMachine[id] < minTime)
        minTime = timeOnMachine[id];
    }
    nextOperation[id - 1] = -1; // derni�re op. sans successeur
  }
}

Problem LoadProblemFromPath(const string& file_path)
{
  std::ifstream file(file_path, std::ios::in);
  auto problem = LoadProblemFromStream(file);

  file.close(); // pas besoin de lire la suite
  return problem;
}

Problem LoadProblemFromStream(std::istream& input)
{
  // Taille du probl�me
  int nJob, nMac, lowerBound;
  input >> nJob;
  input >> nMac;
  input >> lowerBound;
  int size = nJob * nMac;

  // Lecture des gammes+dur�es
  auto operationsSpecs = std::vector<std::pair<int ,int>>(size);
  for (auto& operationSpecs : operationsSpecs) {
    input >> operationSpecs.first;
    input >> operationSpecs.second;
  }

  return Problem(nJob, nMac, lowerBound, operationsSpecs);
}

string Problem::ToString() const
{
  std::ostringstream res;

  res
    << ' ' << std::to_string(nJob)
    << ' ' << std::to_string(nMac)
    << ' ' << std::to_string(lowerBound) << '\n';

  unsigned mac_digits = std::to_string(nMac - 1).length();
  unsigned dur_digits = std::to_string(maxTime).length();

  int id = 0;
  for (int jid = 0; jid < nJob; jid++) {
    for (int oid = 0; oid < nMac; oid++, id++) {
      res
        << ' '
        << std::setw(mac_digits) << machineNumber[id] << ' '
        << std::setw(dur_digits) << timeOnMachine[id];
      if (oid != nMac-1) {
        res << ' ';
      } else {
        res << '\n';
      }
    }
  }

  return res.str();
}

std::ostream& operator<<(std::ostream& output, const Problem& problem)
{
  output << problem.ToString();
  return output;
}
