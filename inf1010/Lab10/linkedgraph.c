/* LAB 10 EDA INF1010 Grafos - 05_12_2018  */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAXNODES 7

struct graph {
	struct arc *arcs;		// A pointer to the list of adjacencies
};

struct arc {
	int weight;				// The weight of the arc
	int dest;				// Especific the right graph
	struct arc *next;		// A pointer to the linked graph
};

typedef struct graph Graph;

struct MinHeapNode { 
    int  v; 
    int dist; 
}; 

struct MinHeap { 
    int size;      // Number of heap nodes present currently 
    int capacity;  // Capacity of min heap 
    int *pos;     // This is needed for decreaseKey() 
    struct MinHeapNode **array; 
};

struct MinHeapNode* newMinHeapNode(int v, int dist) { 
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode)); 
    minHeapNode->v = v;
    minHeapNode->dist = dist; 
    return minHeapNode; 
} 

struct MinHeap* createMinHeap(int capacity) { 
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap)); 
    minHeap->pos = (int *)malloc(capacity * sizeof(int)); 
    minHeap->size = 0; 
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*)); 
    return minHeap; 
} 

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) { 
    struct MinHeapNode* t = *a; 
    *a = *b; 
    *b = t; 
}

void minHeapify(struct MinHeap* minHeap, int idx) { 
    int smallest, left, right; 
    smallest = idx; 
    left = 2 * idx + 1; 
    right = 2 * idx + 2; 
  
    if (left < minHeap->size && 
        minHeap->array[left]->dist < minHeap->array[smallest]->dist ) 
      smallest = left; 
  
    if (right < minHeap->size && 
        minHeap->array[right]->dist < minHeap->array[smallest]->dist ) 
      smallest = right; 
  
    if (smallest != idx) 
    { 
        // The nodes to be swapped in min heap 
        struct MinHeapNode *smallestNode = minHeap->array[smallest]; 
        struct MinHeapNode *idxNode = minHeap->array[idx]; 
  
        // Swap positions 
        minHeap->pos[smallestNode->v] = idx; 
        minHeap->pos[idxNode->v] = smallest; 
  
        // Swap nodes 
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]); 
  
        minHeapify(minHeap, smallest); 
    } 
}

int isEmpty(struct MinHeap* minHeap) { 
    return minHeap->size == 0; 
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap) { 
    if (isEmpty(minHeap)) 
        return NULL; 
  
    // Store the root node 
    struct MinHeapNode* root = minHeap->array[0]; 
  
    // Replace root node with last node 
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1]; 
    minHeap->array[0] = lastNode; 
  
    // Update position of last node 
    minHeap->pos[root->v] = minHeap->size-1; 
    minHeap->pos[lastNode->v] = 0; 
  
    // Reduce heap size and heapify root 
    --minHeap->size; 
    minHeapify(minHeap, 0); 
  
    return root; 
} 

void decreaseKey(struct MinHeap* minHeap, int v, int dist) { 
    // Get the index of v in  heap array 
    int i = minHeap->pos[v]; 
  
    // Get the node and update its dist value 
    minHeap->array[i]->dist = dist; 
  
    // Travel up while the complete tree is not hepified. 
    // This is a O(Logn) loop 
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) 
    { 
        // Swap this node with its parent 
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2; 
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i; 
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]); 
  
        // move to parent index 
        i = (i - 1) / 2; 
    } 
} 
  
bool isInMinHeap(struct MinHeap *minHeap, int v) { 
   if (minHeap->pos[v] < minHeap->size) 
     return true; 
   return false; 
}

void printArr(int dist[], int n) { 
    printf("Vertex   Distance from Source\n"); 
    for (int i = 0; i < n; ++i) 
        printf("%d \t\t %d\n", i, dist[i]); 
}

void dijkstra(Graph* graph, int src) 
{ 
    int V = MAXNODES; // Get the number of vertices in graph 
    int dist[V];      // dist values used to pick minimum weight edge in cut 
  
    // minHeap represents set E 
    struct MinHeap* minHeap = createMinHeap(V); 
  
    // Initialize min heap with all vertices. dist value of all vertices  
    for (int v = 0; v < V; ++v) 
    { 
        dist[v] = INT_MAX; 
        minHeap->array[v] = newMinHeapNode(v, dist[v]); 
        minHeap->pos[v] = v; 
    } 
  
    // Make dist value of src vertex as 0 so that it is extracted first 
    minHeap->array[src] = newMinHeapNode(src, dist[src]); 
    minHeap->pos[src]   = src; 
    dist[src] = 0; 
    decreaseKey(minHeap, src, dist[src]); 
  
    // Initially size of min heap is equal to V 
    minHeap->size = V; 
  
    // In the followin loop, min heap contains all nodes 
    // whose shortest distance is not yet finalized. 
    while (!isEmpty(minHeap)) 
    { 
        // Extract the vertex with minimum distance value 
        struct MinHeapNode* minHeapNode = extractMin(minHeap); 
        int u = minHeapNode->v; // Store the extracted vertex number 
  
        // Traverse through all adjacent vertices of u (the extracted 
        // vertex) and update their distance values 
        struct arc* pCrawl = graph[u].arcs; 
        while (pCrawl != NULL) 
        { 
            int v = pCrawl->dest; 
  
            // If shortest distance to v is not finalized yet, and distance to v 
            // through u is less than its previously calculated distance 
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&  
                                          pCrawl->weight + dist[u] < dist[v]) 
            { 
                dist[v] = dist[u] + pCrawl->weight; 
  
                // update distance value in min heap also 
                decreaseKey(minHeap, v, dist[v]); 
            } 
            pCrawl = pCrawl->next; 
        } 
    } 
  
    // print the calculated shortest distances 
    printArr(dist, V); 
}

bool visited[MAXNODES];

void ini_visited() {
	for (int i = 0; i < MAXNODES; i++) {
		visited[i] = false;	
	} /* for */
} /* end */

void insertEdge(Graph *g, int vertex1, int vertex2, int weight) {	
	struct arc *new = (struct arc*)malloc(sizeof(struct arc));
	if ( new == NULL) {
		puts("Não há memória suficiente");
		exit(1);
	} /* if */
	if (g[vertex1].arcs == NULL) {
		new->weight = weight;
		new->dest = vertex2;
		new->next = NULL;
		g[vertex1].arcs = new;
	} /* if */
	else { //g[vertex1]->arcs != NULL
		new->weight = weight;
		new->dest = vertex2;
		new->next = g[vertex1].arcs;
		g[vertex1].arcs = new;
	} /* else */
} /* end */

static void check(Graph *g, int v) {
	struct arc *aux = g[v].arcs;
	visited[v] = true;
	for (; aux != NULL; aux = aux->next){
		if (!visited[aux->dest]){
			check(g, aux->dest);
		} /* if */
	} /* for */
} /* end */

static int all_visited(void) {
	for (int i = 0; i < MAXNODES; i++) {
		if (visited[i] == false) {
			return 0;
		} /* if */
	} /* for */
	return 1;
}

int connected(Graph *g) {
	ini_visited();
	check(g, 0);
	return all_visited();
} /* end */

int main(void) {
	Graph graphs[MAXNODES];
	
	/* inicialização do grafo */
	graphs[0].arcs = NULL;
	graphs[1].arcs = NULL;
	graphs[2].arcs = NULL;
	graphs[3].arcs = NULL;
	graphs[4].arcs = NULL;
	graphs[5].arcs = NULL;
	graphs[6].arcs = NULL;
	
	
	/* criação do grafo lab10.1 */
	insertEdge(graphs, 0, 1, 28);
	insertEdge(graphs, 0, 5, 10);
	insertEdge(graphs, 1, 0, 28);
	insertEdge(graphs, 1, 2, 16);
	insertEdge(graphs, 1, 6, 14);
	insertEdge(graphs, 2, 1, 16);
	insertEdge(graphs, 3, 6, 18);
	insertEdge(graphs, 4, 5, 24);
	insertEdge(graphs, 4, 6, 25);
	insertEdge(graphs, 5, 0, 10);
	insertEdge(graphs, 5, 4, 25);
	insertEdge(graphs, 6, 1, 14);
	insertEdge(graphs, 6, 3, 18);
	insertEdge(graphs, 6, 4, 24);
	
	/* verifica se é conexo */
	printf("%d\n",connected(graphs));
	
	/* criação do grafo lab10.2 */
	insertEdge(graphs, 2, 3, 15);
	insertEdge(graphs, 3, 2, 15);
	
	/* calcula as distâncias entre o nó origem e os demais possíveis destinos */
	dijkstra(graphs, 0);
	
	
	return 0;
} /* end main */


