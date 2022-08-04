#include <fstream>
#include <limits>
#include <numeric>

#include "problem.h"

/*
    Fichier incluant la méthode de stockage des informations du problème du JOB SHOP dans la structure Data
    Les données sont extraites des fichiers du Job Shop Financier
                    - à modifier pour rendre plus generique
*/

Problem::Problem(int nJob, int nMac, int lowerBound, std::vector<std::pair<int , int>> operationsSpecs)
  : size(nJob * nMac)
  , nJob(nJob)
  , nMac(nMac)
  , lowerBound(lowerBound)
  , minTime(std::numeric_limits<int>::max())
  , maxTime(0)
{
  // Initialisation des structures de données
  operationNumber.resize(nJob);
  for (vector<int>& v : operationNumber)
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
  int id = 0; // id unique pour chaque opération
  for (int jid = 0; jid < nJob; jid++) {
    prevOperation[id] = -1; // 1ere op. du job sans antécédent
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
  int nJob, nMac, lowerBound;
  input >> nJob;
  input >> nMac;
  input >> lowerBound;
  int size = nJob * nMac;

  // Lecture des gammes+durées
  auto operationsSpecs = std::vector<std::pair<int ,int>>();
  operationsSpecs.resize(size);
  for (auto operationSpecs : operationsSpecs) {
    int machineNumber, timeOnMachine;
    input >> machineNumber;
    input >> timeOnMachine;
  }

  return Problem(nJob, nMac, lowerBound, operationsSpecs);
}

string Problem::ToString() const
{
  string res;

  res += std::to_string(nJob) + ' ';
  res += std::to_string(nMac) + ' ';
  res += std::to_string(lowerBound) + '\n';

  int tmp = nMac;
  unsigned mac_digits = 0;
  while (tmp != 0) {
    tmp /= 10;
    mac_digits++;
  }
  tmp = maxTime;
  unsigned dur_digits = 0;
  while (tmp != 0) {
    tmp /= 10;
    dur_digits++;
  }

  int id = 0;
  int space_pad;
  string tmp_number;
  for (int jid = 0; jid < nJob; jid++) {
    for (int oid = 0; oid < nMac; oid++, id++) {
      tmp_number = std::to_string(machineNumber[id]);
      space_pad = 4 + mac_digits - tmp_number.length();
      res += string(space_pad, ' ') + tmp_number;
      tmp_number = std::to_string(timeOnMachine[id]);
      space_pad = 1 + dur_digits - tmp_number.length();
      res += string(space_pad, ' ') + tmp_number;
    }
    res += '\n';
  }

  return res;
}

std::ostream& operator<<(std::ostream& output, const Problem& problem)
{
  output << problem.ToString();
  return output;
}
