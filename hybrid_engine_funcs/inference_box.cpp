#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include "../../graphtinker/structs/structs.h"

/* void inference_box(
		unsigned int batch,
		unsigned int *proc_type,
		GEStatsUnit_t inferencebox_stats,
		unsigned int num_active_vertices
	){
	unsigned int T1 = inferencebox_stats.statsval1; // count of number of active vertices for next iteration	
	unsigned int T2 = inferencebox_stats.statsval2; //count of total degrees of active vertices for next iteration
	
	unsigned int special_k1 = 10;
	unsigned int special_k2 = 100;
	unsigned int special_k3 = (special_k1 * 1048576);
	unsigned int currentbatchno = batch + 1; // batch with id=0 is the first batch
	
	unsigned int edgesloadedsofar = (currentbatchno * 1048576);
	
	unsigned int next_block_id = (edgesloadedsofar + (special_k3 - 1)) / special_k3; // here, 3M=>10M;  98M=>100M
	unsigned int next_block = next_block_id * special_k3;	
	unsigned int threshold = next_block / special_k2;
	cout<<"threshold = "<<threshold<<", currentbatchno="<<currentbatchno<<", T1="<<T1<<", T2="<<T2<<" (inferenceBox)"<<endl;
	
	if(T1>threshold && T2>threshold){
		*proc_type=FULLPROC; 
	} else {
		*proc_type=INCRPROC;
	}
	
	return;
} */

void inference_box(
		unsigned int batch,
		unsigned int *proc_type,
		GEStatsUnit_t inferencebox_stats,
		unsigned int num_active_vertices
	){
	if(num_active_vertices == 0){ *proc_type=FULLPROC; return; }
	
	unsigned int edgesloadedsofar = ((batch + 1) * 1048576);
	unsigned int K = edgesloadedsofar / num_active_vertices;
	if(K < 50){ 
		*proc_type=FULLPROC; 
	} else {
		*proc_type=INCRPROC;
	}
	cout<<"threshold=50, edgesloadedsofar="<<edgesloadedsofar<<", K="<<K<<" (inferenceBox)"<<endl;
	return;
}