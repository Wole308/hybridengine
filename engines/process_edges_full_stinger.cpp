#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include "/net/zf15/oj2zf/graphtinker/structs/structs.h"

void process_edges_full_stinger(
		unsigned int it,
		VProperty_t *vtemp_prop,
		VProperty_t *v_prop,
		struct stinger * S,
		unsigned int heba_validheight
		){
			
	unsigned int validedgesprocessed = 0;
	
	/* cout<<"heba_validheight : "<<heba_validheight<<endl;
	cout<<"HEDGEBLOCKARRAYHEIGHT : "<<HEDGEBLOCKARRAYHEIGHT<<endl; */
	for(unsigned int xvtx_id=0; xvtx_id < heba_validheight; xvtx_id += 1){
		unsigned int uprop = v_prop[xvtx_id].data;	
	
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
				
				/* if(it==0){
					if(uprop < INFINITI){ 
						cout<<"valid process_edge operation seen5 : vtemp_prop["<<n<<"].data : "<<vtemp_prop[n].data
						<<" uprop : "<<uprop
						<<", res : "<<res
						<<", res2 : "<<res2
						<<", temp.data : "<<temp.data
						<<", xvtx_id : "<<xvtx_id
						<<", n : "<<n
						<<", w : "<<w
						<<", t : "<<t<<endl; 
					}
				}	 */					
				// process edge ends here
			}
			
		  } STINGER_PARALLEL_FORALL_EDGES_OF_VTX_END();		  
		  validedgesprocessed += (kout < max_outlen)?kout:max_outlen; 
	}	
	/* cout<<"stinger full processing finished. "<<validedgesprocessed<<" edges processed (process_edges_full)"<<endl; */
	return;
}
