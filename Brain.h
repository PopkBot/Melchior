#pragma once

#pragma once
//#include<thread>
//#include <stdio.h>    
//#include <stdlib.h> 

#include<iostream>
//#include <chrono>
//#include<time.h>

#include<math.h>
#include"nnN1.h"


#define BrainCellsNum 4
#define deltaMaxCount 20
#define deltaTreashold 1*pow(10,0)
#define deltaD 0.1


class Brain
{
public:


	nnN1 brain[BrainCellsNum];
	nnN1 brainD[BrainCellsNum];

	double wD[BrainCellsNum][nodeCount][nodeCount];				//[] [receiver node] [emmitor node]
	double wDBest[BrainCellsNum][nodeCount][nodeCount];
	double bD[BrainCellsNum][nodeCount];
	double bDBest[BrainCellsNum][nodeCount];

	long double score;
	long double scorePrev=-1;
	long double scoreD;
	long double scoreBest = -1;



	struct deltaBrain {
		
	};

	int deltaCount=0;

	bool deltaBool = false;
	bool deltaBest = false;


	void initialize_brain() {

		for (int i = 0; i < BrainCellsNum; i++) {
			brain[i].initialize();
			brainD[i].initialize();
		}



	}

	void think_brain(float inp[], float outp[]) {

		if(!deltaBool)
			for (int cellsCount = 0; cellsCount < BrainCellsNum; cellsCount++) {
				outp[cellsCount] = brain[cellsCount].thinK(inp);
				//printf("\n == %f \t ", outp[2]);
			}
		else if(deltaBool)
			for (int cellsCount = 0; cellsCount < BrainCellsNum; cellsCount++) {
				outp[cellsCount] = brainD[cellsCount].thinK(inp);
				//printf("\n == %f \t ", outp[2]);
			}
	}

	void setScore(long double sc) {

		if (!deltaBool) {
			score = sc;
		}
		else {
			scoreD = sc;
			
		}



	}


	void mutation_brain() {



		
		//printf("\n ======Flag1===\n");

		if ((scorePrev >= score & score > deltaTreashold) | deltaBool ) {
		//	printf("\n ======Flag===\n");
			if (deltaCount < deltaMaxCount) {

			//	printf("\n ===SCB %f\tSCD %f\tSC %f\n", scoreBest, scoreD, score);
				deltaBest = false;
				//printf("\n ===DMute===\n");
				if (scoreD > scoreBest & deltaBool) {
				//	printf("\n ===SCB %f\tSCD %f\tSC %f\n", scoreBest, scoreD,score);
					scoreBest = scoreD;
					
					for (int i = 0; i < BrainCellsNum; i++) {
						//brainD[i] = brain[i];

						for (int lnum = 2; lnum <= layerCount; lnum++) {
							for (int nnum = 1; nnum <= brainD[i].l[lnum].nCount; nnum++) {
								for (int k = 1; k < lnum; k++) {
									for (int j = 1; j <= brainD[i].l[k].nCount; j++) {
										
										wDBest[i][brainD[i].l[lnum].nodes[nnum].nNum][brainD[i].l[k].nodes[j].nNum] = wD[i][brainD[i].l[lnum].nodes[nnum].nNum][brainD[i].l[k].nodes[j].nNum];
											
										
									}
								}
								bDBest[i][brainD[i].l[lnum].nodes[nnum].nNum] = bD[i][brainD[i].l[lnum].nodes[nnum].nNum];
								
							}
						}
					}
					

				}
			
				deltaBool = true;

				for (int i = 0; i < BrainCellsNum; i++) {
					brainD[i] = brain[i];

					for (int lnum = 2; lnum <= layerCount; lnum++) {
						for (int nnum = 1; nnum <= brainD[i].l[lnum].nCount; nnum++) {
							for (int k = 1; k < lnum; k++) {
								for (int j = 1; j <= brainD[i].l[k].nCount; j++) {
									if (brainD[i].l[lnum].nodes[nnum].n[brainD[i].l[k].nodes[j].nNum]) {
										wD[i][brainD[i].l[lnum].nodes[nnum].nNum][brainD[i].l[k].nodes[j].nNum] = (rand() % (int)(xfac * 1000) / 1000.0 - xfac / 2.0) * deltaD;
										brainD[i].l[lnum].nodes[nnum].w[brainD[i].l[k].nodes[j].nNum] += wD[i][brainD[i].l[lnum].nodes[nnum].nNum][brainD[i].l[k].nodes[j].nNum];
									}
								}
							}
							bD[i][brainD[i].l[lnum].nodes[nnum].nNum] = (rand() % (int)(xfac * 1000) / 1000.0 - xfac / 2.0) * deltaD;
							brainD[i].l[lnum].nodes[nnum].bias += bD[i][brainD[i].l[lnum].nodes[nnum].nNum];
						}
					}
				}

				deltaCount++;
			//	printf("\n ===deltaCount %d\n", deltaCount);

			}
			if(deltaCount>= deltaMaxCount) {
			//	printf("\n ===SCB %f\tSCD %f\tSC %f\n", scoreBest, scoreD, score);
				deltaBest = false;
				if (scoreBest >= score) {
			//		printf("\n ======Flag0===\n");
					deltaBest = true;
		//			printf("\n ====DELTA0====  %f \n", scoreBest - scorePrev);
					score = scoreBest;
					scoreBest = -1;
				}
				//deltaBest = true;
				deltaBool = false;
				deltaCount = 0;
				

			}
			



		}
		//printf("\n ======Flag===\n");
		if ((!deltaBool) & deltaBest) {
			//printf("\n ===============================================================DELTA====  %f \n", score - scorePrev);
			//if (score < scorePrev)printf("\n\n\n\n =========================================================================================FUUUUUDELTA====\n\n\n\n");
			for (int i = 0; i < BrainCellsNum; i++) {


				

				for (int lnum = 2; lnum <= layerCount; lnum++) {
					for (int nnum = 1; nnum <= brain[i].l[lnum].nCount; nnum++) {
						for (int k = 1; k < lnum; k++) {
							for (int j = 1; j <= brain[i].l[k].nCount; j++) {
								if (brain[i].l[lnum].nodes[nnum].n[brain[i].l[k].nodes[j].nNum]) {

									brain[i].l[lnum].nodes[nnum].w[brain[i].l[k].nodes[j].nNum] += wDBest[i][brainD[i].l[lnum].nodes[nnum].nNum][brainD[i].l[k].nodes[j].nNum];
								}
							}
						}
						brain[i].l[lnum].nodes[nnum].bias += bDBest[i][brainD[i].l[lnum].nodes[nnum].nNum];
					}
				}

				brain[i].mutate_links();

			}

		}
		else if (!deltaBool & !deltaBest) {
			for (int cellC = 0; cellC < BrainCellsNum; cellC++) {
				brain[cellC].mutation();
				
			}
			//printf("\n===FLAG MUTE======\n");
		}


		if (!deltaBool)
			scorePrev = score;
		else {
			//scorePrev = scoreD;
		}


	}

	void think_brainD(float inp[], float outp[]) {

		if (deltaCount < deltaMaxCount) {
			for (int i = 0; i < BrainCellsNum; i++) {
				brainD[i] = brain[i];
				
				for (int lnum = 2; lnum <= layerCount; lnum++) {
					for (int nnum = 1; nnum <= brainD[i].l[lnum].nCount; nnum++) {
						for (int k = 1; k < lnum; k++) {
							for (int j = 1; j <= brainD[i].l[k].nCount; j++) {
								if (brainD[i].l[lnum].nodes[nnum].n[brainD[i].l[k].nodes[j].nNum]) {
									wD[i][brainD[i].l[lnum].nodes[nnum].nNum][brainD[i].l[k].nodes[j].nNum] = (rand() % (int)(xfac * 1000) / 1000.0 - xfac / 2.0) * deltaD;

									brainD[i].l[lnum].nodes[nnum].w[brainD[i].l[k].nodes[j].nNum] += wD[i][brainD[i].l[lnum].nodes[nnum].nNum][brainD[i].l[k].nodes[j].nNum];
								}


							}
						}

						bD[i][brainD[i].l[lnum].nodes[nnum].nNum]= (rand() % (int)(xfac * 1000) / 1000.0 - xfac / 2.0)*deltaD;

						brainD[i].l[lnum].nodes[nnum].bias += bD[i][brainD[i].l[lnum].nodes[nnum].nNum];

					}
				}

			}






		}








	}


};

