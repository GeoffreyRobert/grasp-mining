#ifndef _OUT_JS_
#define _OUT_JS_

#include "Global.h"
#include "data/problem.h"
#include "data/bierwirth_sequence.h"

/*
	Fichier incluant la déclaration de la structure permettant de stocker les résultats obtenus au cours des algorithmes
	But : proposer fichier de sortie
*/

struct OutJS {

	string name_Instance[MAX_INSTANCES+1];										// nom de l'instance
	int lower_bound[MAX_INSTANCES+1];											// nom de l'instance
	double tt_Instance_X_Replication[MAX_INSTANCES+1][MAX_REPLICATION] ;			// temps total d'execution pour la replication
	double ttb_Instance_X_Replication[MAX_INSTANCES+1][MAX_REPLICATION] ;		// temps d'execution pour arriver à la meilleure solution pour la replication
	int s_Instance_X_Replication[MAX_INSTANCES+1][MAX_REPLICATION] ;			// solution found
	int nb_Opt[MAX_INSTANCES+1];												// solution found
	double deviance_Instance_X_Replication[MAX_INSTANCES+1][MAX_REPLICATION] ;	// deviance
	
	double avg_TTB_Instance[MAX_INSTANCES + 1];								// temps d'execution moyen pour arriver à la meilleure solution pour la replication
	double avg_TT_Instance[MAX_INSTANCES + 1];								// temps d'execution moyen pour arriver à la meilleure solution pour la replication
	double avg_S_Instance[MAX_INSTANCES + 1];									// temps d'execution moyen pour arriver à la meilleure solution pour la replication
	double avg_DEV_Instance[MAX_INSTANCES + 1];								// temps d'execution moyen pour arriver à la meilleure solution pour la replication


	void stock_Res(Problem& data, BierwirthSequence& b, int instance, int replic); // méthode pour stocker résultat
	void outputResults(int start, int end);			// résultats en sortie					


	// fichier de sortie pour obtenir un diagramme de GANTT - solution perso (code java)
	void writeGANTT_SVG(Data& data, BierwirthSequence& seq, string pName);
};

#endif	// _OUT_JS_