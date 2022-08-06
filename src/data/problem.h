#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <istream>
#include <ostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

using std::string;
using std::vector;

/*
    Fichier incluant la déclaration de la structure permettant de stocker les
    informations du problème
*/

using OperationId = unsigned;
using JobId = unsigned;
using MachineId = unsigned;
using OperationRank = unsigned;

class Problem {
  // *************************************************************************
  // JOB SHOP BASE	********************************************************
  // *************************************************************************
public:
  Problem(JobId nJob, MachineId nMac, int lowerBound, vector<std::pair<unsigned, int>> operationSpecs);

  string ToString() const;

  void Clear();

  static constexpr OperationId InvalidOp = std::numeric_limits<OperationId>::max();
  JobId nJob; // nombre de jobs
  MachineId nMac; // nombre de machines
  OperationId size; // taille du problème
  int lowerBound; // borne inf du problème

  // caractéristiques des jobs
  int minTime;
  int maxTime;

  vector<vector<OperationId>> operationNumber; // numéro du sommet traité (job, op)

  vector<OperationId> prevOperation; // numéro de l'op. suivante dans le job
  vector<OperationId> nextOperation; // numéro de l'op. préc. dans le job
  vector<MachineId> machineNumber; // numéro de machine qui exec. l'op
  vector<int> timeOnMachine; // durée à passer sur la machine
};

std::ostream& operator<<(std::ostream&, const Problem&);

// méthode pour extraire les données d'un fichier .txt
Problem LoadProblemFromPath(const string& file_path);
Problem LoadProblemFromStream(std::istream& file);

#endif
