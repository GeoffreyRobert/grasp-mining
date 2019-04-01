#ifndef OUT_JS_H_
#define OUT_JS_H_

#include "data/problem.h"
#include "data/bierwirth_sequence.h"

/*
	Fichier incluant la d�claration de la structure permettant de stocker les r�sultats obtenus au cours des algorithmes
	But : proposer fichier de sortie
*/

struct OutJS {

	vector<string> name_Instance;					// nom de l'instance
	vector<int> lower_bound;						// solution connue de l'instance
	vector<double> tt_Instance_X_Replication;		// temps total d'execution par replication
	vector<double> ttb_Instance_X_Replication;		// temps pour arriver � la meilleure sol. par replication
	vector<int> s_Instance_X_Replication;			// score trouve par instance pour chaque replication
	vector<int> nb_Opt;								// solution found
	vector<double> deviance_Instance_X_Replication;	// deviance par rapport � l'optimum (?)

	vector<double> avg_TT_Instance;					// temps d'execution moyen par instance
	vector<double> avg_TTB_Instance;				// temps moyen pour arriver � la meilleure sol. par instance
	vector<double> avg_S_Instance;					// score moyen trouv� par instance
	vector<double> avg_DEV_Instance;				// �cart type du score par instance


	void StockResult(Problem&, Solution&, int instance, int replic); // m�thode pour stocker r�sultat
	void OutputResults(int start, int end);			// r�sultats en sortie					


	// fichier de sortie pour obtenir un diagramme de GANTT - solution perso (code java)
	void writeGANTT_SVG(Problem& problem, Solution& solution, string& prob_name);
};

#endif	// OUT_JS_H_