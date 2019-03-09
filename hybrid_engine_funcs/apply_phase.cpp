#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include "../../graphtinker/structs/structs.h"

void apply_phase(
		VProperty_t *vtemp_prop,
		VProperty_t *v_prop,
		GEStatsUnit_t *inferencebox_stats,
		#ifdef EN_ARRAY_HEBA
		unsigned int *active_vertices,
		unsigned int *active_vertices_size
		#else
		vector<unsigned int> & active_vertices
		#endif
		){	
	for(unsigned int i=0; i<HEDGEBLOCKARRAYHEIGHT; i++){
		VProperty_t vprop = v_prop[i]; 
		VProperty_t temp = vtemp_prop[i]; 
		
		if(vprop.data != temp.data){
			temp.data = MIN_type1(vprop.data, temp.data);
			
			
			
			
			v_prop[i].data = temp.data;
			#ifdef EN_ARRAY_HEBA
			active_vertices[*active_vertices_size] = i;
			*active_vertices_size += 1;
			#else 
			active_vertices.push_back(i);
			#endif
			
			inferencebox_stats->statsval1 += 1;	// for inference box		
			inferencebox_stats->statsval2 += (temp.indegree + temp.outdegree);
		}
		
		v_prop[i].flag = INVALID;
	}
	return;
}