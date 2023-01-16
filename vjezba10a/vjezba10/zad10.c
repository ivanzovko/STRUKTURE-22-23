#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include "header.h"


int main() {
	Drzave Head = {
		.ime_drzave = {0},
		.grad=NULL,
		.next = NULL
	};
	Greske status = OK;	
	
	status=CitajDatoteku(&Head);
	if (status == OK) {
		Ispis(&Head);
		//SEARCH
		TraziGlavna(&Head);
	}

	//BRISI SVE
	BrisiDrzave(&Head);
	return status;
}

int CitajDatoteku(PositionDrz lista_drzava) {
	char ime_dat[MAX_FILENAME] = { 0 }; char redak_drzave[MAX_RED] = { 0 };
	FILE* fp = NULL;
	int status = OK;
	printf("\tUnesite ime glavne datoteke:\t");
	gets(ime_dat);		
	fp = fopen(ime_dat, "r");
	if (fp == NULL) {
		printf("Postovani, datoteka %s se nije ispravno otvorila!\n", ime_dat);
		return ERROR_OPENNING_FILE;
	}	
	while (!feof(fp)) {	
		fgets(redak_drzave, MAX_RED, fp);	
		status=UpisDrzave(lista_drzava, redak_drzave);
		if (status != OK)
			return status;
		}

	fclose(fp);
	return OK;
}
int UpisDrzave(PositionDrz home_drzave, char* redak_drzave) {
	char ime_drz[MAX_IME_DRZAVE] = { 0 };char ime_dat[MAX_FILENAME] = { 0 };
	sscanf(redak_drzave, "%s %s", ime_drz, ime_dat);
	PositionDrz q = NULL;
	
	q = (PositionDrz)malloc(sizeof(struct Drzave));
	if (q == NULL) {
		printf("Greska u alokaciji memorije liste.\n");
		return ERROR_ALLOCATING_MEMORY;
	}
	strcpy(q->ime_drzave, ime_drz);
	q->grad=CitajGradove(ime_dat);	
	if ((int)q->grad == ERROR_OPENNING_FILE)
		return ERROR_OPENNING_FILE;
	else if ((int)q->grad == ERROR_ALLOCATING_MEMORY)
		return ERROR_ALLOCATING_MEMORY;
	UnesiSortDrz(home_drzave, q);
	return OK;
}
PositionGrad CitajGradove(char* ime_dat) {
	char redak_grad[MAX_LINE_GRADA]={0}; char ime_grada[MAX_IME_GRADA] = {0};
	int broj_stanovnika = 0;
	FILE* fp = NULL;
	PositionGrad root=NULL;	PositionGrad tmp = NULL;

	fp = fopen(ime_dat, "r");
	if (fp == NULL) {
		printf("Postovani, datoteka %s se nije ispravno otvorila!\n", ime_dat);
		return ERROR_OPENNING_FILE;
	}
	while (!feof(fp)) {
		fgets(redak_grad, MAX_LINE_GRADA, fp);
		sscanf(redak_grad, "%s %d", ime_grada, &broj_stanovnika);
		tmp = AlokacijaStabla(ime_grada, broj_stanovnika);
		if (tmp == NULL)
			return ERROR_ALLOCATING_MEMORY;
		root = Unos(root, tmp);
		
	}
	fclose(fp);
	return root;
}
PositionGrad AlokacijaStabla(char* ime_grada,int br_stanovnika) {
	PositionGrad q = NULL;
	q = (PositionGrad)malloc(sizeof(Gradovi));
	if (q == NULL) {
		printf("Greska u alokaciji memorije stabla.\n");
		return NULL;
	}
	q->left = NULL;
	q->right = NULL;
	q->broj_stanovnika = br_stanovnika;
	strcpy(q->ime_grada,ime_grada);
	return q;
}
PositionGrad Unos(PositionGrad p, PositionGrad q) {
	if (p == NULL)
		return q;
	if (p->broj_stanovnika > q->broj_stanovnika)
		p->left = Unos(p->left, q);
	else if (p->broj_stanovnika < q->broj_stanovnika)
		p->right = Unos(p->right, q);
	else {
		if(strcmp(p->ime_grada,q->ime_grada)<=0)
			p->left = Unos(p->left, q);
		else if (strcmp(p->ime_grada, q->ime_grada) > 0)
			p->right = Unos(p->right, q);

	}
	return p;
}
int UnesiSortDrz(PositionDrz lista_drzava, PositionDrz q) {
	while (lista_drzava->next != NULL && strcmp(lista_drzava->next->ime_drzave, q->ime_drzave) < 0) {
		lista_drzava = lista_drzava->next;
	}
	q->next = lista_drzava->next;
	lista_drzava->next = q;
	return EXIT_SUCCESS;
}
void Ispis_I(PositionGrad p) {
	if (p == NULL) {
		return;
	}
	Ispis_I(p->right);
	printf("\t%s\t%d\n", p->ime_grada,p->broj_stanovnika);
	Ispis_I(p->left);
}
int Ispis(PositionDrz p) {
	p = p->next;
	while (p != NULL) {
		printf("\n\t********%s********\n\n", p->ime_drzave);
		Ispis_I(p->grad);
		p = p->next;
	}
	
	return EXIT_SUCCESS;
}
PositionDrz PronadiDrzavu(PositionDrz p,char *ime_drzave) {
	p = p->next;
	while (p != NULL && strcmp(p->ime_drzave, ime_drzave))
		p = p->next;
	return p;
}
int TraziGradove(PositionGrad root, int br_stanovnika){
	int br = 0;
	if (root == NULL)
		return NULL;
	if (root->right != NULL && root->broj_stanovnika > br_stanovnika)
		br+=TraziGradove(root->right, br_stanovnika);
	if (root->broj_stanovnika > br_stanovnika) {
		printf("\t%s\t%d\n", root->ime_grada, root->broj_stanovnika);
		br++;
	}
	if (root->left != NULL && root->broj_stanovnika >= br_stanovnika)
		br+=TraziGradove(root->left, br_stanovnika);
	if (root->right != NULL && root->broj_stanovnika<=br_stanovnika) 
		br+=TraziGradove(root->right, br_stanovnika);
	
	return br;
}
int TraziGlavna(PositionDrz drzava) {
	char ime_drzave[MAX_IME_DRZAVE] = { 0 };
	char unos[MAX_RED] = { 0 };
	int br_stan = 0;
	PositionDrz TrazenaDrzava = NULL;

	do {
		strcpy(ime_drzave, "");
		br_stan = 0;
		printf("\n\tUnesite drzavu i broj stanovnika:\t");
		fgets(unos, MAX_RED, stdin);
		sscanf(unos, "%s %d", ime_drzave, &br_stan);
		if (strcmp(ime_drzave, "exit")) {
			if (br_stan < 0) {
				printf("\nPogresan unos, stanovnici ne mogu biti negativni.\n");
				continue;
			}
			TrazenaDrzava = PronadiDrzavu(drzava, ime_drzave);
			if (TrazenaDrzava == NULL) {
				printf("\tTrazena drzava nije pronadena!\n");
				continue;
			}
			if (TraziGradove(TrazenaDrzava->grad, br_stan) == 0) {
				printf("\tTrazeni gradovi nisu pronadeni!\n");
			}
		}
	} while (strcmp(ime_drzave, "exit"));
	return EXIT_SUCCESS;
}
int BrisiStablo(PositionGrad root) {
	if (root == NULL)
		return EXIT_SUCCESS;
	BrisiStablo(root->right);
	BrisiStablo(root->left);
	free(root);
	return EXIT_SUCCESS;
}
int BrisiDrzave(PositionDrz p) {
	PositionDrz q;
	while (p->next != NULL) {
		q = p->next;
		p->next = q->next;
		BrisiStablo(q->grad);
		free(q);
	}

}