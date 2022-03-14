#include <stdio.h>
#include <stdlib.h>

struct Node;

typedef struct Node* Tree;

struct Node {
  int value;
  Tree left, right;
};

// Jeśli w drzewie wskazywanym przez treePtr nie występuje wartość x,
// to zostaje wstawiona.
void insert(Tree *treePtr, int x){
	printf("wgl sie zaczelo");
	Tree t = *treePtr;
	if (t == NULL){
		printf("dupa3 \n");
		Tree r = malloc(sizeof(struct Node));
		r -> value = x;
		r -> right = r -> left = NULL;
		*treePtr = r; 	
	}
	else if (t->value > x){
		printf("dupa1 \n");
		insert(&(t -> right), x);
	}
	else if (t->value < x){
		printf("dupa2 \n");
		insert(&(t -> left), x);
	}
	else {
		printf("dupa \n");
	}
};

// Wypisuje na wyjście wartości przechowywane w drzewie t w porządku rosnącym.
void printAll(Tree t){
	if (t != NULL) {
		printAll(t -> left);
		printf("%d\n",t->value);
		printAll(t -> right);
	};
};

// Usuwa z pamięci drzewo t.
void removeAll(Tree t){
	if (t != NULL) {
		removeAll(t -> left);
		removeAll(t -> right);
		free(t);
	};
};

int main(){
	printf("dziala");
	Tree t =  NULL;
	int n;
	printf("dziala");
	while(scanf("%d",&n)==1){
		printf("tutaj");
		insert(&t,n);
	}
	printf("dziala");
	printAll(t);
	removeAll(t);
	return 0;
}
