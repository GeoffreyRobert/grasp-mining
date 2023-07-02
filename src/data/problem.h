#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <istream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "data/typedefs.h"

using std::string;
using std::vector;

/*
    Fichier incluant la d�claration de la structure permettant de stocker les
    informations du probl�me
*/

class Problem {
  // *************************************************************************
  // JOB SHOP BASE	********************************************************
  // *************************************************************************
public:
  Problem(
      JobId nJob
    , MachineId nMac
    , int lowerBound
    , vector<std::pair<unsigned, int>> operationSpecs);

  const vector<OperationId>& OperationsOnMachine(MachineId machine) const;

  string ToString() const;

  void Clear();

  const JobId nJob; // nombre de jobs
  const MachineId nMac; // nombre de machines
  const OperationId opNum; // total number of schedulable operations excluding origin/final
  const OperationId size; // total number of operations including origin/final

  // One "origin" op and one "termination" op
  const OperationId OriginOp = 0;
  const OperationId FinalOp;

  // caract�ristiques des jobs
  const int lowerBound; // borne inf du probl�me
  int minTime;
  int maxTime;

  vector<vector<OperationId>> operationNumber; // numero du sommet trait� (job, op)
  vector<vector<OperationId>> operationsOnMachine; // for each machine, for each job, operation on the machine

  vector<OperationId> prevOperation; // id of the next operation in the job
  vector<OperationId> nextOperation; // num�ro de l'op. pr�c. dans le job
  vector<MachineId> machineNumber; // num�ro de machine qui exec. l'op
  vector<int> timeOnMachine; // dur�e � passer sur la machine
};

std::ostream& operator<<(std::ostream&, const Problem&);

// m�thode pour extraire les donn�es d'un fichier .txt
Problem LoadProblemFromPath(const string& file_path);
Problem LoadProblemFromStream(std::istream& file);

#endif
