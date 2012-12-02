/* 
 * File:   newmain.cpp
 * Author: arka
 *
 * Created on December 1, 2012, 10:21 PM
 */

#include <cstdlib>
#include<iostream>

#include "pqueue.h"

typedef struct{
    int nodeNum;
    double fCost;
    int parent;
    size_t pos;
}node_t;

static int
cmp_fCost(double next, double curr)
{
	return (next < curr);
}


static double
get_fCost(void *a)
{
	return (double) ((node_t *) a)->fCost;
}


static void
set_fCost(void *a, double fCost)
{
	((node_t *) a)->fCost = fCost;
}


static size_t
get_pos(void *a)
{
	return ((node_t *) a)->pos;
}


static void
set_pos(void *a, size_t pos)
{
	((node_t *) a)->pos = pos;
}



using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

        pqueue_t *pq;
	node_t   *ns;
	node_t   *n;

	ns = (node_t*)malloc(10 * sizeof(node_t));
	pq = pqueue_init(10, cmp_fCost, get_fCost, set_fCost, get_pos, set_pos);
	if (!(ns && pq)) return 1;

	ns[0]={-5,-5,1};pqueue_insert(pq, &ns[0]);
	ns[1]={-4,4,2};pqueue_insert(pq, &ns[1]);
	ns[2]={-6,6,3};pqueue_insert(pq, &ns[2]);
	//ns[3].fCost = 6; ns[3].val = -6; pqueue_insert(pq, &ns[3]);
	//ns[4].fCost = 1; ns[4].val = -1; pqueue_insert(pq, &ns[4]);

	n = (node_t*)pqueue_peek(pq);
	cout<<"peek: %e [%d]\n"<< n->fCost<<":"<<n->nodeNum<<endl;

	pqueue_change_priority(pq, 8, &ns[1]);
	pqueue_change_priority(pq, 7, &ns[2]);

   	while ((n = (node_t*)pqueue_pop(pq)))
		cout<<"pop: %e [%d]\n"<<n->fCost<<":"<<n->nodeNum<<endl;

	pqueue_free(pq);
	free(ns);

	return 0;
    //return 0;
}

