#pragma once
//#include<thread>
//#include <stdio.h>    
//#include <stdlib.h> 

#include<iostream>
//#include <chrono>
//#include<time.h>

#include<math.h>



#define xfac 10
#define mutationLinkRate 0.08	//<1
#define mutationWandBRate 0.2	//<1


#define inputNodesCount 10
#define outputNodesCount 1
//#define h1NodesCount 6
//#define h2NodesCount 5

#define hiddenLayersCount 3
#define hiddenNodesCount 6
#define nodeCount inputNodesCount+outputNodesCount+hiddenLayersCount*hiddenNodesCount+1
#define layerMaxNodeCount 11

#define layerCount 2+hiddenLayersCount

#define mutateLinks true
#define mutateWandB true

class nnN1
{
public:
	struct node
	{
		bool n[nodeCount];
		double w[nodeCount];
		double bias;
		double inVal;
		double outVal;
		bool nEnable;
		int nNum;
	};

	struct layer
	{
		node nodes[layerMaxNodeCount];
		int nCount;

	};

	float linksCount = 0;
	float enableNodeCount;
	float maxLinks;
	float maxEnNodes = nodeCount;

	//long double score;



	layer l[layerCount + 1];

	node nodes[nodeCount + layerCount];

	void initialize() {
		layer input,output, h[hiddenLayersCount];
		//srand(time(0));

		for (int i = 0; i <= nodeCount; i++) {
			nodes[i].nNum = i;
			for (int j = 0; j <= nodeCount; j++)
			{

				nodes[i].n[j] = false;
				nodes[i].w[j] =  rand() % 2000 / 1000.0f - 1;
				nodes[i].bias = 0;
				nodes[i].inVal = 0;
				nodes[i].outVal = 0;
			}
		}

		int nodeN = 1;

		input.nCount = inputNodesCount;
		output.nCount = outputNodesCount;

		for (int i = 1; i <= inputNodesCount; i++) {
			input.nodes[i] = nodes[nodeN];
			nodeN++;
		}
		for (int i = 1; i <= outputNodesCount; i++) {
			output.nodes[i] = nodes[nodeN];
			nodeN++;
		}
		for (int j = 0; j < hiddenLayersCount; j++) {
			for (int i = 1; i <= hiddenNodesCount; i++) {
				h[j].nCount = hiddenNodesCount;
				h[j].nodes[i] = nodes[nodeN];
				nodeN++;
			}
		}


		l[1] = input;
		//l[2] = h[0];
		//l[3] = h[1];
		l[layerCount] = output;

		for (int i = 2; i < layerCount; i++) {
			l[i] = h[i - 2];
		}


		
		/*
		int nodeN = 1;
		for (int i = 1; i <= inputNodesCount; i++) {
			input.nodes[i] = nodes[nodeN];
			nodeN++;
		}
		for (int i = 1; i <= outputNodesCount; i++) {
			output.nodes[i] = nodes[nodeN];
			nodeN++;
		}
		for (int i = 1; i <= h1NodesCount; i++) {
			h1.nodes[i] = nodes[nodeN];
			nodeN++;
		}
		for (int i = 1; i <= h2NodesCount; i++) {
			h2.nodes[i] = nodes[nodeN];
			nodeN++;
		}

		l[1] = input;
		l[2] = h1;
		l[3] = h2;
		l[4] = output;
		*/




		

		for (int i = 2; i <= layerCount; i++) {
			maxLinks += l[i - 1].nCount * l[i].nCount;
		}

		//printf("\n\n= = = = = = NN initialized = = = = = =\n\n");
	}




	float sigmoid(float val) {
		
		return(1 / (1 + 1 / exp(val)) * 2 - 1);
		
	}

	float x_10(float val) {
		float func;

		func = val / 10;
		if (func > 1)func = 1;
		if (func < -1)func = -1;
		
		return(func);
	}

	float thinK(float inp[]) {

		float outp;

		for (int i = 1; i <= l[1].nCount; i++) {
			l[1].nodes[i].outVal = (double)inp[i - 1];
		}

		layersCalc();

		

		outp = (float)l[layerCount].nodes[1].outVal;
		
		return(outp);
	}

	void layersCalc() {
		double val = 0;
		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				val = 0;
				for (int j = 1; j <= (lnum - 1); j++) {
					for (int k = 1; k <= l[j].nCount; k++) {
						if (l[lnum].nodes[i].n[l[j].nodes[k].nNum])
							val += l[j].nodes[k].outVal * l[lnum].nodes[i].w[l[j].nodes[k].nNum];
					}
				}
				val += l[lnum].nodes[i].bias;
				l[lnum].nodes[i].outVal = sigmoid(val);
			}
		}


	}

	void mutation() {

		int bn;
		//if ((rand() % 20000 / 1000.0f < mutationLinkRate) & mutateLinks) initialize();
		bool bmutated = false;
		/*
		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				if ((rand() % 100000 / 100000.0f < mutationLinkRate) & mutateLinks) {
					bn = rand() % (nodeCount + 1);
					l[lnum].nodes[i].n[bn] = !l[lnum].nodes[i].n[bn];
				}
			}
		}
		while(bmutated==false)
			for (int lnum = 2; lnum <= layerCount; lnum++) {
				for (int i = 1; i <= l[lnum].nCount; i++) {
					if (mutateWandB)
						for (int bn = 1; bn <= nodeCount; bn++)
							if ((rand() % 10000 / 10000.0f < mutationWandBRate) & l[lnum].nodes[i].n[bn] & !bmutated) {
								bmutated = true;
								l[lnum].nodes[i].w[bn] += rand() % (int)(xfac * 1000) / 1000.0f - xfac / 2.0f;
							}
					if ((rand() % 10000 / 10000.0f < mutationWandBRate) & mutateWandB & !bmutated) {
						l[lnum].nodes[i].bias += rand() % (int)(xfac * 1000) / 1000.0f - xfac / 2.0f;
						bmutated = true;
					}


				}

				linksNodesCount();
			}

			*/

		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				if ((rand() % 100000 / 100000.0f < mutationLinkRate) & mutateLinks) {
					bn = rand() % (nodeCount + 1);
					l[lnum].nodes[i].n[bn] = !l[lnum].nodes[i].n[bn];
				}
				if(mutateWandB)
					for(int bn=1;bn<=nodeCount;bn++)
				      if ((rand() % 10000 / 10000.0f < mutationWandBRate)& l[lnum].nodes[i].n[bn]) {
				
					   l[lnum].nodes[i].w[bn] += rand() % (int)(xfac * 1000) / 1000.0 - xfac / 2.0;
				       }
				if ((rand() % 10000 / 10000.0f < mutationWandBRate/5) & mutateWandB)
					l[lnum].nodes[i].bias += rand() % (int)(xfac * 1000) / 1000.0 - xfac / 2.0;


			}

			linksNodesCount();
		}

	}


	void mutate_links() {

		int bn;
	
		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				if ((rand() % 100000 / 100000.0f < mutationLinkRate) & mutateLinks) {
					bn = rand() % (nodeCount + 1);
					l[lnum].nodes[i].n[bn] = !l[lnum].nodes[i].n[bn];
				}
			}
		}
	}


	void linksNodesCount() {

		for (int i = 1; i <= l[1].nCount; i++) l[1].nodes[i].nEnable = true;
		for (int i = 1; i <= l[layerCount].nCount; i++) l[layerCount].nodes[i].nEnable = true;

		for (int lnum = 2; lnum < layerCount; lnum++) {
			for (int i = 1; i <= l[2].nCount; i++) l[lnum].nodes[i].nEnable = false;
		}
		
		enableNodeCount = l[1].nCount + l[layerCount].nCount;
		linksCount = 0;

		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				for (int j = 1; j <= (lnum - 1); j++) {
					for (int k = 1; k <= l[j].nCount; k++) {
						if (l[lnum].nodes[i].n[l[j].nodes[k].nNum]) {
							l[lnum].nodes[i].nEnable = true;
							l[j].nodes[k].nEnable = true;
							linksCount++;
						}
					}
				}
			}
		}
		for (int lnum = 2; lnum <= layerCount - 1; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				if (l[lnum].nodes[i].nEnable)
					enableNodeCount++;
			}
		}

	}



	float sigmDeriv(float val) {

		return(val * (1 - val));
	}

};

