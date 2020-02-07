#include <fstream>
#include <limits>

#include "problem.h"

/*
	Fichier incluant la méthode de stockage des informations du problème du JOB SHOP dans la structure Data
	Les données sont extraites des fichiers du Job Shop Financier 
			- à modifier pour rendre plus generique
	
*/

void Problem::LoadProblemFromFile(const string& file_path) {

	// Nettoyage des structures de données
	Clear();

	std::ifstream file(file_path, std::ios::in);

	// Taille du problème
	file >> nJob;
	file >> nMac;
	file >> lowerBound;
	size = nJob*nMac;

	// Initialisation des structures de données
	Init();

	// Lecture des gammes+durées
	for (int jid=0; jid<nJob; jid++) {
		prevOperation[jid*nMac] = -1;
		for (int oid=0; oid<nMac; oid++) {
			operationNumber[jid][oid] = jid * nMac + oid;
			nextOperation[jid * nMac + oid]	= jid * nMac + oid + 1;
			opToJob[operationNumber[jid][oid]]	= jid;
			opToRank[operationNumber[jid][oid]] = oid;
			file >> machineNumber[operationNumber[jid][oid]];
			file >> timeOnMachine[operationNumber[jid][oid]];
		}
		nextOperation[jid * nMac + nMac - 1] = -1;
		for (int oid=1; oid<nMac; oid++) {
			prevOperation[jid*nMac + oid] = jid*nMac + oid - 1;
		}
	}
	
	file.close(); // pas besoin de lire la suite
}


void Problem::Init() {
	operationNumber.resize(nJob);
	for (vector<int>& v: operationNumber) v.resize(nMac);
	prevOperation.resize(size);
	nextOperation.resize(size);
	machineNumber.resize(size);
	timeOnMachine.resize(size);
	opToJob.resize(size);
	opToRank.resize(size);
}


void Problem::Clear() {
	operationNumber.clear();
	prevOperation.clear();
	nextOperation.clear();
	machineNumber.clear();
	timeOnMachine.clear();
	opToJob.clear();
	opToRank.clear();
}