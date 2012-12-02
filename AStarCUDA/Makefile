# Makefile for building the TFTP server program

# *****************************************************
# Variables to control Makefile operation

COMPILER = g++
FLAGS = -Wall -g
GRAPH_DATA = GraphData.o
HEADERS = GraphData.h StringSplit.h PQ.h
PQ_HEADERS = PQ.h alloc.h common.h
PQ_OBJ = PQ.o
CUDA_ASTAR = CudaAStar.o
# ****************************************************
# Targets needed to bring the executable up to date
all: cudaAStar

cudaAStar: $(CUDA_ASTAR) $(GRAPH_DATA) $(PQ_OBJ)
	$(COMPILER) $(FLAGS) -o astarcuda $(CUDA_ASTAR) $(GRAPH_DATA) $(PQ_OBJ)

PQ.o : $(PQ_HEADERS)
GraphData.o: $(HEADERS)
CudaAStar.o: $(HEADERS)


clean: 
	@rm -f *.o astarcuda