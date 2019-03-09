#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include "../../graphtinker/structs/structs.h" // Courtesy : Jinja 2.0

/// explanation
/// reason why numofvalidreduceops might not be == numofsuccessfulreduceops :
/// root vertex, 1st iteration : root reduces 4 dst vertices (numofsuccessfulreduceops = 4)
/// some vertices, 1st iteration : sees root, and the vertices root has reduces (numofvalidreduceops = numofsuccessfulreduceops + x)

uint_vprop_tt reduce(
				uint_vprop_tt tmp,
				uint_vprop_tt res
				){		
		return reduce_BFS(
					tmp, 
					res
					);
	
	
	
	
}

uint_vprop_tt reduce_BFS(
				uint_vprop_tt tmp, 
				uint_vprop_tt res
				){
	if(res < tmp){
		return res;
	} else if(res > tmp){
		return tmp;
	}
	return res;
}

uint_vprop_tt reduce_SSSP(
				uint_vprop_tt tmp, 
				uint_vprop_tt res
				){
	if(res < tmp){
		return res;
	} else if(res > tmp){
		return tmp;
	}
	return res;
}

uint_vprop_tt reduce_CC(
				uint_vprop_tt tmp, 
				uint_vprop_tt res
				){
	if(res < tmp){
		return res;
	} else if(res > tmp){
		return tmp;
	}
	return res;
}

uint_vprop_tt reduce_PR(
				uint_vprop_tt tmp, 
				uint_vprop_tt res
				){
	tmp = tmp + res;	
	return tmp;
}











