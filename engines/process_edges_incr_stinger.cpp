#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include "/net/zf15/oj2zf/graphtinker/structs/structs.h"

void process_edges_incr_stinger(
		unsigned int it,
		VProperty_t *vtemp_prop,
		VProperty_t *v_prop,
		struct stinger * S,
		#ifdef EN_ARRAY_HEBA
		unsigned int * active_vertices,
		unsigned int active_vertices_size
		#else
		vector<unsigned int> active_vertices
		#endif
		){
	
	unsigned int validedgesprocessed = 0;
	#ifdef EN_ARRAY_HEBA
	for(unsigned int id=0; id < active_vertices_size; id += 1){
	#else
	for(unsigned int id=0; id < active_vertices.size(); id += 1){
	#endif

		unsigned int xvtx_id = active_vertices[id];
		unsigned int uprop = v_prop[active_vertices[id]].data;	

		size_t kout = 0; 
		// assert (S); 
		size_t max_outlen = INFINITI; 
		unsigned int slide = 0;
		STINGER_PARALLEL_FORALL_EDGES_OF_VTX_BEGIN(S, xvtx_id) {
		const int64_t n = STINGER_EDGE_DEST;
		const int64_t w = STINGER_EDGE_WEIGHT;
		const int64_t tf = STINGER_EDGE_TIME_FIRST;
		const int64_t tr = STINGER_EDGE_TIME_RECENT;
		const int64_t t = STINGER_EDGE_TYPE;
		
		if (n >= 0) { 
		  size_t where = stinger_size_fetch_add (&kout, 1);
		  
			// process edge starts here
			uint_vprop_tt res = 
				process_edge(
						uprop, 
						w
						);
					
			VProperty_t temp = 
				read_vtmpproperty(
						n,
						vtemp_prop
						);
			
			uint_vprop_tt res2 = reduce(
						temp.data, 
						res
						);
			
			write_vtmpproperty(
						n, 
						res2,
						vtemp_prop
						);							
			// process edge ends here
		}
		
	  } STINGER_PARALLEL_FORALL_EDGES_OF_VTX_END();		  
	  validedgesprocessed += (kout < max_outlen)?kout:max_outlen;
	}
	/* cout<<"stinger incr processing finished. "<<validedgesprocessed<<" edges processed (process_edges_full)"<<endl; */
	return;
}
