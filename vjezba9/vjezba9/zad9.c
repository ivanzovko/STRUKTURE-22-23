#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define N (10)
#define GRESKA_ALOKACIJE (-1)
#define GRESKA_OTVARANJA (-2)
#define NULA (0)

struct stablo;
typedef struct stablo* Position;
struct list;
typedef struct list* SPosition;
typedef struct stablo {
	int broj;
	Position left;
	Position right;
}Stablo;
typedef struct list {
	int broj;
	SPosition next;
}List;

int Start(Position root);
Position AlokacijaStabla(int broj);
Position Unos(Position p, Position q);
void Ispis_I(Position p);
int BrisiStablo(Position root);
int Replace(Position root);
int UpisStabla(Position root,SPosition p);
int UpisUList(SPosition p, int broj);
int UpisUDat(SPosition p);
int BrisiList(SPosition p);
int main() {
	int pom = 0;
	Position root = NULL;
	pom = Start(root);
	if (pom != 0)
		return pom;
	return EXIT_SUCCESS;
}

int Start(Position root) {
	List head = {
		.broj = 0,
		.next = NULL
	};
	srand((unsigned)time(NULL));
	Position tmp = NULL;
	//int x[N] = { 2,5,7,8,11,1,4,2,3,7 },i;
	int x[N],i;
	for (i = 0; i < N; i++) {
		x[i] = rand() % 79 + 10;
		tmp = AlokacijaStabla(x[i]);
		if (tmp == NULL)
			return GRESKA_ALOKACIJE;
		root = Unos(root, tmp);
	}
	printf("Stablo:  ");
	Ispis_I(root);
	printf(" zamijenjeno je s: ");
	Replace(root);
	Ispis_I(root);
	printf("\n");
	if (UpisStabla(root,&head) == GRESKA_ALOKACIJE)
		return GRESKA_ALOKACIJE;	
	if (UpisUDat(&head) == EXIT_SUCCESS) {
		printf("Upisano u datoteku.\n");		
	}
	else {
		printf("Greska otvaranja dat.\n");
		return GRESKA_OTVARANJA;
	}
	BrisiList(&head);
	BrisiStablo(root);
	return EXIT_SUCCESS;
}
Position Unos(Position p, Position q) {
	if (p == NULL)
		return q;
	if (p->broj <= q->broj)
		p->left = Unos(p->left, q);
	else if (p->broj >= q->broj)
		p->right = Unos(p->right, q);							
	return p;
}
Position AlokacijaStabla(int broj) {
	Position q = NULL;
	q = (Position)malloc(sizeof(Stablo));
	if (q == NULL) {
		printf("Greska u alokaciji memorije.\n");
		return NULL;
	}
	q->left = NULL;
	q->right = NULL;
	q->broj = broj;
	return q;
}
void Ispis_I(Position p) {
	if (p == NULL)
		return;
	Ispis_I(p->left);
	printf("%d ", p->broj);
	Ispis_I(p->right);
}
int BrisiStablo(Position root) {
	if (root == NULL)
		return EXIT_SUCCESS;
	BrisiStablo(root->right);
	BrisiStablo(root->left);
	free(root);
	return EXIT_SUCCESS;
}
int Replace(Position root) {
	int L, R;
	if (root->left == NULL && root->right == NULL) {
		root->broj = 0;
		return NULA;
	}
	
	if (root->right == NULL) {
		L = root->left->broj;
		root->broj = L + Replace(root->left);
	}
	else if (root->left == NULL) {
		R = root->right->broj;
		root->broj = R + Replace(root->right);
	}
	else {
		L = root->left->broj;
		R = root->right->broj;
		root->broj = L + R + Replace(root->left) + Replace(root->right);
	}
		
	return root->broj;
}
int UpisStabla(Position root,SPosition p) {
	if (root == NULL)
		return;
	UpisStabla(root->left,p);
	if (UpisUList(p, root->broj) != EXIT_SUCCESS)
		return GRESKA_ALOKACIJE;
	UpisStabla(root->right, p);
}
int UpisUDat(SPosition p) {
	p = p->next;
	FILE* fp = NULL;
	fp = fopen("brojevi.txt", "w");
	if (fp == NULL)
		return GRESKA_OTVARANJA;
	while (p != NULL) {
		fprintf(fp, "%d  ", p->broj);
		p = p->next;
	}
	fclose(fp);
	return EXIT_SUCCESS;
}
int UpisUList(SPosition p,int broj) {
	while (p->next != NULL)
		p = p->next;
	SPosition q=NULL;
	q = (Position)malloc(sizeof(struct list));
	if (q == NULL)
		return GRESKA_ALOKACIJE;
	q->broj = broj;
	p->next = q;
	q->next = NULL;
	return EXIT_SUCCESS;
}
int BrisiList(SPosition p) {
	SPosition q=NULL;
	while (p->next != NULL) {
		q = p->next;
		p->next = q->next;
		free(q);
	}
	return EXIT_SUCCESS;
}

