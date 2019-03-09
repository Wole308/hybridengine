#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include "../../graphtinker/structs/structs.h"

void process_edges_full(
		unsigned int it,
		VProperty_t *vtemp_prop,
		VProperty_t *v_prop,
		LLEdgeBlock_t *ll_heba0,
		LLLogicalVertexEntity_t *ll_lva0,
		LLEdgeBlockTracker_t ll_tracker0,
		unsigned int heba_validheight
		){
			
	unsigned int validedgesprocessed = 0;
			
	for(unsigned int start_xvtxid=0; start_xvtxid < heba_validheight; start_xvtxid += LVACOARSENESSWIDTH){
		edge_t edge;
		unsigned int curraddr;
		LLLogicalVertexEntity_t entity;
		uint_vid_tt lvaindex;
		
		
		lvaindex = start_xvtxid / LVACOARSENESSWIDTH;
		entity = ll_lva0[lvaindex];
		
		if(entity.flag != INVALID){
			curraddr = entity.baseaddr;
			
			while(true){
				for(unsigned int j=0; j<LLEDGEBLOCKSIZE; j++){
					edge = ll_heba0[curraddr].LLEdgeblock[j];
					if(edge.flag == VALID){
						
						// process edge starts here
						unsigned int uprop = v_prop[edge.xvtx_id].data;
						unsigned int edgew = edge.edgew;
						unsigned int xadjvtx_id = edge.xadjvtx_id;				
						
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
						/* if(it==0){
						if(uprop < INFINITI){ 
						cout<<"valid process_edge operation seen5 : vtemp_prop["<<xadjvtx_id<<"].data : "<<vtemp_prop[xadjvtx_id].data
						<<" uprop : "<<uprop
						<<", res : "<<res
						<<", res2 : "<<res2
						<<", temp.data : "<<temp.data
						<<", edge.xvtx_id : "<<edge.xvtx_id
						<<", xadjvtx_id : "<<xadjvtx_id
						<<", edgew : "<<edgew<<endl; 
						}
						} */
						// process edge ends here
						validedgesprocessed += 1;
					}
				}
				
				// load more?
				unsigned int next_cptr = ll_heba0[curraddr].metadata.nextCPtr;
				if(next_cptr == NAv){ break; }
				else { curraddr = next_cptr; }
			}
		}		
	}
	/* cout<<"full processing finished. "<<validedgesprocessed<<" edges processed (process_edges_full)"<<endl; */
	return;
}
