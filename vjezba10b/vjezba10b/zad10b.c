#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include "header.h"


int main() {
	PositionDrz root = NULL;
	int status=OK;
	
	root = CitajDatoteku(root,&status);
	if (root == NULL) {
		root = BrisiDrzave(root);
		return status;
	}
	Ispis_I(root);
	TraziGlavna(root);

	root=BrisiDrzave(root);
	return status;
}

PositionDrz CitajDatoteku(PositionDrz stablo_drzava,int* status) {
	char ime_dat[MAX_FILENAME] = { 0 }; char redak_drzave[MAX_RED] = { 0 };
	FILE* fp = NULL;
	PositionDrz q = NULL;
	char ime_drz[MAX_IME_DRZAVE] = { 0 }; 

	printf("\tUnesite ime glavne datoteke:\t");
	gets(ime_dat);
	fp = fopen(ime_dat, "r");
	if (fp == NULL) {
		printf("Postovani, datoteka %s se nije ispravno otvorila!\n", ime_dat);
		*status = ERROR_OPENNING_FILE;
		return NULL;
	}
	while (!feof(fp)) {
		fgets(redak_drzave, MAX_RED, fp);
		sscanf(redak_drzave, "%s %s", ime_drz, ime_dat);
		q=Kreiranje(ime_drz);
		if (q == NULL) {
			*status = ERROR_ALLOCATING_MEMORY;
			return NULL;
		}
		*status=CitajGradove(q,ime_dat, status);
		if (*status != OK)
			return NULL;
		stablo_drzava = UnesiDrz(stablo_drzava, q);		
	}
	fclose(fp);		
	return stablo_drzava;
}
PositionDrz Kreiranje(char* ime_drz) {	
	PositionDrz q = NULL;
	q = AlokacijaStabla(ime_drz);
	if (q == NULL) 
		return NULL;	
	return q;
}
PositionDrz AlokacijaStabla(char* ime_drz) {
	PositionDrz q = NULL;
	q = (PositionDrz)malloc(sizeof(Drzave));
	if (q == NULL) {
		printf("Greska u alokaciji memorije stabla(Drazave).\n");
		return NULL;
	}
	q->left = NULL;
	q->right = NULL;
	strcpy(q->ime_drzave, ime_drz);
	InicGrad(&q->grad);
	return q;
}
int InicGrad(PositionGrad p) {
	p->broj_stanovnika = 0;
	memset(p->ime_grada, 0, MAX_IME_GRADA);
	p->next = NULL;
	return EXIT_SUCCESS;
}
int CitajGradove(PositionDrz q,char* ime_dat) {	
	char redak_grad[MAX_LINE_GRADA] = { 0 }; char ime_grada[MAX_IME_GRADA] = { 0 };
	int broj_stanovnika = 0;
	FILE* fp = NULL;
	PositionGrad tmp = NULL;

	fp = fopen(ime_dat, "r");
	if (fp == NULL) {
		printf("Postovani, datoteka %s se nije ispravno otvorila!\n", ime_dat);
		return ERROR_OPENNING_FILE;
	}
	while (!feof(fp)) {
		tmp = NULL;
		fgets(redak_grad, MAX_LINE_GRADA, fp);
		sscanf(redak_grad, "%s %d", ime_grada, &broj_stanovnika);
		tmp = AlokacijaGrada(ime_grada,broj_stanovnika);
		if (tmp == NULL) 
			return ERROR_ALLOCATING_MEMORY;
		 UnosGradSort(&q->grad, tmp);
	}
	fclose(fp);
	return OK;
}
PositionGrad AlokacijaGrada(char* ime_grada, int br_stan) {
	PositionGrad p = NULL;
	p = (PositionGrad)malloc(sizeof(Gradovi));
	if (p == NULL){
		printf("Greska u alokaciji memorije liste(grada).\n");
		return NULL;
	}
	p->broj_stanovnika = br_stan;
	strcpy(p->ime_grada,ime_grada);
	p->next = NULL;
	return p;
}
int UnosGradSort(PositionGrad grad, PositionGrad q) {
	while (grad->next != NULL && GradCompare(grad->next, q)> 0) {
		grad = grad->next;
	}
	q->next = grad->next;
	grad->next = q;
	return EXIT_SUCCESS;
}
int GradCompare(PositionGrad p, PositionGrad q) {
	int result = 0;
	result = p->broj_stanovnika - q->broj_stanovnika;
	if (result == 0)
		result = !strcmp(p->ime_grada,q->ime_grada);
	return result;
}
PositionDrz UnesiDrz(PositionDrz p, PositionDrz q) {
	if (p == NULL)
		return q;
	if (strcmp(p->ime_drzave, q->ime_drzave) >= 0)
		p->left = UnesiDrz(p->left, q);
	else if (strcmp(p->ime_drzave, q->ime_drzave) < 0)
		p->right = UnesiDrz(p->right, q);	
	return p;
}
void Ispis_I(PositionDrz p) {
	if (p == NULL) {
		return;
	}
	Ispis_I(p->left);
	printf("\n\t********%s********\n\n", p->ime_drzave);
	Ispis(&p->grad);
	Ispis_I(p->right);
}
int Ispis(PositionGrad p) {
	p = p->next;
	while (p != NULL) {
		printf("\t%s\t%d\n", p->ime_grada,p->broj_stanovnika);
		p = p->next;
	}
	return EXIT_SUCCESS;
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
			if (TraziGradove(&TrazenaDrzava->grad, br_stan) == 0) {
				printf("\tTrazeni gradovi nisu pronadeni!\n");
			}
		}
	} while (strcmp(ime_drzave, "exit"));
	return EXIT_SUCCESS;
}
PositionDrz PronadiDrzavu(PositionDrz p, char* ime_drzave) {
	if (p == NULL)
		return NULL;
	if (strcmp(ime_drzave,p->ime_drzave)<0)
		return PronadiDrzavu(p->left,ime_drzave);
	else if (strcmp(ime_drzave, p->ime_drzave) > 0)
		return  PronadiDrzavu(p->right, ime_drzave);
	return p;
}
int TraziGradove(PositionGrad p, int br_stanovnika) {
	int br = 0;
	p = p->next;
	while (p != NULL && p->broj_stanovnika > br_stanovnika) {
		printf("\t%s\t%d\n", p->ime_grada, p->broj_stanovnika);
		br++;
		p = p->next;
	}
	

	return br;
}
int BrisiGrad(PositionGrad p) {	
	PositionGrad q;
	while (p->next != NULL) {
		q = p->next;
		p->next = q->next;
		free(q);
	}
	return EXIT_SUCCESS;
}
PositionDrz BrisiDrzave(PositionDrz root) {
	if (root == NULL)
		return NULL;
	BrisiDrzave(root->right);
	BrisiDrzave(root->left);
	BrisiGrad(&root->grad);	
	free(root);	
	return NULL;
}


