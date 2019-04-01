#include "Out_JS.h"

void OutJS::StockResult(Problem& problem, Solution& solution, int instance, int replic) {
	
	tt_Instance_X_Replication[instance][replic] = ((double) solution.totalTime - b.init)/CLOCKS_PER_SEC;
	ttb_Instance_X_Replication[instance][replic]=((double) solution.timeToBest - b.init)/CLOCKS_PER_SEC;
	s_Instance_X_Replication[instance][replic]=b.makespan;
	deviance_Instance_X_Replication[instance][replic]= (((float) b.makespan-problem.lowerBound)/problem.lowerBound)*100;

	avg_S_Instance[instance]+=b.makespan;		
	avg_TTB_Instance[instance]+=ttb_Instance_X_Replication[instance][replic];
	avg_TT_Instance[instance]+=tt_Instance_X_Replication[instance][replic];
	if (b.makespan == problem.lowerBound) ++nb_Opt[instance]; 
}

void OutJS::OutputResults(int start, int end) {
	double avg_S = 0, avg_TTB=0, avg_TT=0, avg_DEV = 0, avg_LB=0, avg_BestValue = 0, avg_TTBValue=0, avg_DEVBestValue=0;
	int bestValue;
	double bestTime;
	
	string s1, s2, s3, s4, s5, s6, s7, s8;
	ofstream resultsForGantt("..\\Results\\JSFF_Results.csv", ios::out | ios::app); 
	ofstream resultsLightForGantt("..\\Results\\JSFF_Results_light.csv", ios::out | ios::app);

	int maxInst = end - start+1;
	resultsLightForGantt << "INSTANCE ;  LB ; S ; TT_S ; TTB_S ; Dev_S ; BFS ; TTB_BFS ; Dev_BFS ; Nb_OPT/"+to_string(MAX_REPLICATION) << "\n";	
	for (int inst = start; inst <= end; ++inst) {
		bestValue = INFINITE_C;
		bestTime = INFINITE_C;
		avg_LB+=lower_bound[inst];
		avg_S_Instance[inst]=avg_S_Instance[inst]/MAX_REPLICATION;
		avg_TT_Instance[inst]=avg_TT_Instance[inst]/MAX_REPLICATION;
		avg_TTB_Instance[inst]=avg_TTB_Instance[inst]/MAX_REPLICATION;
		avg_DEV_Instance[inst] = (((float) avg_S_Instance[inst]-lower_bound[inst])/lower_bound[inst])*100;

		resultsForGantt << name_Instance[inst] << " ; " << " LB = " << lower_bound[inst] << "\n";
		resultsForGantt << "Replication ;  S ; TT_S ; TTB_S ; Dev_S  " << "\n";
		for (int i = 0; i<MAX_REPLICATION; ++i) { 
			s1=to_string(tt_Instance_X_Replication[inst][i]);		s1=s1.replace(s1.find("."),1,",");
			s2=to_string(ttb_Instance_X_Replication[inst][i]);		s2=s2.replace(s2.find("."),1,",");
			s3=to_string(deviance_Instance_X_Replication[inst][i]); s3=s3.replace(s3.find("."),1,",");
			
			resultsForGantt << i+1 << " ; " << s_Instance_X_Replication[inst][i] << " ; " <<s1 << " ; " << s2 <<" ; " << s3  << "\n";
			if (s_Instance_X_Replication[inst][i]<bestValue) {
				bestValue=s_Instance_X_Replication[inst][i];
				bestTime=ttb_Instance_X_Replication[inst][i];
			}
		}

		avg_S+=avg_S_Instance[inst];
		avg_TT+=avg_TT_Instance[inst];
		avg_TTB+=avg_TTB_Instance[inst];
		avg_DEV+=avg_DEV_Instance[inst];
		avg_BestValue+=bestValue;
		avg_TTBValue+=bestTime;

		s1 = to_string(avg_S_Instance[inst]);		s1=s1.replace(s1.find("."),1,",");
		s2 = to_string(avg_TT_Instance[inst]);		s2=s2.replace(s2.find("."),1,",");
		s3 = to_string(avg_TTB_Instance[inst]);		s3=s3.replace(s3.find("."),1,",");
		s4 = to_string(avg_DEV_Instance[inst]);		s4=s4.replace(s4.find("."),1,",");
		s5 = to_string(bestValue);						
		s6 = to_string(bestTime);						s6=s6.replace(s6.find("."),1,",");
		s7 = to_string((((float) bestValue-lower_bound[inst])/lower_bound[inst])*100);	s7=s7.replace(s7.find("."),1,",");

		avg_DEVBestValue+=(((float) bestValue-lower_bound[inst])/lower_bound[inst])*100;
		resultsForGantt<< "Average : \n";
		resultsForGantt << " - " << " ; " << s1 << " ; " << s2 << " ; " << s3 <<" ; " << s4 << " ; " << "\n\n\n";
		
		resultsLightForGantt << name_Instance[inst]<< " ; " << lower_bound[inst]<< " ; " << s1 << " ; " << s2 << " ; " << s3 <<" ; " << s4 << " ; " << s5 << " ; " << s6 << " ; "  << s7 << " ; "  <<nb_Opt[inst]<< "\n";

	}

	s1 = to_string(avg_S / maxInst);			s1 = s1.replace(s1.find("."), 1, ",");
	s2 = to_string(avg_TT / maxInst);			s2 = s2.replace(s2.find("."), 1, ",");
	s3 = to_string(avg_TTB / maxInst);			s3 = s3.replace(s3.find("."), 1, ",");
	s4 = to_string(avg_DEV / maxInst);			s4 = s4.replace(s4.find("."), 1, ",");
	s5 = to_string(avg_BestValue / maxInst);	s5 = s5.replace(s5.find("."), 1, ",");
	s6 = to_string(avg_TTBValue / maxInst);		s6 = s6.replace(s6.find("."), 1, ",");
	s7 = to_string(avg_DEVBestValue / maxInst);	s7 = s7.replace(s7.find("."), 1, ",");
	s8 = to_string(avg_LB / maxInst);			s8 = s8.replace(s8.find("."), 1, ",");
		
	resultsLightForGantt<< "Average : \n";
	resultsLightForGantt << " - " << " ; " << s8 << " ; " << s1 << " ; " << s2 <<" ; " <<  s3 << " ; " << s4 << " ; " << s5  <<" ; " <<  s6 << " ; " << s7<< " ; " <<  "\n";
	
	resultsForGantt.close();
	resultsLightForGantt.close();
}


void OutJS::writeGANTT_SVG(Problem& problem, Solution& solution, string& prob_name) {
	int i, job, mac;
	int nbmac = problem.nMac, nbjob = problem.nJob;
	int margin = 60;
	int marginBetweenRect = 2;
	int fleche = 20;
	int rectWidth = 50;
	int xStart = 0, xLength = 0, yStart = 0, yLength = 0, stringLen = 0;

	ofstream dataForGantt("..\\Results\\" + prob_name + "_gantt.svg", ios::out | ios::trunc);


	//dataForGantt << data.pName + "\n");  // écrire une ligne dans le fichier resultat.txt


	/*----------------------------------------------------------------------
	Ratio pour taille des rectangles du diagramme
	----------------------------------------------------------------------*/
	int ratio = 2;
	/*if (solution.makespan <= 30) {
	ratio = 20;
	} else if (solution.makespan < 100) {
	ratio = 10;
	} else if (solution.makespan < 250) {
	ratio = 5;
	} else if (solution.makespan < 500) {
	ratio = 2;
	} */
	int maxDiagLength = (int)((ratio)*solution.makespan + 2 * margin + fleche + problem.nMac);
	int maxDiagHeight = (rectWidth + marginBetweenRect)*problem.nMac + margin + fleche + 10;

	dataForGantt << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
	//dataForGantt <<  "<svg width=\"" + maxDiagLength + "\" height=\"" + maxDiagHeight + "\">");
	dataForGantt << " <svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"" << maxDiagLength << "\" height=\"" << maxDiagHeight << "\">\n";
	dataForGantt << "<title> Diagramme de Gantt du Probleme: " << prob_name << "</title>\n";
	dataForGantt << "<desc> Cette figure represente le planning des operations du probleme traite.</desc>\n";


	/*----------------------------------------------------------------------
	Création de la palette de couleurs
	----------------------------------------------------------------------*/
	int colors[MAX_JOBS][3];
	init_genrand64(0);
	for (i = 0; i < nbjob; ++i) {
		colors[i][0] = genrand64_int64() % 256;
		colors[i][1] = genrand64_int64() % 256;
		colors[i][2] = genrand64_int64() % 256;
	}

	for (i = 0; i < problem.size; i++) {

		job = problem.jobForOp[i];
		mac = problem.machineNumber[i];

		xStart = margin + (ratio)*(solution.endDate[i] - problem.timeOnMachine[i]) + marginBetweenRect; //+dd1*macVisited[mac]
		xLength = (ratio)*(problem.timeOnMachine[i]) - marginBetweenRect; //+dd1
		yStart = margin + mac*rectWidth + marginBetweenRect;
		yLength = rectWidth - marginBetweenRect;

		dataForGantt << "<rect style=\"fill:rgb(" << colors[job][0] << "," << colors[job][1] << "," << colors[job][2] << ");fill-opacity:0.5;\" width=\"" << xLength << "\" height=\"" << yLength << "\" x=\"" << xStart << "\" y=\"" << yStart << "\"/>\n";


		dataForGantt << "<text x=\"" << ((xLength + 2 * xStart) / 2) << "\" y=\"" << (yStart + 15) << "\" font-family=\"sans-serif\" font-size=\"10px\" text-anchor=\"middle\">J" << job << "</text>";
		dataForGantt << "<text x=\"" << ((xLength + 2 * xStart) / 2) << "\" y=\"" << (yStart + 25) << "\" font-family=\"sans-serif\" font-size=\"10px\" text-anchor=\"middle\">" << (solution.endDate[i] - problem.timeOnMachine[i]) << "</text>";
		dataForGantt << "<text x=\"" << ((xLength + 2 * xStart) / 2) << "\" y=\"" << (yStart + 35) << "\" font-family=\"sans-serif\" font-size=\"10px\" text-anchor=\"middle\">" << solution.endDate[i] << "</text>\n";



		// dataForGantt <<  "<rect style=\"fill:rgb(" + colors[job][0] + "," + colors[job][1] + "," + colors[job][2] + ");fill-opacity:0." + opac + ";stroke:#000000;stroke-opacity:1;opacity:1\" id=\"rect"+i+"\" width=\"" + yStart + "\" height=\"200\" x=\"" + xStart + "\" y=\"" + xLength + "\"/>\n");

	}


	//buffer.setColor(Color.DARK_GRAY);
	// vertical
	dataForGantt << "<line x1=\"" << margin << "\" y1=\"" << (margin - fleche) << "\" x2=\"" << margin << "\" y2=\"" << (margin + nbmac*rectWidth + marginBetweenRect) << "\" stroke=\"dimgrey\" />\n";
	dataForGantt << "<line x1=\"" << margin << "\" y1=\"" << (margin - fleche) << "\" x2=\"" << (margin + 10) << "\" y2=\"" << (margin - fleche + 10) << "\" stroke=\"dimgrey\" />\n";

	for (i = 1; i <= nbmac; ++i) {
		dataForGantt << "<text x=\"" << (margin - 25) << "\" y=\"" << (margin - fleche + i*rectWidth) << "\" font-family=\"sans-serif\" font-size=\"10px\" text-anchor=\"middle\">M." << i << "</text>\n";

	}


	//horizontal
	dataForGantt << "<line x1=\"" << margin << "\" y1=\"" << (margin + nbmac*rectWidth + marginBetweenRect) << "\" x2=\"" << (margin + ratio*solution.makespan + fleche) << "\" y2=\"" << (margin + nbmac*rectWidth + marginBetweenRect) << "\" stroke=\"dimgrey\" />\n";
	dataForGantt << "<line x1=\"" << (margin + ratio*solution.makespan + fleche - 10) << "\" y1=\"" << (margin + nbmac*rectWidth + marginBetweenRect - 10) << "\" x2=\"" << (margin + ratio*solution.makespan + fleche) << "\" y2=\"" << (margin + nbmac*rectWidth + marginBetweenRect) << "\" stroke=\"dimgrey\" />\n";
	dataForGantt << "<line x1=\"" << (margin + ratio*solution.makespan) << "\" y1=\"" << (margin + nbmac*rectWidth + marginBetweenRect + 2) << "\" x2=\"" << (margin + ratio*solution.makespan) << "\" y2=\"" << (margin + nbmac*rectWidth + marginBetweenRect + 10) << "\" stroke=\"dimgrey\" />\n";

	dataForGantt << "<text x=\"" << (margin + 10) << "\" y=\"" << (margin - fleche - 5) << "\" font-family=\"sans-serif\" font-size=\"20px\" text-anchor=\"middle\" fill=\"dimgrey\">Machines</text>\n";
	dataForGantt << "<text x=\"" << (margin + ratio*solution.makespan + 2 * fleche) << "\" y=\"" << (margin + nbmac*rectWidth + marginBetweenRect) << "\" font-family=\"sans-serif\" font-size=\"20px\" text-anchor=\"middle\" fill=\"dimgrey\">Time</text>\n";

	dataForGantt << "<text x=\"" << (maxDiagLength / 2) << "\" y=\"" << (margin + nbmac*rectWidth + marginBetweenRect + 30) << "\" font-family=\"sans-serif\" font-size=\"30px\" text-anchor=\"middle\" fill=\"dimgrey\">Diagramme de Gantt du problème: " << prob_name << " - makespan: " << solution.makespan << "</text>\n";


	//Fin du fichier
	dataForGantt << "</svg>\n";
	dataForGantt.close();// fermer le fichier à la fin des traitements

}
