#include <fstream>
#include <limits>
#include <numeric>

#include "problem.h"

/*
	Fichier incluant la méthode de stockage des informations du problème du JOB SHOP dans la structure Data
	Les données sont extraites des fichiers du Job Shop Financier 
			- à modifier pour rendre plus generique
	
*/

Problem::Problem() :
    size(0), nJob(0), nMac(0), lowerBound(std::numeric_limits<int>::max()),
    minTime(std::numeric_limits<int>::max()), maxTime(0)
{
}

Problem::Problem(const string& file_path) {
    LoadProblemFromPath(file_path);
}

void Problem::LoadProblemFromPath(const string& file_path) {

    std::ifstream file(file_path, std::ios::in);
    LoadProblemFromStream(file);

    file.close(); // pas besoin de lire la suite
}

void Problem::LoadProblemFromStream(std::istream& input) {

	// Nettoyage des structures de données
	Clear();

	// Taille du problème
	input >> nJob;
    input >> nMac;
    input >> lowerBound;
	size = nJob * nMac;

	// Initialisation des structures de données
	Init();

    // Initialisation des antécédents / successeurs
    std::iota(prevOperation.begin(), prevOperation.end(), -1);
    std::iota(nextOperation.begin(), nextOperation.end(), 1);

	// Lecture des gammes+durées
    int id = 0;     // id unique pour chaque opération
	for (int jid=0; jid<nJob; jid++) {
        prevOperation[id] = -1;             // 1ere op. du job sans antécédent
		for (int oid=0; oid<nMac; oid++, id++) {
			operationNumber[jid][oid] = id;
			opToJob[id]	= jid;
			opToRank[id] = oid;
            input >> machineNumber[id];
            input >> timeOnMachine[id];
            if (timeOnMachine[id] > maxTime) maxTime = timeOnMachine[id];
            if (timeOnMachine[id] < minTime) minTime = timeOnMachine[id];
		}
		nextOperation[id - 1] = -1;         // dernière op. sans successeur
	}
}


string Problem::ToString() const {
    string res;

    res += std::to_string(nJob) + ' ';
    res += std::to_string(nMac) + ' ';
    res += std::to_string(lowerBound) + '\n';

    int tmp = nMac;
    unsigned mac_digits = 0;
    while (tmp != 0) { tmp /= 10; mac_digits++; }
    tmp = maxTime;
    unsigned dur_digits = 0;
    while (tmp != 0) { tmp /= 10; dur_digits++; }

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


std::ostream& operator<<(std::ostream& output, const Problem& problem) {
    output << problem.ToString();
    return output;
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