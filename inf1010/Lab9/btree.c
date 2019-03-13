/* LAB 9 EDA INF1010 Árvore B e Caminhos em Grafos - 28_11_2018 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEGREE 2

struct btreenode{
    int keys[DEGREE];           // An array of keys
    int d;               // Minimum degree (defines the range of numbers of keys)
    struct btreenode **nodes;   // An array of child pointers
    int n;               // Current number of keys
    bool leaf;           // Is true when node is leaf. Otherwise false.
};

typedef struct btreenode BTreeNode;

void visualizeBTree(BTreeNode *root) {
    int i = 0;
    
    if (root->leaf == false) {
        while (i < (root->n + 1)) {
            visualizeBTree(root->nodes[i]);
            if (i < root->n) {
                printf("%d \n", root->keys[i]);
            } /* if */
            i++;
        } /* while */
    } /* if */
    else { //root->leaf == true
        for (; i< root->n; i++) {
            printf("%d \n", root->keys[i]);
        } /* for */
    } /* else */
} /* end */

int searchBTree(BTreeNode *root, int value){
	int i = 0;

	while (i < root->n && value < root->keys[i]) {
		i++;
	} /* while */
	if (i < root->n && root->keys[i] == value) {
		return 1;
	} /* if */
	if (root->leaf == true){
		return 0;
	} /* if */
	
	return searchBTree(root->nodes[i], value);
} /* end */

void betweenBTree(BTreeNode *root, int inferiorLim, int upperLim){
	int i = 0;
	
	if (root->leaf == false) {
		while (i < (root->n + 1)) {
			while (i < root->n && root->keys[i] <= inferiorLim ) {
				i++;
			} /* while */
			betweenBTree(root->nodes[i], inferiorLim, upperLim);
			if (i < root->n){
				if (root->keys[i] < upperLim) {
					printf("%d\n", root->keys[i]);
		    	} /* if */
		    	else { // root->keys[i] >= value
		        	break;
		    	} /* else */
		    } /* if */
		    i++;
		} /* while */
	} /* if */
	else { // root->leaf == true
		for (; i < root->n; i++) {
			if (root->keys[i] > inferiorLim && root->keys[i] < upperLim) {
				printf("%d\n", root->keys[i]);
			} /* if */
			if (root->keys[i] >= upperLim) {
				break;
			} /* if */
		} /* for */
	} /* else */
} /* end */

int main(int argc, char* argv[]){
    BTreeNode *root;
    
    root = NULL;
    root = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->keys[0] = 200;
    root->keys[1] = 600;
    root->d = DEGREE;
    root->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->n = 2;
    root->leaf = false;
    root->nodes[0] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[0]->keys[0] = 100;
    root->nodes[0]->keys[1] = 150;
    root->nodes[0]->d = DEGREE;
    root->nodes[0]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[0]->n = 2;
    root->nodes[0]->leaf = false;
    root->nodes[1] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[1]->keys[0] = 300;
    root->nodes[1]->keys[1] = 400;
    root->nodes[1]->d = DEGREE;
    root->nodes[1]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[1]->n = 2;
    root->nodes[1]->leaf = false;
    root->nodes[2] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[2]->keys[0] = 700;
    root->nodes[2]->keys[1] = 740;
    root->nodes[2]->d = DEGREE;
    root->nodes[2]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[2]->n = 2;
    root->nodes[2]->leaf = false;
    root->nodes[0]->nodes[0] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[0]->nodes[0]->keys[0] = 50;
    root->nodes[0]->nodes[0]->keys[1] = 70;
    root->nodes[0]->nodes[0]->d = DEGREE;
    root->nodes[0]->nodes[0]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[0]->nodes[0]->n = 2;
    root->nodes[0]->nodes[0]->leaf = true;
    root->nodes[0]->nodes[1] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[0]->nodes[1]->keys[0] = 120;
    root->nodes[0]->nodes[1]->keys[1] = 140;
    root->nodes[0]->nodes[1]->d = DEGREE;
    root->nodes[0]->nodes[1]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[0]->nodes[1]->n = 2;
    root->nodes[0]->nodes[1]->leaf = true;
    root->nodes[0]->nodes[2] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[0]->nodes[2]->keys[0] = 160;
    root->nodes[0]->nodes[2]->keys[1] = 180;
    root->nodes[0]->nodes[2]->d = DEGREE;
    root->nodes[0]->nodes[2]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[0]->nodes[2]->n = 2;
    root->nodes[0]->nodes[2]->leaf = true;
    root->nodes[1]->nodes[0] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[1]->nodes[0]->keys[0] = 220;
    root->nodes[1]->nodes[0]->keys[1] = 280;
    root->nodes[1]->nodes[0]->d = DEGREE;
    root->nodes[1]->nodes[0]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[1]->nodes[0]->n = 2;
    root->nodes[1]->nodes[0]->leaf = true;
    root->nodes[1]->nodes[1] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[1]->nodes[1]->keys[0] = 320;
    root->nodes[1]->nodes[1]->keys[1] = 380;
    root->nodes[1]->nodes[1]->d = DEGREE;
    root->nodes[1]->nodes[1]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[1]->nodes[1]->n = 2;
    root->nodes[1]->nodes[1]->leaf = true;
    root->nodes[1]->nodes[2] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[1]->nodes[2]->keys[0] = 500;
    root->nodes[1]->nodes[2]->keys[1] = 550;
    root->nodes[1]->nodes[2]->d = DEGREE;
    root->nodes[1]->nodes[2]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[1]->nodes[2]->n = 2;
    root->nodes[1]->nodes[2]->leaf = true;
    root->nodes[2]->nodes[0] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[2]->nodes[0]->keys[0] = 620;
    root->nodes[2]->nodes[0]->keys[1] = 630;
    root->nodes[2]->nodes[0]->d = DEGREE;
    root->nodes[2]->nodes[0]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[2]->nodes[0]->n = 2;
    root->nodes[2]->nodes[0]->leaf = true;
    root->nodes[2]->nodes[1] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[2]->nodes[1]->keys[0] = 710;
    root->nodes[2]->nodes[1]->keys[1] = 720;
    root->nodes[2]->nodes[1]->d = DEGREE;
    root->nodes[2]->nodes[1]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[2]->nodes[1]->n = 2;
    root->nodes[2]->nodes[1]->leaf = true;
    root->nodes[2]->nodes[2] = (BTreeNode*)malloc(sizeof(BTreeNode));
    root->nodes[2]->nodes[2]->keys[0] = 800;
    root->nodes[2]->nodes[2]->keys[1] = 820;
    root->nodes[2]->nodes[2]->d = DEGREE;
    root->nodes[2]->nodes[2]->nodes = (BTreeNode**)malloc((DEGREE + 1)*(sizeof(BTreeNode*)));
    root->nodes[2]->nodes[2]->n = 2;
    root->nodes[2]->nodes[2]->leaf = true;
    
  	switch (atoi(argv[1])) {
 		case 1: {
 			visualizeBTree(root);
 			break;
 		}
    	case 2: {
    		betweenBTree(root, 699, 751);
    		break;
    	}
    	case 3: {
    		printf("%d\n", searchBTree(root,atoi(argv[2])));
    		break;
    	}
    } /* switch */
    
    return 0;
} /* end main */
