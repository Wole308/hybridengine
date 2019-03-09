#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <omp.h>
using namespace std;
#include "../../graphtinker/structs/structs.h" // Courtesy : Jinja 2.0

void graph_engine(
		unsigned int proc_type,
		unsigned int rootvid,
		unsigned int edgeupdatecmd,
		unsigned int batch,
		VProperty_t * v_prop,
		#ifdef EN_ARRAY_HEBA
		edge_t * edgesv,
		unsigned int edgesv_size,
		#else
		vector<edge_t> edgesv,
		#endif		
		struct stinger * S,
		unsigned int heba_validheight,
		unsigned int total_edges_in_database,
		run_summary2_t * run_summary
		){	

	GEStatsUnit_t inferencebox_stats;
	inferencebox_stats.statsval1 = 0;
	inferencebox_stats.statsval2 = 0;
	
	unsigned int num_active_vertices = 0;
	
	unsigned int it = 0;
	
	#ifdef EN_ARRAY_HEBA
	unsigned int *active_vertices; active_vertices = new uint_vid_tt[HEDGEBLOCKARRAYHEIGHT]; 
	unsigned int *active_vertices_tmp; active_vertices_tmp = new uint_vid_tt[HEDGEBLOCKARRAYHEIGHT]; 
	unsigned int active_vertices_tracker = 0;
	unsigned int active_vertices_tmp_tracker = 0;
	#else
	vector<unsigned int> active_vertices;
	vector<unsigned int> active_vertices_tmp;
	#endif
	
	unsigned int totalnum_active_vertices = 0;	
	VProperty_t *vtemp_prop; vtemp_prop = new VProperty_t[HEDGEBLOCKARRAYHEIGHT]; 
	
	/// set root (if first batch) & initial active vertices
	cout<<"initializing root vertex ... "<<endl;
	// v_prop[2630].data = 0;
	v_prop[rootvid].data = 0;
	#ifdef EN_ARRAY_HEBA
	// active_vertices[active_vertices_tracker] = 2630;
	active_vertices[active_vertices_tracker] = rootvid;
	active_vertices_tracker += 1;
	#else
	// active_vertices.push_back(2630);
	active_vertices.push_back(rootvid);
	#endif
	
	/// set initial active vertices
	#ifdef EN_ARRAY_HEBA
	for(unsigned int i=0; i<edgesv_size; i++){
	#else 
	for(unsigned int i=0; i<edgesv.size(); i++){
	#endif
		unsigned int xvtx_id = edgesv[i].xvtx_id;
		unsigned int xadjvtx_id = edgesv[i].xadjvtx_id;
		VProperty_t v_propp = v_prop[xvtx_id];		
		
		if(v_propp.flag != VALID){
			#ifdef EN_ARRAY_HEBA
			active_vertices[active_vertices_tracker] = xvtx_id;
			active_vertices_tracker += 1;
			#else 
			active_vertices.push_back(xvtx_id);		
			#endif		
			v_prop[xvtx_id].flag = VALID;
			
			// stats collector information for Hybrid Graph Engine
			inferencebox_stats.statsval1 += 1;
			inferencebox_stats.statsval2 += (v_prop[xvtx_id].indegree + v_prop[xvtx_id].outdegree);				
		}
		
	}
	
	// load phase
		load_vtmpprops(
			vtemp_prop,
			v_prop
			);
	
	// inference phase
			
	while(true){
		
		run_summary[batch].ProcType[it] = proc_type;
		#ifdef EN_ARRAY_HEBA
		run_summary[batch].num_active_vertices[it] = active_vertices_tracker;
		#else
		run_summary[batch].num_active_vertices[it] = active_vertices.size();
		#endif
		std::clock_t tic = std::clock();
		
		// load phase
		load_vtmpprops(
			vtemp_prop,
			v_prop
			);
		
		// processing phase
		if(proc_type==FULLPROC){
			process_edges_full_stinger(
				it,
				vtemp_prop,
				v_prop,
				S,
				heba_validheight
				);
		} else if (proc_type==INCRPROC){
			process_edges_incr_stinger(
				it,
				vtemp_prop,
				v_prop,
				S,
				#ifdef EN_ARRAY_HEBA
				active_vertices,
				active_vertices_tracker
				#else
				active_vertices
				#endif
				);
		}

		// apply phase
		apply_phase(
			vtemp_prop,
			v_prop,
			&inferencebox_stats,
			#ifdef EN_ARRAY_HEBA
			active_vertices_tmp,
			&active_vertices_tmp_tracker
			#else
			active_vertices_tmp
			#endif
			);
			
		if(it > 100){ cout<<"bug! : out-of-range (graph_engine34)"<<endl; }
		float toc = (std::clock() - tic) / (double) (CLOCKS_PER_SEC / 1000);
		run_summary[batch].GElatency_LoadProcApplyPhase_us[it] = (float)(toc)*1000; 
		run_summary[batch].GElatency_LoadProcApplyPhase_ms[it] = (float)(toc);	
		run_summary[batch].GEthroughput_GE_edgespersec[it] = (total_edges_in_database / (float)(run_summary[batch].GElatency_LoadProcApplyPhase_ms[it])) * 1000;
		run_summary[batch].GEthroughput_GE_millionedgespersec[it] = ((total_edges_in_database / (float)(run_summary[batch].GElatency_LoadProcApplyPhase_ms[it])) * 1000) / 1000000;
		run_summary[batch].sumLoadProcApplylatency_ms += run_summary[batch].GElatency_LoadProcApplyPhase_ms[it];
		totalnum_active_vertices += run_summary[batch].num_active_vertices[it];	
		run_summary[batch].itcount = it + 1;
		cout<<"============================ LoadProcApply Latency for batch "<<batch<<", iteration "<<it<<" (msec) : "<<run_summary[batch].GElatency_LoadProcApplyPhase_ms[it]<<" msec, ProcType : "<<run_summary[batch].ProcType[it]<<", num_active_vertices : ["<<run_summary[batch].num_active_vertices[it]<<"] ============================ "<<endl;
		cout<<"============================ GE throughput for batch "<<batch<<", iteration "<<it<<" (million edges per sec) : "<<run_summary[batch].GEthroughput_GE_millionedgespersec[it]<<" million edges / sec, ProcType : "<<run_summary[batch].ProcType[it]<<", num_active_vertices : ["<<run_summary[batch].num_active_vertices[it]<<"] ============================ "<<endl;
		
		// load active vertices for next iteration
		#ifdef EN_ARRAY_HEBA
		active_vertices_tracker = 0;
		for (int i=0; i<active_vertices_tmp_tracker; i++){
			active_vertices[i] = active_vertices_tmp[i];
		}
		active_vertices_tracker = active_vertices_tmp_tracker;
		active_vertices_tmp_tracker = 0;
		num_active_vertices = active_vertices_tracker;
		#else
		active_vertices.clear();
		for (int i=0; i<active_vertices_tmp.size(); i++){
			active_vertices.push_back(active_vertices_tmp[i]);
		}
		active_vertices_tmp.clear();
		num_active_vertices = active_vertices.size();
		#endif
			
		// inference phase
	
		
		inferencebox_stats.statsval1=0;
		inferencebox_stats.statsval2=0;
		
		// exit conditions
		#ifdef EN_ARRAY_HEBA
		if(active_vertices_tracker == 0){ 
			cout<<"hybrid engine finished"<<endl;
			cout<<"active_vertices.size() : "<<active_vertices_tracker<<endl; 
			cout<<"total number of active vertices for batch "<<batch<<" : "<<totalnum_active_vertices<<endl;
			cout<<"run_summary["<<batch<<"].sumLoadProcApplylatency_ms = "<<run_summary[batch].sumLoadProcApplylatency_ms<<" ms (graph_engine)"<<endl;
			break; 
		}
		#else
		if(active_vertices.empty()){
			cout<<"hybrid engine finished"<<endl;
			cout<<"active_vertices.size() : "<<active_vertices.size()<<endl; 
			cout<<"total number of active vertices for batch "<<batch<<" : "<<totalnum_active_vertices<<endl;
			cout<<"run_summary["<<batch<<"].sumLoadProcApplylatency_ms = "<<run_summary[batch].sumLoadProcApplylatency_ms<<" ms (graph_engine)"<<endl;
			break; 
		}
		#endif
		if(it > 500) { cout<<"bug! : too many iterations (graph_engine)"<<endl; }
		it += 1;
	}
	
	cout<<"*** printing sum GE throughputs for each batch (graph_engine) ***"<<endl;
	cout<<"run_summary["<<batch<<"].throughput = "<<((((((batch + 1) * EDGESTREAMLENGHT) * run_summary[batch].itcount) / run_summary[batch].sumLoadProcApplylatency_ms) * 1000) / 1000000)<<" million edges per sec (test_updatesingleedge)"<<endl;
	#ifdef UNDIRECTEDGRAPH
	// if(batch >= ((unsigned int)((NO_OF_EDGES * 2) / EDGESTREAMLENGHT))){
	if(batch >= ((unsigned int)(NO_OF_EDGES / EDGESTREAMLENGHT))){
	#else 
	if(batch >= ((unsigned int)(NO_OF_EDGES / EDGESTREAMLENGHT))){
	#endif
		cout<<"printing sum GE throughputs for each batch"<<endl;
		float overall_average_throughput_for_all_batches = 0;
		if(edgeupdatecmd == INSERTEDGE){
			for(unsigned int z=0; z<batch; z++){
				cout<<"run_summary["<<z<<"].throughput = "<<((((((z + 1) * EDGESTREAMLENGHT) * run_summary[z].itcount) / run_summary[z].sumLoadProcApplylatency_ms) * 1000) / 1000000)<<" million edges per sec (test_updatesingleedge)"<<endl;
				overall_average_throughput_for_all_batches += ((((((z + 1) * EDGESTREAMLENGHT) * run_summary[z].itcount) / run_summary[z].sumLoadProcApplylatency_ms) * 1000) / 1000000);			
			}
		} else if(edgeupdatecmd != INSERTEDGE){
			for(unsigned int z=0; z<batch; z++){
				cout<<"run_summary["<<z<<"].throughput = "<<(((((NO_OF_EDGES - ((z + 1) * EDGESTREAMLENGHT)) * run_summary[z].itcount) / run_summary[z].sumLoadProcApplylatency_ms) * 1000) / 1000000)<<" million edges per sec (test_updatesingleedge)"<<endl;
				overall_average_throughput_for_all_batches += (((((NO_OF_EDGES - ((z + 1) * EDGESTREAMLENGHT)) * run_summary[z].itcount) / run_summary[z].sumLoadProcApplylatency_ms) * 1000) / 1000000);				
			}
		}
		cout<<"*=*=**=*=* Average GE throughput for all batches (overall_average_throughput_for_all_batches / batch) : "<<(overall_average_throughput_for_all_batches / batch)<<" million edges per sec (test_updatesingleedge) *=*=**=*=*"<<endl;
		cout<<"*=*=**=*=* Total number of active vertices observed : "<<totalnum_active_vertices<<" vertices (test_updatesingleedge) *=*=**=*=*"<<endl;
	}	
	
	return;
}