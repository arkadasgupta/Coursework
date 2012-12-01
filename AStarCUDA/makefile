# Makefile for building the TFTP server program

# *****************************************************
# Variables to control Makefile operation

COMPILER = g++
FLAGS = -Wall -g
GRAPH_DATA = GraphData.o
HEADERS = GraphData.h StringSplit.h
CUDA_ASTAR = CudaAStar.o
OBJECTS_SERVER = WebProxyServer.o server_commons.o
OBJECTS_CLIENT = WebProxyClient.o client_commons.o
OBJECTS_COMMON = client_server_commons.o http_10_proto.o
HEADERS_SERVER = server_commons.h client_server_commons.h http_10_proto.h
HEADERS_CLIENT = client_commons.h client_server_commons.h http_10_proto.h
HEADERS_COMMON = client_server_commons.h http_10_proto.h
# ****************************************************
# Targets needed to bring the executable up to date
all: cudaAStar

cudaAStar: $(CUDA_ASTAR) $(GRAPH_DATA)
	$(COMPILER) $(FLAGS) -o cudaAStar $(CUDA_ASTAR) $(GRAPH_DATA)

GraphData.o: $(HEADERS)
CudaAStar.o: $(HEADERS)
