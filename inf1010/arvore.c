#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#define MAX(a,b)  ((a) >= (b) ? (a) :  (b))
#define N 128

struct arv {
   int info;
   struct arv* esq;
   struct arv* dir;
};

struct heap {
   int max; /* tamanho maximo do heap */
   int pos; /* proxima posicao disponivel no vetor */
   int* prioridade; /* vetor das prioridades */
};

int v[N];

typedef struct arv Arv;
typedef struct heap Heap;

Arv* inicializa(void);								//inicializa a árvore
Arv* cria(int v);									//cria o nó
int busca(Arv* raiz, int key);						//busca chave na árvore
int vazia(Arv* raiz);								//verifica se a árvore está vazia
Arv* insere(Arv* pRaiz, int key);					//insere elemento na árvore binária
Arv* AbbMenor(int ini, int fim, Arv *pRaiz);		//insere elementos do vetor global a fim de criar uma árvore de menor altura possível
void imprimeSim(Arv* praiz);						//imprime em ordem simétrica
void imprimePre(Arv* praiz);						//imprime em pós-ordem
int ordemSimIgual(Arv* praiz1, Arv* praiz2);		//verifica se as árvores têm a mesma ordem (simétrica)
int ordemPreIgual(Arv* praiz1, Arv* praiz2);		//verifica se as árvores têm a mesma ordem (pré-ordem)
int igual(Arv* praiz1, Arv* praiz2);				//verifica se as árvore são iguais em chaves e estrutura
int altura(Arv* praiz);								//verifica a altura da árvore
int ehBalanceada(Arv* praiz);						//verifica se a árvore satisfaz as condições de árvore balanceada
Heap* heap_cria(int max);
void heap_insere(Heap* heap, int prioridade);
int heap_remove(Heap* heap);
static void troca(int a, int b, int* v);
static void corrige_acima(Heap* heap, int pos);
static void corrige_abaixo(Heap* heap);
void iniciar_aleatorio(void);						//inicializa os números aleatórios
static int compara(const void* x, const void* y);	//função auxilar a qsort
int binarySearch(int n, int* v, int size);			//busca binária em vetor ordenado

Arv* inicializa(void){
   return NULL;
}

Arv* cria(int key){
   Arv* p = (Arv*)malloc(sizeof(Arv));
   if (p == NULL){
	   printf("Não há memória na máquina");
	   exit(1);
   }
   p->info = key;
   p->esq = NULL;
   p->dir = NULL;
   return p;
}

Arv* insere(Arv *pRaiz,int key){
    if(pRaiz == NULL){
        pRaiz = cria(key);
        return pRaiz;
    }
    if(pRaiz->info > key){
        pRaiz->esq = insere(pRaiz->esq, key);
        return pRaiz;
    }
    else if(pRaiz->info < key){
        pRaiz->dir = insere(pRaiz->dir, key);
    }
    return pRaiz;
}

Arv* AbbMenor(int ini, int fim, Arv *pRaiz){
    int meio = (ini+fim)/2;
    if(ini >= fim){
        pRaiz = insere(pRaiz, v[meio]);
        return pRaiz;
    }
    pRaiz = insere(pRaiz, v[meio]);
    pRaiz = AbbMenor(ini, meio-1, pRaiz);
    pRaiz = AbbMenor(meio+1, fim, pRaiz);
    return pRaiz;
}

int vazia(Arv* raiz){
	return raiz == NULL;
}

int busca(Arv* raiz, int key){
	if(vazia(raiz)) return 0;
	return raiz->info == key || busca(raiz->esq, key) || busca(raiz->dir, key);
}

void imprimeSim(Arv* praiz){
	if(!vazia(praiz)){
		imprimeSim(praiz->esq);
		printf("%d ,", praiz->info);
		imprimeSim(praiz->dir);
	}
}

void imprimePre(Arv* praiz){
	if(!vazia(praiz)){
		printf("%d ,",praiz->info);
		imprimePre(praiz->esq);
		imprimePre(praiz->dir);
	}
}

int ordemSimIgual(Arv* praiz1, Arv* praiz2){
	if (vazia(praiz1) && vazia(praiz2)) return 1;
	if (vazia(praiz1) || vazia(praiz2)) return 0;
	return ordemSimIgual(praiz1->esq, praiz2->esq) && praiz1->info == praiz2->info && ordemSimIgual(praiz1->dir, praiz2->dir);
}

int ordemPosIgual(Arv* praiz1, Arv* praiz2){
	if (vazia(praiz1) && vazia(praiz2)) return 1;
	if (vazia(praiz1) || vazia(praiz2)) return 0;
	return praiz1->info == praiz2->info && ordemSimIgual(praiz1->esq, praiz2->esq) && ordemSimIgual(praiz1->dir, praiz2->dir);
}

int igual(Arv* praiz1, Arv* praiz2){
	if(vazia(praiz1) && vazia(praiz2))return 1;
	if(ordemPosIgual(praiz1, praiz2) && ordemSimIgual(praiz1, praiz2)) return 1;
	return 0;
}

int verificaAbb(Arv* pRaiz, int min, int max) { 
  	if (pRaiz==NULL) return 1;
       	if (pRaiz->info < min || pRaiz->info > max) return 0; 
 	return verificaAbb(pRaiz->esq, min, pRaiz->info-1) && verificaAbb(pRaiz->dir, pRaiz->info+1, max); 
} 

int ehAbb(Arv* pRaiz){ 
  return(verificaAbb(pRaiz, 0, INT_MAX)); 
}

int altura(Arv* praiz){ 
   if(praiz == NULL) return 0; 
   return 1 + MAX(altura(praiz->esq), altura(praiz->dir)); 
}  

int ehBalanceada(Arv* praiz) { 
   int eh; 
   int dh;

   if(praiz == NULL) return 1;  
  
   eh = altura(praiz->esq);
   dh = altura(praiz->dir);
  
   if( abs(eh-dh) <= 1 && ehBalanceada(praiz->esq) && ehBalanceada(praiz->dir)) return 1; 
   return 0; 
} 

Heap* heap_cria(int max){
   Heap* heap=(Heap*)malloc(sizeof(Heap));
   heap->max=max;
   heap->pos=0;
   heap->prioridade=(int *)malloc(max*sizeof(int));
return heap;
}

void heap_insere(Heap* heap, int prioridade){
   if ( heap->pos < heap->max ){
      heap->prioridade[heap->pos] = prioridade;
      corrige_acima(heap,heap->pos);
      heap->pos++;
   }
   else
      printf("Heap CHEIO!\n");
}

int heap_remove(Heap* heap){
	if (heap->pos>0) {
		int topo=heap->prioridade[0];
		heap->prioridade[0]=heap->prioridade[heap->pos-1];
		heap->pos--;
		corrige_abaixo(heap);
		return topo;
	}
	else {
		printf("Heap VAZIO!");
		return -1;
	}
}

static void troca(int a, int b, int* v) {
   int f = v[a];
   v[a] = v[b];
   v[b] = f;
}

static void corrige_acima(Heap* heap, int pos) {
	while (pos > 0){
  		int pai = (pos-1)/2;
		if (heap->prioridade[pai] < heap->prioridade[pos])
			troca(pos,pai,heap->prioridade);
		else
			break;
		pos=pai;
	}
}

static void corrige_abaixo(Heap* heap){
    int pai=0;
	while (2*pai+1 < heap->pos){
		int filho_esq=2*pai+1;
		int filho_dir=2*pai+2;
		int filho;
		if (filho_dir >= heap->pos) filho_dir=filho_esq;
		if (heap->prioridade[filho_esq]>heap->prioridade[filho_dir])
			filho=filho_esq;
		else
			filho=filho_dir;
		if (heap->prioridade[pai]<heap->prioridade[filho])
			troca(pai,filho,heap->prioridade);
		else
			break;
		pai=filho;
	}
}

void iniciar_aleatorio(void){
	srand((unsigned int)time(NULL));
	rand();
	rand();
	rand();
}

static int compara(const void* x, const void* y){
	int r = *(int*) x;
	int q = *(int*) y;

	return r-q;
}

int binarySearch(int n, int* v, int size){
	int i = 0, f = size - 1, m = size/2;

	while (i<=f){
		m = (i+f)/2;
		if (n == v[m]) return 1;
		else if (n > v[m]) i = m + 1;
		else f = m - 1;
	}
	return 0;
}

void insereRandInt(int* v){
	int i, randInt;
	
	for(i=0;i<N;){
		randInt = rand();
		if (binarySearch(randInt, v, i))
			continue;
		v[i] = randInt;
		qsort(v,i+1,sizeof(int),compara);
		i++;
	}
}

int main(){
	return 0;
}
