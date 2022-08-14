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

Problem::Problem(JobId nJob_, MachineId nMac_, int lowerBound_, std::vector<std::pair<MachineId, int>> operationsSpecs)
  : nJob(nJob_)
  , nMac(nMac_)
  , opNum(nJob * nMac)
  , size(opNum + 2)
  , FinalOp(opNum + 1)
  , lowerBound(lowerBound_)
  , minTime(std::numeric_limits<int>::max())
  , maxTime(0)
{
  // Initialisation des structures de données
  operationNumber.resize(nJob);
  for (auto& v : operationNumber)
    v.resize(nMac);

  operationsOnMachine.resize(nMac);
  for (auto& v : operationsOnMachine)
    v.resize(nJob);

  prevOperation.resize(size);
  nextOperation.resize(size);
  machineNumber.resize(size);
  timeOnMachine.resize(size);

  // Initialisation des antécédents / successeurs
  // ignore the origin/final ops
  prevOperation[OriginOp] = OriginOp;
  std::iota(++prevOperation.begin(), prevOperation.end(), 0);
  std::iota(nextOperation.begin(), --nextOperation.end(), 1);
  nextOperation[FinalOp] = FinalOp;

  // Lecture des gammes+durées
  OperationId oid = 1; // id unique pour chaque opération, indexées a 1
  for (auto& operationSpecs : operationsSpecs) {
    machineNumber[oid] = operationSpecs.first;
    timeOnMachine[oid] = operationSpecs.second;
    ++oid;
  }

  oid = 1;
  for (JobId jid = 0; jid < nJob; jid++) {
    prevOperation[oid] = OriginOp; // 1ere op. du job sans antécédent
    nextOperation[oid + nMac - 1] = FinalOp; // dernière op. sans successeur
    for (OperationRank onum = 0; onum < nMac; onum++, oid++) {
      operationNumber[jid][onum] = oid;
      MachineId mid = machineNumber[oid];
      operationsOnMachine[mid][jid] = oid;
      if (timeOnMachine[oid] > maxTime)
        maxTime = timeOnMachine[oid];
      if (timeOnMachine[oid] < minTime)
        minTime = timeOnMachine[oid];
    }
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

  OperationId oid = 1;
  for (JobId jid = 0; jid < nJob; jid++) {
    for (MachineId mid = 0; mid < nMac; mid++, oid++) {
      res
        << ' '
        << std::setw(mac_digits) << machineNumber[oid] << ' '
        << std::setw(dur_digits) << timeOnMachine[oid];
      if (mid != nMac - 1) {
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
