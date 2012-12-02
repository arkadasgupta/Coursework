# Makefile for building the TFTP server program

# *****************************************************
# Variables to control Makefile operation

COMPILER = g++
FLAGS = -Wall -g
GRAPH_DATA = GraphData.o
HEADERS = GraphData.h StringSplit.h
PQ_HEADERS = pqueue.h
PQ_OBJ = pqueue.o
CUDA_ASTAR = CudaAStar.cpp
CUST_QUEUE_HDR = customQueue.h
# ****************************************************
# Targets needed to bring the executable up to date
all: cudaAStar

cudaAStar: $(GRAPH_DATA) 
	$(COMPILER) $(FLAGS) $(CUDA_ASTAR) $(GRAPH_DATA) $(CUST_QUEUE_HDR) -o astarcuda 

#pqueue.o : $(PQ_HEADERS)
GraphData.o: $(HEADERS)
#CudaAStar.o: $(HEADERS)


clean: 
	@rm -f *.o astarcuda