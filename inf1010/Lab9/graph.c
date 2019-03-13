/* LAB 9 EDA INF1010 Árvore B e Caminhos em Grafos - 28_11_2018 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXNODES 7

int adj[MAXNODES][MAXNODES];
int visited[MAXNODES];

void ini_visited(int *v){
	for (int i = 0; i < MAXNODES; i++){
		v[i] = false;	
	} /* for */
} /* end */

void ini_adj(int v[][MAXNODES]){
	int i, j;
	for	(i = 0; i < MAXNODES; i++){
		for (j = 0; j < MAXNODES; j++){
			v[i][j] = false;	
		} /* for */
	} /* for */
} /* end */

void dfs(int node) {
	int j;
	printf("dfs(%1d)\n", node);
	visited[node] = true;
	for (j = 0; j < MAXNODES; j++) {
		if (adj[node][j] && !visited[j]) {
			dfs(j);			
		} /* if */
	} /* for */
} /* end */

void bfs(int node){
	int j;
	int aux = 0;
	int count = 0;
	int next = 0;
	int circular_list[5];
	
	while (count < MAXNODES) {
		printf("bfs(%1d)\n", node);
		visited[node] = true;
		for	(j = 0; j < MAXNODES; j++) {
			if (adj[node][j] && !visited[j]) {
				circular_list[aux] = j;
				aux = (aux + 1)%5;	
			} /* if */
		} /* for */	
		node = circular_list[next];
		next = (next + 1)%5;
		count++;		
	} /* while */	
} /* end */

int main(int argc, char* argv[]){
	/* inicializa vetor de adjacencias */	
	ini_adj(adj);
	
	/* criação do grafo */    
	adj[0][5] = true;
	adj[0][1] = true;
	adj[1][0] = true;
	adj[1][6] = true;
	adj[1][2] = true;
	adj[2][1] = true;
	adj[3][6] = true;
	adj[4][5] = true;
	adj[4][6] = true;
	adj[5][0] = true;
	adj[5][4] = true;	
	adj[6][1] = true;
	adj[6][3] = true;
	adj[6][4] = true;    
	
	/* inicializa vetor de vertices visitados e chama dfs*/	
	puts("percurso em grafo em profundidade");	
	ini_visited(visited);
	dfs(0);

	/* inicializa vetor de vertices visitados e chama bfs*/
	puts("percurso em grafo em largura");	
	ini_visited(visited);
	bfs(0);

	return 0;
}
