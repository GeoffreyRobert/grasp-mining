#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <numeric>

#include "problem.h"

/*
    Fichier incluant la méthode de stockage des informations du problème du JOB SHOP dans la structure Data
    Les données sont extraites des fichiers du Job Shop Financier
                    - à modifier pour rendre plus generique
*/

Problem::Problem(unsigned nJob, unsigned nMac, int lowerBound, std::vector<std::pair<unsigned, int>> operationsSpecs)
  : size(nJob * nMac)
  , nJob(nJob)
  , nMac(nMac)
  , lowerBound(lowerBound)
  , minTime(std::numeric_limits<int>::max())
  , maxTime(0)
{
  // Initialisation des structures de données
  operationNumber.resize(nJob);
  for (auto& v : operationNumber)
    v.resize(nMac);
  prevOperation.resize(size);
  nextOperation.resize(size);
  machineNumber.resize(size);
  timeOnMachine.resize(size);
  opToJob.resize(size);
  opToRank.resize(size);

  // Initialisation des antécédents / successeurs
  std::iota(prevOperation.begin(), prevOperation.end(), -1);
  std::iota(nextOperation.begin(), nextOperation.end(), 1);

  // Lecture des gammes+durées
  unsigned id = 0; // id unique pour chaque opération
  for (unsigned jid = 0; jid < nJob; jid++) {
    prevOperation[id] = -1; // 1ere op. du job sans antécédent
    for (unsigned oid = 0; oid < nMac; oid++, id++) {
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
    nextOperation[id - 1] = -1; // dernière op. sans successeur
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
  // Taille du problème
  unsigned nJob, nMac;
  int lowerBound;
  input >> nJob;
  input >> nMac;
  input >> lowerBound;
  unsigned size = nJob * nMac;

  // Lecture des gammes+durées
  auto operationsSpecs = std::vector<std::pair<unsigned ,int>>(size);
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

  int mac_digits = static_cast<int>(std::to_string(nMac - 1).length());
  int dur_digits = static_cast<int>(std::to_string(maxTime).length());

  unsigned id = 0;
  for (unsigned jid = 0; jid < nJob; jid++) {
    for (unsigned oid = 0; oid < nMac; oid++, id++) {
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
