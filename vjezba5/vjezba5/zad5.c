#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define GRESKA_ALOKACIJE (-1)
#define GRESKA_OTVARANJA (-2)
#define NULA (-3)
#define NEISPRAVNA_OPER (-4)
#define prazna_dat (-5)
#define MAX_FILE_NAME (256)
#define MAX_RED (1024)

struct Izraz;
typedef struct Izraz* Position;
typedef struct Izraz {
	float broj;
	Position next;
}Izraz;

int CitajIzraz(char* ime_dat, Position p);
int Ispis(Position p);
int push(float n,Position p);
float pop(Position p,int* flag);
int BrojiStog(Position p);
int BrisiSve(Position p);
int main() {
	Izraz head = {
		.broj = 0.0,
		.next = NULL
	};
	char ime_dat[MAX_FILE_NAME]={0};
	int br = 0;
	printf("Unesite ime datoteke:\t");
	scanf(" %s", ime_dat);
	if (CitajIzraz(ime_dat, &head)==0) {
		if (BrojiStog(head.next) == 1) {
			printf("Vas rezultat je:\t");
			Ispis(head.next);
		}
		else {
			printf("Neispravno formatiran izraz!\n");
		}
	}
	BrisiSve(&head);
	return EXIT_SUCCESS;
}


int CitajIzraz(char* ime_dat, Position p) {	
	FILE* fp = NULL;
	fp = fopen(ime_dat, "r");
	if (fp == NULL) {
		printf("Postovani, datoteka %s se nije ispravno otvorila!\r\n", ime_dat);
		return GRESKA_OTVARANJA;
	}
	char buffer[MAX_RED] = { 0 };
	char* pok = buffer;
	float n = 0,prvi=0,drugi=0;
	int flag;
	while (!feof(fp)) {
		fscanf(fp, " %s", buffer);
		if (sscanf(pok, "%f", &n) != 1) {
			switch (*pok) {
			case '+':	
				drugi = pop(p,&flag);
				prvi = pop(p, &flag);
				if (flag == 1)
					return EXIT_FAILURE;
				push(prvi + drugi,p); 
				break;
			case '-':
				drugi = pop(p, &flag);
				prvi = pop(p, &flag);
				if (flag == 1)
					return EXIT_FAILURE;
				push(prvi - drugi, p);
				break;
			case '*':
				drugi = pop(p, &flag);
				prvi = pop(p, &flag);
				if (flag == 1)
					return EXIT_FAILURE;
				push(prvi * drugi, p);
				break;
			case '/':
				drugi = pop(p, &flag);
				prvi = pop(p, &flag);
				if (flag == 1)//STOG JE PRAZAN
					return EXIT_FAILURE;
				if (drugi == 0) {
					printf("Zabranjeno je dijeljenje s nulom! >:(\n");
					return NULA;
				} else {
					push(prvi / drugi, p);
					}			
				break;
			default:
				printf("Unijeli ste neispravnu operaciju!\n");				
				return NEISPRAVNA_OPER;
				break;
			}
		}
		else {
			push(n,p);
		}			
	}
	
	return EXIT_SUCCESS;
}
int BrojiStog(Position p) {
	int br = 0;
	while (p != NULL)
	{
		br++;
		p = p->next;		
	}
	return br;
}
int Ispis(Position p) {
	while (p != NULL)
	{
		printf("%.2f\n", p->broj);
		p = p->next;
		}
	return EXIT_SUCCESS;
}
int push(float n, Position p) {//UpisNaPocetak
	Position q = NULL;
	q = (Position)malloc(sizeof(struct Izraz));
	if (q == NULL) {
		printf("Greska alokacije");
		return EXIT_FAILURE;
	}	
	q->broj = n;
	q->next = p->next;
	p->next = q;
	return EXIT_SUCCESS;
}
float pop(Position p, int *flag) {//BrisanjesPocetka
	Position q;
	*flag = 0;
	if (p->next == NULL) {
		printf("Stog je prazan\n");
		*flag = 1;
		return EXIT_FAILURE;
	}
	float pom=0;
	q = p->next;
	p->next= q->next;
	pom = q->broj;
	free(q);
	return pom;
}
int BrisiSve(Position p) {
	Position q;
	while (p->next != NULL) {
		q = p->next;
		p->next = q->next;
		free(q);
	}

}