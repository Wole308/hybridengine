import argparse
import xml.etree.ElementTree as ET
import os
from jinja2 import Environment, FileSystemLoader
import math
import sys

context = {}
print('number of arguments : ' + str(len(sys.argv)))

#### system variables
context['device'] = sys.argv[1] # CPU, FPGA
context['codelanguage'] = sys.argv[2] # C(Stinger), CPP(GraphTinker)
context['LLFULLDB'] = sys.argv[3] # ON, OFF
context['SGHashing'] = sys.argv[4] # ON, OFF
context['VPropUpdate'] = sys.argv[5] # ON, OFF
context['EBAContainerType'] = sys.argv[6] # ON, OFF
context['runtype'] = sys.argv[7] # CPU{PARALLEL, SERIAL}, FPGA{CPUEM, VHLS, HW}
context['engine'] = sys.argv[8] # gds, GE, ALL
context['mickeymouse'] = sys.argv[9] # mickeymouseON/OFF
context['config'] = sys.argv[10] # NOT USED
context['WORK_BLOCK_HEIGHT'] = int(sys.argv[11]) # WORK_BLOCK_HEIGHT
context['SUB_BLOCK_HEIGHT'] = int(sys.argv[12]) # SUB_BLOCK_HEIGHT ***
context['PAGE_BLOCKHEIGHT'] = int(sys.argv[13])  # PAGE_BLOCKHEIGHT
context['EDGEFIFOSIZE'] = int(sys.argv[14]) # *** NB: EDGEFIFOSIZE should be an integer factor of EDGESTREAMLENGHT!!!
context['GT_DRAMBanks'] = int(sys.argv[15])  # GT_DRAMBanks ***
context['graphbenchmark'] = ''+str(sys.argv[16]) # graphbenchmark ***
context['NO_OF_VERTICES'] = int(sys.argv[17]) # --change happened here //*** ('50' is extra just to avoid divide inaccuracies) --correct later //***
context['NO_OF_EDGES'] = int(sys.argv[18]) # --change happened here //*** ('50' is extra just to avoid divide inaccuracies) --correct later //***
context['changingparam'] = sys.argv[19] # NOT USED
context['batchcount'] = str(sys.argv[20]) #batchcount {5, 'INFINITI'} //NB: to extrapolate, use the right combination of 14 and 15 
context['inputsize'] = str(sys.argv[21]) #should=NO_OF_EDGES if you're running on entire edge dataset	
context['graphEngineType'] = str(sys.argv[22]) #'MONOGRAPHENGINE', 'HYBRIDGRAPHENGINE'
context['ProcType'] = str(sys.argv[23]) #'FULLPROC', 'INCRPROC'	
context['algoID'] = str(sys.argv[24]) #BFS #SSSP #CC #TC #PR
context['rootUserVPropertyID'] = str(sys.argv[25])
context['host'] = str(sys.argv[26])
context['graphdirectiontype'] = str(sys.argv[27])
context['insertion_rate'] = str(sys.argv[28])
context['deletion_type'] = str(sys.argv[29])
context['deletion_rate'] = str(sys.argv[30])
	
# edge condition handling
context['NO_OF_VERTICES'] = int((context['NO_OF_VERTICES'] + (context['PAGE_BLOCKHEIGHT'] - 1))/context['PAGE_BLOCKHEIGHT']) * context['PAGE_BLOCKHEIGHT'] # ensures MAX_INDEX_FINDABLE_IN_A_PARTITION is a factor of HEDGEBLOCKARRAYHEIGHT
	
# defaults
context['DEFAULT_SUB_BLOCKS_PER_PAGE'] = 8

print ('GraphTinker configured parameters (Courtesy: Jinja 2.0)...')
print ('device: ' + str(context['device']))
print ('codelanguage: ' + str(context['codelanguage']))
print ('LLFULLDB: ' + str(context['LLFULLDB']))
print ('SGHashing: ' + str(context['SGHashing']))
print ('VPropUpdate: ' + str(context['VPropUpdate']))
print ('EBAContainerType: ' + str(context['EBAContainerType']))
print ('runtype: ' + str(context['runtype']))
print ('engine: ' + str(context['engine']))
print ('mickeymouse: ' + str(context['mickeymouse']))
print ('config: ' + str(context['config']))
print ('WORK_BLOCK_HEIGHT: ' + str(context['WORK_BLOCK_HEIGHT']))
print ('SUB_BLOCK_HEIGHT: ' + str(context['SUB_BLOCK_HEIGHT']))
print ('PAGE_BLOCKHEIGHT: ' + str(context['PAGE_BLOCKHEIGHT']))
print ('EDGEFIFOSIZE: ' + str(context['EDGEFIFOSIZE']))
print ('NOOFDRAMBANKS: ' + str(context['GT_DRAMBanks']))
print ('graphbenchmark: ' + str(context['graphbenchmark']))
print ('NO_OF_VERTICES: ' + str(context['NO_OF_VERTICES']))
print ('NO_OF_EDGES: ' + str(context['NO_OF_EDGES']))
print ('changingparam: ' + str(context['changingparam']))
print ('batchcount: ' + str(context['batchcount']))
print ('inputsize: ' + str(context['inputsize']))
print ('graphEngineType: ' + str(context['graphEngineType']))
print ('ProcType: ' + str(context['ProcType']))
print ('algoID: ' + str(context['algoID']))
print ('rootUserVPropertyID: ' + str(context['rootUserVPropertyID']))
print ('host: ' + str(context['host']))
print ('graphdirectiontype: ' + str(context['graphdirectiontype']))
print ('insertion_rate: ' + str(context['insertion_rate']))
print ('deletion_type: ' + str(context['deletion_type']))
print ('deletion_rate: ' + str(context['deletion_rate']))
	
#================================
o_path0="../../hybridengine/engines/process_edges_full.cpp"
o_path1="../../hybridengine/engines/process_edges_incr.cpp"
o_path2="../../hybridengine/engines/graph_engine.cpp" 
o_path3="../../hybridengine/hybrid_engine_funcs/apply_phase.cpp" 
o_path4="../../hybridengine/hybrid_engine_funcs/inference_box.cpp" 
o_path5="../../hybridengine/hybrid_engine_funcs/load_vtmpprops.cpp" 
o_path6="../../hybridengine/hybrid_engine_funcs/process_edge.cpp" 
o_path7="../../hybridengine/hybrid_engine_funcs/read_vtmpproperty.cpp" 
o_path8="../../hybridengine/hybrid_engine_funcs/reduce.cpp" 
o_path9="../../hybridengine/engines/process_edges_full_stinger.cpp"
o_path10="../../hybridengine/engines/process_edges_incr_stinger.cpp"
o_path11="../../hybridengine/hybrid_engine_funcs/write_vtmpproperty.cpp"
o_path12="../../hybridengine/hybrid_engine_funcs/write_vtmpproperty.cpp"
o_path13="../../hybridengine/hybrid_engine_funcs/write_vtmpproperty.cpp"
o_path14="../../hybridengine/hybrid_engine_funcs/write_vtmpproperty.cpp"
o_path15="../../hybridengine/hybrid_engine_funcs/write_vtmpproperty.cpp"

out_path0=os.path.abspath(o_path0)
out_path1=os.path.abspath(o_path1)
out_path2=os.path.abspath(o_path2)
out_path3=os.path.abspath(o_path3)
out_path4=os.path.abspath(o_path4)
out_path5=os.path.abspath(o_path5)
out_path6=os.path.abspath(o_path6)
out_path7=os.path.abspath(o_path7)
out_path8=os.path.abspath(o_path8)
out_path9=os.path.abspath(o_path9)
out_path10=os.path.abspath(o_path10)
out_path11=os.path.abspath(o_path11)
out_path12=os.path.abspath(o_path12)
out_path13=os.path.abspath(o_path13)
out_path14=os.path.abspath(o_path14)
out_path15=os.path.abspath(o_path15)

templ_path0="../../hybridengine/engines"
templ_path1="../../hybridengine/engines/" 
templ_path2="../../hybridengine/engines/" 
templ_path3="../../hybridengine/hybrid_engine_funcs/" 
templ_path4="../../hybridengine/hybrid_engine_funcs/" 
templ_path5="../../hybridengine/hybrid_engine_funcs/" 
templ_path6="../../hybridengine/hybrid_engine_funcs/" 
templ_path7="../../hybridengine/hybrid_engine_funcs/" 
templ_path8="../../hybridengine/hybrid_engine_funcs/" 
templ_path9="../../hybridengine/engines/" 
templ_path10="../../hybridengine/engines/" 
templ_path11="../../hybridengine/hybrid_engine_funcs/" 
templ_path12="../../hybridengine/hybrid_engine_funcs/" 
templ_path13="../../hybridengine/hybrid_engine_funcs/" 
templ_path14="../../hybridengine/hybrid_engine_funcs/" 
templ_path15="../../hybridengine/hybrid_engine_funcs/"

#####################
### System parameters (General)
if(context['graphdirectiontype']=='DIRECTEDGRAPH'):
	context['AVERAGE_GRAPH_DEGREE'] = context['NO_OF_EDGES'] / context['NO_OF_VERTICES']
elif(context['graphdirectiontype']=='UNDIRECTEDGRAPH'):
	context['AVERAGE_GRAPH_DEGREE'] = (context['NO_OF_EDGES'] / context['NO_OF_VERTICES']) * 2
print ('average Graph degree: ' + str(context['AVERAGE_GRAPH_DEGREE']))
context['MAX_NO_OF_VERTICES'] = context['NO_OF_VERTICES']
context['GT_MainWorkUnits'] = context['GT_DRAMBanks']
context['TRIP_COUNT'] = 1 #context['NO_OF_EDGES']/context['EDGESTREAMLENGHT'] #how many times the FPGA device is called upon to compute
context['GLOBALVARLENGTH'] = 64 #variables and persist between FPGA runs
context['pyplotsmootheningfactor'] = 1

#####################
### System parameters (gds Main)
context['NOOFEDGESTREAMSETS'] = 8
context['EDGESTREAMLENGHT'] = context['NOOFEDGESTREAMSETS'] * context['EDGEFIFOSIZE'] # 8192 #size of stream send to the device per trip (constant) ***NB: EDGESTREAMLENGHT should be an integer multiple of EDGEFIFOSIZE***
context['PARTIALEDGEFIFOLENGHT'] = context['EDGESTREAMLENGHT']
context['EDGEFIFOLENGHT'] = context['PARTIALEDGEFIFOLENGHT']*context['GT_DRAMBanks'] #2DRAM banks
context['WORK_BLOCKS_PER_PAGE'] = context['PAGE_BLOCKHEIGHT']/context['WORK_BLOCK_HEIGHT'] # 16
context['SUB_BLOCKS_PER_PAGE'] = context['PAGE_BLOCKHEIGHT']/context['SUB_BLOCK_HEIGHT']
context['WORK_BLOCKS_PER_SUBBLOCK'] = context['SUB_BLOCK_HEIGHT']/context['WORK_BLOCK_HEIGHT']
context['HEDGEBLOCKARRAYHEIGHT'] = context['MAX_NO_OF_VERTICES'] / context['GT_DRAMBanks']
if(context['EBAContainerType']=='VECTOR_CONTAINER') :
	context['HEDGEBLOCKARRAYSIZE'] = context['HEDGEBLOCKARRAYHEIGHT'] * context['WORK_BLOCKS_PER_PAGE']
else :
	context['HEDGEBLOCKARRAYSIZE'] = context['HEDGEBLOCKARRAYHEIGHT'] * context['WORK_BLOCKS_PER_PAGE'] * 2 * (256/context['PAGE_BLOCKHEIGHT']) #* 2 #automate later! //*** // remove later (* 2)
context['EBACLUSTERMAPSIZE'] = context['HEDGEBLOCKARRAYHEIGHT'] #/100	
context['fastsearchSIMDunit_count'] = context['WORK_BLOCK_HEIGHT'] # 20 #8

context['pyplotlabel'] = 'bmrk'+str(context['graphbenchmark'])+'_ddr'+str(context['GT_DRAMBanks'])+'_sb'+str(context['SUB_BLOCK_HEIGHT'])+'_pw'+str(context['PAGE_BLOCKHEIGHT'])+'_bs'+str(context['EDGEFIFOSIZE'])
context['summarylabel'] = "device"+str(context['device'])+"_runtype"+str(context['runtype'])+"_engine"+str(context['engine'])+"_mickeymouse"+str(context['mickeymouse'])+"_config"+str(context['config'])+"_WORK_BLOCK_HEIGHT"+str(context['WORK_BLOCK_HEIGHT'])+"_SUB_BLOCK_HEIGHT"+str(context['SUB_BLOCK_HEIGHT'])+"_PAGE_BLOCKHEIGHT"+str(context['PAGE_BLOCKHEIGHT'])+"_NOOFEDGESTREAMSETS"+str(context['NOOFEDGESTREAMSETS'])+"_EDGEFIFOSIZE"+str(context['EDGEFIFOSIZE'])+"_NOOFDRAMBANKS"+str(context['GT_DRAMBanks'])+"_BENCHMARK"+str(context['graphbenchmark'])

context['runningthroughputX_stride'] = 10

if(context['batchcount'] == 'INFINITI'):
	context['approxNumOfBatches'] = (context['NO_OF_EDGES'] / (context['EDGESTREAMLENGHT'] * context['GT_DRAMBanks'])) * 2 #automate later! //***
else :
	context['approxNumOfBatches'] = context['batchcount']

#####################
### System parameters (GE)	
if(context['graphbenchmark']=='flickr'): # test benchmark
	context['WORK_BANK_HEIGHT'] = 16 #512 #128 # 64 # 16 --change happened here //***
else:
	context['WORK_BANK_HEIGHT'] = 2048 #512 #128 # 64 # 16 --change happened here //***
context['HALFWORK_BANK_HEIGHT'] = context['WORK_BANK_HEIGHT']/context['WORK_BLOCKS_PER_SUBBLOCK'] # formerly 2 //*** should be called something other than HALF...
context['WORK_BANK_SIZE'] = context['WORK_BANK_HEIGHT'] * context['WORK_BLOCKS_PER_PAGE']
context['HALFWORK_BANK_SIZE'] = context['WORK_BANK_SIZE']/context['WORK_BLOCKS_PER_SUBBLOCK'] #formerly 2 //*** should be called something other than HALF...
if(context['AVERAGE_GRAPH_DEGREE']<(context['PAGE_BLOCKHEIGHT']/4)):
	context['EDGETRAINLENGTH'] = context['WORK_BANK_SIZE'] * context['WORK_BLOCK_HEIGHT'] * (context['DEFAULT_SUB_BLOCKS_PER_PAGE'] / context['SUB_BLOCKS_PER_PAGE']) # expands as subblocks per page reduces --
else:
	context['EDGETRAINLENGTH'] = context['WORK_BANK_SIZE'] * context['WORK_BLOCK_HEIGHT'] * (context['DEFAULT_SUB_BLOCKS_PER_PAGE'] / context['SUB_BLOCKS_PER_PAGE']) * 4 # expands as subblocks per page reduces
	
context['NO_OF_VERTEX_PARTITIONS'] = context['SUB_BLOCKS_PER_PAGE']
context['MAX_INDEX_FINDABLE_IN_A_PARTITION'] = (context['MAX_NO_OF_VERTICES'] + (context['PAGE_BLOCKHEIGHT']-1))/context['PAGE_BLOCKHEIGHT'] # make as integer factor of MAX_NO_OF_VERTICES
context['SIZE_OF_EACH_VERTEX_PARTITION'] = context['MAX_INDEX_FINDABLE_IN_A_PARTITION'] * context['SUB_BLOCK_HEIGHT']
context['SIZE_OF_EACH_VERTEX_SUBPARTITION'] = (context['MAX_INDEX_FINDABLE_IN_A_PARTITION'] * context['SUB_BLOCK_HEIGHT'])/context['GT_DRAMBanks']

context['GE_WorkThreads'] = context['SUB_BLOCKS_PER_PAGE']
context['GE_trainlength'] = context['EDGETRAINLENGTH']

context['averageGEiterationlength'] = 100
context['GraphPlotCount'] = 2 #how many graphs we are py plotting. should correspond to context['GraphNames_seq']

context['GEGRAPHSLICECOUNT'] = 1 # 8 # 4 #1 #2 #number of graph slices --- #N:B. Only active when EN_GESLICING is enabled in struct
context['GEGRAPHSLICEWIDTH'] = context['SUB_BLOCKS_PER_PAGE'] / context['GEGRAPHSLICECOUNT'] #4 #how many times processing phase has to occur (on each partial graph slices)
context['GE_ActiveWorkThreads'] = context['GEGRAPHSLICEWIDTH'] # number of active workers for each graph slice

context['GE_NUMOFSUBSLICES'] = 1 # 2 --- #N:B. Only active when EN_GESUBSLICING is enabled in struct
context['GE_GRAPHSUBSLICEWIDTH'] = context['SIZE_OF_EACH_VERTEX_SUBPARTITION'] / context['GE_NUMOFSUBSLICES'] # context['HEDGEBLOCKARRAYHEIGHT'] / context['GE_NUMOFSUBSLICES']
context['SIZE_OF_EACH_VERTEX_SUBSLICEPARTITION'] = context['SIZE_OF_EACH_VERTEX_SUBPARTITION'] / context['GE_NUMOFSUBSLICES']

context['HYBRIDGRAPHENGINE_BRANCHTHRESHOLD'] = 100000 #100K 

#####################
### System constants
context['VALID'] = 5
context['INVALID'] = 6
context['METAVALID'] = 7
context['DELETED'] = 8 # means the data was formerly valid but later set invalid

context['YES'] = 1
context['NO'] = 0	
		
context['GRAPHTINKERDEVICEID'] = 1 #GraphTinker
context['GRAPHTINKERMINIONSDEVICEID'] = 2 #GraphTinkerMinions
context['GRAPHENGINEDEVICEID'] = 3 #GraphEngine		
		
#####################
### Sequencers		
context['fastsearchSIMDunit_seq'] = []
for i in range (0,context['fastsearchSIMDunit_count']):
		context['fastsearchSIMDunit_seq'].append(i)			
context['GE_WorkThreads_seq'] = []
for i in range (0,context['GE_WorkThreads']):
		context['GE_WorkThreads_seq'].append(i)
context['GE_trainlength_seq'] = []
for i in range (0,context['GE_trainlength']):
		context['GE_trainlength_seq'].append(i)			
context['GT_DRAMBankNames_seq'] = []
for i in range (0, context['GT_DRAMBanks']):
		c = chr(ord('A') + i)
		context['GT_DRAMBankNames_seq'].append(c)	
context['GE_PROC_TYPES_seq'] = []
context['GE_PROC_TYPES_seq'].append("FullProc")
context['GE_PROC_TYPES_seq'].append("IncrProc")			
context['GT_DRAMBanks_seq'] = []
for i in range (0, context['GT_DRAMBanks']):
		context['GT_DRAMBanks_seq'].append(i)		
context['GT_MainWorkUnits_seq'] = []
for i in range (0, context['GT_MainWorkUnits']):
		context['GT_MainWorkUnits_seq'].append(i)
context['WORK_BLOCKS_PER_PAGE_seq'] = []
for i in range (0,context['WORK_BLOCKS_PER_PAGE']):
		context['WORK_BLOCKS_PER_PAGE_seq'].append(i)
context['GraphNames_seq'] = []
context['GraphNames_seq'].append("GDSthroughput")
context['GraphNames_seq'].append("GEthroughput")
context['GE_ActiveWorkThreads_seq'] = []
for i in range (0,context['GE_ActiveWorkThreads']):
		context['GE_ActiveWorkThreads_seq'].append(i)		
context['GEGRAPHSLICECOUNT_seq'] = []
for i in range (0,context['GEGRAPHSLICECOUNT']):
		context['GEGRAPHSLICECOUNT_seq'].append(i)

#=========================
#=== all templates ===
env0 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path0)), trim_blocks=True, lstrip_blocks=True)
env1 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path1)), trim_blocks=True, lstrip_blocks=True)
env2 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path2)), trim_blocks=True, lstrip_blocks=True)
env3 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path3)), trim_blocks=True, lstrip_blocks=True)
env4 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path4)), trim_blocks=True, lstrip_blocks=True)
env5 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path5)), trim_blocks=True, lstrip_blocks=True)
env6 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path6)), trim_blocks=True, lstrip_blocks=True)
env7 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path7)), trim_blocks=True, lstrip_blocks=True)
env8 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path8)), trim_blocks=True, lstrip_blocks=True)
env9 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path9)), trim_blocks=True, lstrip_blocks=True)
env10 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path10)), trim_blocks=True, lstrip_blocks=True)
env11 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path11)), trim_blocks=True, lstrip_blocks=True)
env12 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path12)), trim_blocks=True, lstrip_blocks=True)
env13 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path13)), trim_blocks=True, lstrip_blocks=True)
env14 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path14)), trim_blocks=True, lstrip_blocks=True)
env15 = Environment(loader=FileSystemLoader(os.path.abspath(templ_path15)), trim_blocks=True, lstrip_blocks=True)

env0.globals.update(zip=zip)
env1.globals.update(zip=zip)
env2.globals.update(zip=zip)
env3.globals.update(zip=zip)
env4.globals.update(zip=zip)
env5.globals.update(zip=zip)
env6.globals.update(zip=zip)
env7.globals.update(zip=zip)
env8.globals.update(zip=zip)
env9.globals.update(zip=zip)
env10.globals.update(zip=zip)
env11.globals.update(zip=zip)
env12.globals.update(zip=zip)
env13.globals.update(zip=zip)
env14.globals.update(zip=zip)
env15.globals.update(zip=zip)

template0 = env0.get_template('process_edges_full.template')
template1 = env1.get_template('process_edges_incr.template') 
template2 = env2.get_template('graph_engine.template') 
template3 = env3.get_template('apply_phase.template') 
template4 = env4.get_template('inference_box.template') 
template5 = env5.get_template('load_vtmpprops.template') 
template6 = env6.get_template('process_edge.template') 
template7 = env7.get_template('read_vtmpproperty.template') 
template8 = env8.get_template('reduce.template') 
template9 = env9.get_template('process_edges_full_stinger.template') 
template10 = env10.get_template('process_edges_incr_stinger.template') 
template11 = env11.get_template('write_vtmpproperty.template') 
template12 = env12.get_template('write_vtmpproperty.template') 
template13 = env13.get_template('write_vtmpproperty.template') 
template14 = env14.get_template('write_vtmpproperty.template') 
template15 = env15.get_template('write_vtmpproperty.template')

# Render the template for the output file.
rendered_file0 = template0.render(context=context)
rendered_file1 = template1.render(context=context)
rendered_file2 = template2.render(context=context)
rendered_file3 = template3.render(context=context)
rendered_file4 = template4.render(context=context)
rendered_file5 = template5.render(context=context)
rendered_file6 = template6.render(context=context)
rendered_file7 = template7.render(context=context)
rendered_file8 = template8.render(context=context)
rendered_file9 = template9.render(context=context)
rendered_file10 = template10.render(context=context)
rendered_file11 = template11.render(context=context)
rendered_file12 = template12.render(context=context)
rendered_file13 = template13.render(context=context)
rendered_file14 = template14.render(context=context)
rendered_file15 = template15.render(context=context)

# Write output file
with open(out_path0, 'w') as outFile0:
	outFile0.write(rendered_file0)
with open(out_path1, 'w') as outFile1:
	outFile1.write(rendered_file1)
with open(out_path2, 'w') as outFile2:
	outFile2.write(rendered_file2)
with open(out_path3, 'w') as outFile3:
	outFile3.write(rendered_file3)
with open(out_path4, 'w') as outFile4:
	outFile4.write(rendered_file4)
with open(out_path5, 'w') as outFile5:
	outFile5.write(rendered_file5)
with open(out_path6, 'w') as outFile6:
	outFile6.write(rendered_file6)
with open(out_path7, 'w') as outFile7:
	outFile7.write(rendered_file7)
with open(out_path8, 'w') as outFile8:
	outFile8.write(rendered_file8)
with open(out_path9, 'w') as outFile9:
	outFile9.write(rendered_file9)	
with open(out_path10, 'w') as outFile10:
	outFile10.write(rendered_file10)
with open(out_path11, 'w') as outFile11:
	outFile11.write(rendered_file11)
with open(out_path12, 'w') as outFile12:
	outFile12.write(rendered_file12)
with open(out_path13, 'w') as outFile13:
	outFile13.write(rendered_file13)
with open(out_path14, 'w') as outFile14:
	outFile14.write(rendered_file14)
with open(out_path15, 'w') as outFile15:
	outFile15.write(rendered_file15)

print ("Successful!")
print ("...")


