#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME (1024)
#define GRESKA (-1)

struct osoba;
typedef struct osoba *Position;
typedef struct osoba {
	char ime[MAX_NAME];
	char prezime[MAX_NAME];
	int god;
	Position next;
 }Osoba;

int UpisP(char* ime2, char* prezime, int god, Position p);
int Ispis(Position p);
int UpisK(char* ime2, char* prezime, int god, Position p);
Position Trazi(char* prezime, Position p);
int Brisi(char* prezime,Position p);

int main() {
	Osoba head = {
		.ime = {0},
		.prezime = {0},
		.god = 0,
		.next = NULL
	};
	char ime[MAX_NAME]={ 0 };
	char prezime[MAX_NAME]={ 0 };
	int god=0;
	char end[MAX_NAME] = "";
	int br = 1;
	char odabir;
	Position p = NULL;



	do {
		printf("Zelite li studenta unijeti na pocetak ili kraj liste?\nP - Pocetak\tK - Kraj\tVas unos:");
		scanf(" %c", &odabir);
		switch (odabir) {
							case 'p':	case 'P':
								printf("Molimo unesite %d studenata!\n", br);
								br++;
								printf("Ime:\t");
								scanf(" %s", ime);
								printf("Prezime:\t");
								scanf(" %s", prezime);
								printf("Godina rodenja:\t");
								scanf("%d", &god);
								if (UpisP(ime, prezime, god, &head) == GRESKA) {
									printf("Neispravna alokacija memorije!\n");
									return EXIT_FAILURE;
								}
								break;
							case 'k':	case 'K':
								printf("Molimo unesite %d studenata!\n", br);
								br++;
								printf("Ime:\t");
								scanf(" %s", ime);
								printf("Prezime:\t");
								scanf(" %s", prezime);
								printf("Godina rodenja:\t");
								scanf("%d", &god);
								if (UpisK(ime, prezime, god, &head) == GRESKA) {
									printf("Neispravna alokacija memorije!\n");
									return EXIT_FAILURE;
								}
								break;
							default: { printf("Pogresan unos odabira unosa!\n");
										return EXIT_FAILURE;
									  }
		}
		printf("Ako zelite zavrsit unos stisnite 0!\t");
		scanf(" %s", &end);
	} while (strcmp(end,"0"));

	Ispis(head.next);
	printf("Unesite prezime studenta kojeg zelite naci:\t");
	scanf(" %s", prezime);
	p=Trazi(prezime, head.next);
	if(p!=NULL)
		printf("%s\t%s\t%d\n", p->ime, p->prezime, p->god);
	printf("Unesite prezime studenta kojeg zelite izbrisati:\t");
	scanf(" %s", prezime);
	Brisi(prezime, &head);
	Ispis(head.next);
	return EXIT_SUCCESS;
}

int UpisP(char* ime2,  char* prezime,int god, Position p) {
	Position q = NULL;
	q = (Position)malloc(sizeof(struct osoba));
	if (q == NULL)
		return GRESKA;
	strcpy(q->ime, ime2);
	strcpy(q->prezime, prezime);
	q->god = god;
	q->next = p->next;
	p->next = q;
	return EXIT_SUCCESS;
	 
}
int Ispis(Position p) {
	printf("\tISPIS LISTE\t\n");
	while (p != NULL)
	{
		printf("%s\t%s\t%d\n",p->ime, p->prezime, p->god);
		p = p->next;
	}
	return EXIT_SUCCESS;
}
int UpisK(char* ime2, char* prezime, int god, Position p) {	
	Position q = NULL;
	while (p->next != NULL)
		p = p->next;
	q = (Position)malloc(sizeof(struct osoba));
	if (q == NULL)
		return GRESKA;
	strcpy(q->ime, ime2);
	strcpy(q->prezime, prezime);
	q->god = god;
	p->next = q;
	q->next = NULL;
	return EXIT_SUCCESS;
}
Position Trazi(char* prezime, Position p) {
	while (p != NULL && strcmp(p->prezime, prezime))
		p = p->next;
	if (p == NULL)
		printf("Nema tog studenta!\n");
	return p;
}
int Brisi(char* prezime, Position p) {
	Position tmp=NULL;
	while (p->next != NULL && strcmp(p->next->prezime, prezime))
		p = p->next;
	if (p == NULL) {
		printf("Nema tog studenta");
		return GRESKA;
	}
	tmp = p;
	p = p->next;
	tmp->next = p->next;	
	free(p);
	return EXIT_SUCCESS;
}