#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include "../../graphtinker/structs/structs.h" // Courtesy : Jinja 2.0

uint_vprop_tt process_edge(
					uint_vprop_tt uprop,
					uint_w_tt edgeweight
					){
	
	uint_vprop_tt newP; newP=0;
	
		return process_edge_BFS(
						uprop, 
						edgeweight
						);
	
	
	
	
}

uint_vprop_tt process_edge_BFS(
				uint_vprop_tt uprop,
				uint_w_tt edgeweight
				){	
	uint_vprop_tt newP;
	newP = uprop + 1;
	/* if((newP != uprop) && (newP < INFINITI)){ 
		cout<<"valid process_edge operation seen : uprop "<<uprop<<", newP : "<<newP<<", edgeweight : "<<edgeweight<<endl; 
	}	 */
	return newP;
}

uint_vprop_tt process_edge_SSSP(
				uint_vprop_tt uprop,
				uint_w_tt edgeweight
				){	
	uint_vprop_tt newP;
	newP = uprop + edgeweight;
	return newP;
}

uint_vprop_tt process_edge_CC(
				uint_vprop_tt uprop,
				uint_w_tt edgeweight
				){
	uint_vprop_tt newP;
	newP = uprop;
	return newP;
}

uint_vprop_tt process_edge_PR(
				uint_vprop_tt uprop,
				uint_w_tt edgeweight
				){	
	uint_vprop_tt newP;
	newP = uprop;	
	return newP;
}

