#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include "../../graphtinker/structs/structs.h"

void process_edges_incr(
		unsigned int it,
		VProperty_t *vtemp_prop,
		VProperty_t *v_prop,
		#ifdef EN_ARRAY_HEBA
		Edge_nt * gt_heba,
		#else
		vector<Edge_nt> & gt_heba, 
		#endif
		#ifdef EN_ARRAY_HEBA
		???
		#else
		vector<unsigned int> active_vertices
		#endif
		){
	
	unsigned int validedgesprocessed = 0;
	vector<uint_clusterptr_tt> clusterptrs;
	for(unsigned int id=0; id < active_vertices.size(); id += 1){

		clusterptrs.clear();
		
		unsigned int hvtx_vid;
		unsigned int xvtx_vid = active_vertices[id];
		clusterptrs.push_back(xvtx_vid);
		unsigned int len = clusterptrs.size();		
		unsigned int wps = WORK_BLOCKS_PER_SUBBLOCK;
		
		while(true){
			for(unsigned int i=0; i<len; i++){		
				hvtx_vid = clusterptrs.back();
				clusterptrs.pop_back();
				
				unsigned int hebaoffset = get_EBoffset(hvtx_vid);
				for(unsigned int t=0; t<WORK_BLOCKS_PER_PAGE; t++){
					Edge_nt edgeset = gt_heba[(hebaoffset + t)];
					
					for(unsigned int k=0; k<WORK_BLOCK_HEIGHT; k++){
						if(edgeset.edges[k].flag == VALID){

							// process edge starts here
							unsigned int uprop = v_prop[xvtx_vid].data;
							unsigned int edgew = edgeset.edges[k].edge_weight;
							unsigned int xadjvtx_id = edgeset.edges[k].xadjvtx_id;				
							
							uint_vprop_tt res = 
								process_edge(
										uprop, 
										edgew
										);
										
							VProperty_t temp = 
								read_vtmpproperty(
										xadjvtx_id,
										vtemp_prop
										);
										
							uint_vprop_tt res2 = reduce(
										temp.data, 
										res
										);
										
							write_vtmpproperty(
										xadjvtx_id, 
										res2,
										vtemp_prop
										);
										
							// process edge ends here
							validedgesprocessed += 1;
						}
					}
					
					if(((t%wps)==0) && (edgeset.clusterinfo.flag == VALID)){
						clusterptrs.push_back(edgeset.clusterinfo.data); 
					}
				}
			}
		
			len = clusterptrs.size();
			if(len == 0){ break; }
		}
	}
	/* cout<<"incr processing finished. "<<validedgesprocessed<<" edges processed (process_edges_full)"<<endl; */
	return;
}
