#include "GraphData.h"


void generateGraph(GraphData *graph){
    std::ifstream graphFile("USA-road-d.NY.gr");
    std::string line,buf;
    AdjacencyList* adj_list;
    int gr_lineCount = 1;
    while(graphFile.good()){
        getline(graphFile,line);
        if(line[0]=='c')
            continue;
        if(line[0]=='p'){
            std::vector<std::string> graph_info;
            split( graph_info, line, ' ' );
            int num_vertices = atoi((graph_info[2]).c_str());
            graph->vertexCount = num_vertices;
            graph->edgeCount = atoi((graph_info[3]).c_str());
            adj_list = (AdjacencyList*)malloc((num_vertices)*(sizeof (AdjacencyList)));
            memset(adj_list, 0 ,num_vertices*(sizeof (AdjacencyList)));
            continue;
        }
        
        if(gr_lineCount > graph->edgeCount)
            break;
        std::vector<std::string> edge_info;
        split( edge_info, line, ' ' );
        int from_vertex = atoi((edge_info[1]).c_str());
        int to_vertex = atoi((edge_info[2]).c_str());
        float weight = atof((edge_info[3]).c_str());
        AdjacencyList temp_adj = adj_list[from_vertex-1];
        for(int i=0;i<8;i++){
            if(temp_adj.adjList[i]==0){
                temp_adj.adjList[i]=to_vertex;
                temp_adj.weight[i]=weight;
                break;
            }
        }
         adj_list[from_vertex-1] = temp_adj;
        gr_lineCount++;
    }
    graphFile.close();
    
    graph->vertexArray = (int*) malloc(graph->vertexCount * sizeof(int));
    graph->edgeArray = (int*)malloc(graph->edgeCount * sizeof(int));
    graph->weightArray = (float*)malloc(graph->edgeCount * sizeof(float));
    int current_edge = 0;
    
    for(int i=0;i<graph->vertexCount;i++){
        AdjacencyList temp_adj = *(adj_list+i);
        graph->vertexArray[i] = current_edge;
        for(int j=0;j<8;j++){
            if(temp_adj.adjList[j]!=0){
            graph->edgeArray[current_edge] = temp_adj.adjList[j];
            graph->weightArray[current_edge] = temp_adj.weight[j];
            current_edge++;
            }else{
                break;
            }
        }
    }
    free(adj_list);
    
    std::ifstream coordFile("USA-road-d-mod.NY.co");
    int co_lineCount = 1;
    int num_vertices_co = 0;
    while(coordFile.good()){
        getline(coordFile,line);
        if(line[0]=='c')
            continue;
        if(line[0]=='p'){
            std::vector<std::string> coordinate_info;
            split( coordinate_info, line, ' ' );
            num_vertices_co = atoi((coordinate_info[4]).c_str());
            graph->latitudes = (float*)malloc(num_vertices_co*(sizeof (float)));
            graph->longitudes = (float*)malloc(num_vertices_co*(sizeof (float)));
            continue;
        }
        if(co_lineCount > num_vertices_co)
            break;
        std::vector<std::string> coordinate_info;
        split( coordinate_info, line, ' ' );
        int vertex_num = atoi((coordinate_info[1]).c_str());
        float longitude = atof((coordinate_info[2]).c_str());
        float latitude = atof((coordinate_info[3]).c_str());
        graph->latitudes[vertex_num-1] = latitude;
        graph->longitudes[vertex_num-1] = longitude;
      co_lineCount++;  
    }
    coordFile.close();

}

void getNodeDetails(GraphData *graph, int vertex){
    std::cout << "Coordinates:" << graph->latitudes[vertex-1] << ":" << graph->longitudes[vertex-1] << std::endl;
    int earliest_edge = graph->vertexArray[vertex-1];
    int final_edge = graph->vertexArray[vertex];
    for(int i=earliest_edge;i<final_edge;i++)
        std::cout << "Adjacency:" << graph->edgeArray[i] << "Weight:" << graph->weightArray[i] << std::endl;
}


bool compareNodeCost(void* a,void* b){
    return ((Node*)a)->fCost<((Node*)b)->fCost;
}

