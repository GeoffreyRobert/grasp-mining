#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <vector>
#include <string>

using std::vector; using std::string;

/*
	Fichier incluant la déclaration de la structure permettant de stocker les informations du problème
*/


class Problem {
	// *********************************************************************************
	// JOB SHOP BASE	****************************************************************
	// *********************************************************************************
public:
	void LoadProblemFromFile(const string& file_path, const string& file_name);	// méthode pour extraire les données d'un fichier .txt

	void Clear();

	void Init();

	int size;								// taille du problème
	int nJob;								// nombre de jobs
	int nMac;								// nombre de machines
	int lowerBound;							// borne inf du problème

    vector<vector<int>> operationNumber;	// numéro du sommet traité (job, op)
	vector<int> prevOperation;				// numéro de l'op. suivante dans le job
	vector<int> nextOperation;				// numéro de l'op. précédente dans le job
	vector<int> machineNumber;				// numéro de la machine devant traiter la pièce
	vector<int> timeOnMachine;				// durée à passer sur la machine
	vector<int> opToJob;					// job associé à un numéro d'opération
	vector<int> opToRank;					// rang d'une opération dans un job

};


#endif