#include "problem.h"

/*
	Fichier incluant la m�thode de stockage des informations du probl�me du JOB SHOP dans la structure Data
	Les donn�es sont extraites des fichiers du Job Shop Financier 
			- � modifier pour rendre plus generique
	
*/


void Problem::LoadProblemFromFile(const string& file_path, const string& file_name) {

	// Nettoyage des structures de donn�es
	Clear();

	ifstream fichier(file_path+file_name, ios::in);

	// Taille du probl�me
	fichier >> nJob;
	fichier >> nMac;
	fichier >> lowerBound;
	size = nJob*nMac;

	// Initialisation des structures de donn�es
	Init();

	// Lecture des gammes+dur�es
	for (int jid=0; jid<nJob; jid++) {
		prevOperation[jid*nMac] = -1;
		for (int oid=0; oid<nMac; oid++) {
			operationNumber[jid][oid]				= jid * nMac + oid;
			nextOperation[jid * nMac + oid]	= jid * nMac + oid + 1;
			jobForOp[operationNumber[jid][oid]]		= jid;
			rankForOp[operationNumber[jid][oid]]	= oid;
			fichier >> machineNumber[operationNumber[jid][oid]];
			fichier >> timeOnMachine[operationNumber[jid][oid]];
		}
		nextOperation[jid * nMac + nMac - 1]= INFINITE_C;
		for (int oid=1; oid<nMac; oid++) {
			prevOperation[jid*nMac+ oid] = jid*nMac + oid- 1;
		}
	}
	
	fichier.close(); // pas besoin de lire la suite
}


void Problem::Init() {
	operationNumber.resize(nJob);
	for (vector<int> v: operationNumber) v.reserve(nMac);
	prevOperation.reserve(size);
	nextOperation.reserve(size);
	machineNumber.reserve(size);
	timeOnMachine.reserve(size);
	jobForOp.reserve(size);
	rankForOp.reserve(size);
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