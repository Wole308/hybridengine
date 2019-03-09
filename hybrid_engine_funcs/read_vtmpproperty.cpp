#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#include "../../graphtinker/structs/structs.h" //Courtesy : Jinja 2.0

VProperty_t read_vtmpproperty(
				unsigned int xadjvtx_id,
				VProperty_t *vtemp_prop
				){	
	VProperty_t vtmp = vtemp_prop[xadjvtx_id];
	return vtmp;
}





