#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <istream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

using std::string;
using std::vector;

/*
    Fichier incluant la déclaration de la structure permettant de stocker les
    informations du problème
*/

class Problem {
  // *************************************************************************
  // JOB SHOP BASE	********************************************************
  // *************************************************************************
public:
  Problem(int nJob, int nMac, int lowerBound, vector<std::pair<int, int>> operationSpecs);

  string ToString() const;

  void Clear();

  unsigned size; // taille du problème
  unsigned nJob; // nombre de jobs
  unsigned nMac; // nombre de machines
  unsigned lowerBound; // borne inf du problème

  // caractéristiques des jobs
  unsigned minTime;
  unsigned maxTime;

  vector<vector<unsigned>> operationNumber; // numéro du sommet traité (job, op)
  vector<int> prevOperation; // numéro de l'op. suivante dans le job
  vector<int> nextOperation; // numéro de l'op. préc. dans le job
  vector<unsigned> machineNumber; // numéro de machine qui exec. l'op
  vector<int> timeOnMachine; // durée à passer sur la machine
  vector<unsigned> opToJob; // job associé à un numéro d'opération
  vector<unsigned> opToRank; // rang d'une opération dans un job
};

std::ostream& operator<<(std::ostream&, const Problem&);

// méthode pour extraire les données d'un fichier .txt
Problem LoadProblemFromPath(const string& file_path);
Problem LoadProblemFromStream(std::istream& file);

#endif
