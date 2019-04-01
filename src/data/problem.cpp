#include <fstream>
#include <limits>

#include "problem.h"

/*
	Fichier incluant la m�thode de stockage des informations du probl�me du JOB SHOP dans la structure Data
	Les donn�es sont extraites des fichiers du Job Shop Financier 
			- � modifier pour rendre plus generique
	
*/

void Problem::LoadProblemFromFile(const string& file_path, const string& file_name) {

	// Nettoyage des structures de donn�es
	Clear();

	std::ifstream file(file_path+file_name, std::ios::in);

	// Taille du probl�me
	file >> nJob;
	file >> nMac;
	file >> lowerBound;
	size = nJob*nMac;

	// Initialisation des structures de donn�es
	Init();

	// Lecture des gammes+dur�es
	for (int jid=0; jid<nJob; jid++) {
		prevOperation[jid*nMac] = -1;
		for (int oid=0; oid<nMac; oid++) {
			operationNumber[jid][oid] = jid * nMac + oid;
			nextOperation[jid * nMac + oid]	= jid * nMac + oid + 1;
			jobForOp[operationNumber[jid][oid]]	= jid;
			rankForOp[operationNumber[jid][oid]] = oid;
			file >> machineNumber[operationNumber[jid][oid]];
			file >> timeOnMachine[operationNumber[jid][oid]];
		}
		nextOperation[jid * nMac + nMac - 1] = -1;
		for (int oid=1; oid<nMac; oid++) {
			prevOperation[jid*nMac+ oid] = jid*nMac + oid- 1;
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
	jobForOp.resize(size);
	rankForOp.resize(size);
}


void Problem::Clear() {
	operationNumber.clear();
	prevOperation.clear();
	nextOperation.clear();
	machineNumber.clear();
	timeOnMachine.clear();
	jobForOp.clear();
	rankForOp.clear();
}