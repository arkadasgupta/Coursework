# Makefile for building the TFTP server program

# *****************************************************
# Variables to control Makefile operation

COMPILER = g++
FLAGS = -Wall -g
GRAPH_DATA = GraphData.o
HEADERS = GraphData.h StringSplit.h
#PQ_HEADERS = pqueue.h
#PQ_OBJ = pqueue.o
ASTAR_OCL = OpenCLAStar.cpp
#CUST_QUEUE_HDR = customQueue.h
OPENCL_COMMON_OBJ = OCLCommon.o
OPENCL_FILES = OCLCommon.cpp OCLCommon.h
OPENCL_FLAG = -I/opt/AMDAPP/include -L/opt/AMDAPP/lib -lOpenCL
# ****************************************************
# Targets needed to bring the executable up to date
all: cudaAStar

cudaAStar: $(GRAPH_DATA) 
	$(COMPILER) $(FLAGS) $(ASTAR_OCL) $(OPENCL_FILES) $(OPENCL_FLAG) $(GRAPH_DATA) -o astarocl 

#pqueue.o : $(PQ_HEADERS)
GraphData.o: $(HEADERS)
#CudaAStar.o: $(HEADERS)
#OCLCommon.o : OCLCommon.h
#	     $(COMPILER) $(FLAGS) OCLCommon.cpp $(OPENCL_FLAG) 
clean: 
	@rm -f *.o astarocl