#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define GRESKA_ALOKACIJE (-1)
#define GRESKA_OTVARANJA (-2)
#define MAX_FILE_NAME (256)
#define MAX_RED (1024)

struct Polinom;
typedef struct Polinom* Position;
typedef struct Polinom {
	int Koef;
	int Pot;
	Position next;
}Polinom;
int CitajPolinome(char* ime_dat, Position p, Position q);
int UnosSort(int koef,int pot,Position p);
int MnoziPolinome(Position prvi,Position drugi,Position umnozak);
int ZbrojiPolinome(Position prvi, Position drugi, Position zbroj);
int ZbrojiPomnozeno(Position p);
int Brisi(int a,Position p);

int main() {
	Polinom head1 = {
		.Koef = {0},
		.Pot = {0},	
		.next = NULL
	};
	Polinom head2 = {
		.Koef = {0},
		.Pot = {0},
		.next = NULL
	};
	Polinom head3 = {
		.Koef = {0},
		.Pot = {0},
		.next = NULL
	};
	Polinom head4 = {
		.Koef = {0},
		.Pot = {0},
		.next = NULL
	};
	char ime_dat[MAX_FILE_NAME]= { 0 };
	printf("Molimo unesite ime datoteke iz koje zelite procitati polinome:\t");
	scanf(" %s", ime_dat);
	if (CitajPolinome(ime_dat, &head1, &head2) != EXIT_SUCCESS)
		return EXIT_FAILURE;
	if (ZbrojiPolinome(&head1, &head2, &head3) != EXIT_SUCCESS)
		return EXIT_FAILURE;
	if(MnoziPolinome(&head1, &head2, &head4)!=EXIT_SUCCESS)
		return EXIT_FAILURE;
	printf("Prvi polinom:\t");	
	Ispis(&head1);
	printf("\nDrugi polinom:\t");
	Ispis(&head2);
	printf("\nZbroj polinoma:\t");
	Ispis(&head3);
	printf("\nUmnozak polinoma:\t");
	Ispis(&head4);
	return EXIT_SUCCESS;
}
int CitajPolinome(char* ime_dat, Position p, Position q) {
	
	FILE* fp = NULL;
	fp = fopen(ime_dat, "r");
	char buffer[MAX_RED] = { 0 };	
	int pot, koef = 0;
	char* pok = buffer;
	int n, tmp = 0;
	if (fp == NULL) {
		printf("Postovani, datoteka %s se nije ispravno otvorila!\r\n", ime_dat);
		return GRESKA_OTVARANJA;
	}

	fgets(buffer, MAX_RED, fp);
	tmp = strlen(buffer);
	while (tmp > 0) {
		sscanf(pok, "%d %d %n", &koef, &pot,&n);
		if (UnosSort(koef, pot, p) != EXIT_SUCCESS)
			return EXIT_FAILURE;
		pok += n;
		tmp = tmp - n;
			
	}
	fgets(buffer, MAX_RED, fp);	
	tmp = strlen(buffer);
	pok = buffer;
	while (tmp > 0) {
		sscanf(pok, "%d %d %n", &koef, &pot, &n);
		if (UnosSort(koef, pot, q) != EXIT_SUCCESS)
			return EXIT_FAILURE;
		pok += n;
		tmp = tmp - n;
		}
	fclose(fp);
	return EXIT_SUCCESS;
}
int UnosSort(int koef, int pot, Position p) {
	while (p->next != NULL && p->next->Pot > pot)
		p = p->next;
	Position q = (Position)malloc(sizeof(struct Polinom));
	if (q == NULL)
		return GRESKA_ALOKACIJE;
	q->Koef = koef;
	q->Pot = pot;
	q->next = p->next;
	p->next = q;
	return EXIT_SUCCESS;
}
int Ispis(Position p) {
	while (p->next != NULL) {
		printf("%d  %d\t", p->next->Koef, p->next->Pot);
		p = p->next;
	}
	return EXIT_SUCCESS;
}
int MnoziPolinome(Position prvi, Position drugi, Position umnozak) {
	Position rezultat = umnozak;
	prvi = prvi->next;
	drugi = drugi->next;
	Position pocetak = drugi;
	int koef, pot = 0;
	while (prvi != NULL) {
		drugi = pocetak;
		while (drugi != NULL) {
			koef = prvi->Koef * drugi->Koef;
			pot = prvi->Pot + drugi->Pot;
			if (UnosSort(koef, pot, umnozak) != EXIT_SUCCESS)
				return EXIT_FAILURE;
			drugi = drugi->next;
		}
		prvi = prvi->next;
	}
	if (ZbrojiPomnozeno(rezultat) != EXIT_SUCCESS)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
int ZbrojiPolinome(Position prvi, Position drugi, Position zbroj) {
	prvi = prvi->next;
	drugi = drugi->next;
	int koef, pot = 0;
	while (prvi != NULL && drugi != NULL) {

		if (prvi->Pot == drugi->Pot) {
			koef = prvi->Koef + drugi->Koef;
			pot = prvi->Pot;
			prvi = prvi->next;
			drugi = drugi->next;
					
		}
		else if (prvi->Pot > drugi->Pot) {
			koef = prvi->Koef;
			pot = prvi->Pot;
			prvi = prvi->next;
		}
		else {
			koef = drugi->Koef;
			pot = drugi->Pot;
			drugi = drugi->next;
		}
		if (UnosSort(koef, pot, zbroj) != EXIT_SUCCESS)
			return EXIT_FAILURE;
	}
	while (prvi != NULL) {
		koef = prvi->Koef;
		pot = prvi->Koef;
		if (UnosSort(koef, pot, zbroj) != EXIT_SUCCESS)
			return EXIT_FAILURE;
		prvi = prvi->next;
	}
	while (drugi != NULL) {
		koef = drugi->Koef;
		pot = drugi->Koef;
		if (UnosSort(koef, pot, zbroj) != EXIT_SUCCESS)
			return EXIT_FAILURE;
		drugi = drugi->next;
	}
	return EXIT_SUCCESS;
		
}
int ZbrojiPomnozeno(Position p) {
	Position pocetak=p;
	p = p->next;
	if (p != NULL) {
		while (p->next != NULL) {
			if (p->Pot == p->next->Pot) {
				p->Koef = p->Koef + p->next->Koef;
				if (Brisi(p->Pot, pocetak) != EXIT_SUCCESS)
					return EXIT_FAILURE;
			}
			if(p->Pot != p->next->Pot)//Ukoliko postoje tri ili više ista pribrojnika
				p = p->next;

		}
	}
	else {
		printf("Nema elemenata");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
int Brisi(int a, Position p) {
	Position temp=NULL;
	p = p->next;
	if (p == NULL) {
		printf("Prazna lista");
		return EXIT_FAILURE;
	}
	while (p->next != NULL && p->Pot != a)
		p = p->next;
	if (p == NULL) {
		printf("Nema tog studenta");
		return EXIT_FAILURE;
	}
	temp = p->next;
	p->next = temp->next;
	free(temp);
	return EXIT_SUCCESS;
}