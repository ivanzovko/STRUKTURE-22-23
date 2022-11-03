#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_NAME (1024)
#define GRESKA_ALOKACIJE (-1)
#define GRESKA_PRONALASKA_STUDENTA (-2)
#define GRESKA_OTVARANJA (-3)
#define MAX_FILE_NAME (256)

struct osoba;
typedef struct osoba* Position;
typedef struct osoba {
	char ime[MAX_NAME];
	char prezime[MAX_NAME];
	int god;
	Position next;
}Osoba;
int IspisMenu();
int Ispis(Position p);
int UnosStudenta(char *ime, char  *prezime, int *god,int *br);
int UpisP(char* ime2, char* prezime, int god, Position p);
int UpisK(char* ime2, char* prezime, int god, Position p);
int UpisIza(char* ime, char* prezime_up, int god, char* prezime_p, Position p);
int UpisIspred(char* ime, char* prezime_up, int god, char* prezime_i, Position p);
Position Trazi(char* prezime, Position p);
int Brisi(char* prezime, Position p);
int SortirajStudente(Position p);
int UpisListeUDatoteku(Position p);
int CitajListuIzDatoteku(Position p);

int main() {
	Osoba head = {
		.ime = {0},
		.prezime = {0},
		.god = 0,
		.next = NULL
	};
	char *ime[MAX_NAME] = { 0 };
	char *prezime[MAX_NAME] = { 0 };
	int god = 0;
	int br = 1;
	char odabir = '\0';
	char prezime_p[MAX_NAME] = { 0 };
	char prezime_i[MAX_NAME] = { 0 };
	Position p = NULL;
	IspisMenu();
	do {
		printf("Vas unos:");
		scanf(" %c", &odabir);
		switch (toupper(odabir)) {
			
		case 'I':
			Ispis(head.next);
			break;

		case 'P':
			UnosStudenta(ime, prezime, &god, &br);
			if (UpisP(ime, prezime, god, &head) == GRESKA_ALOKACIJE) {
				printf("Neispravna alokacija memorije!\n");
				return EXIT_FAILURE;
			}break;

		case 'K':
			UnosStudenta(ime, prezime, &god, &br);
			if (UpisK(ime, prezime, god, &head) == GRESKA_ALOKACIJE) {
				printf("Neispravna alokacija memorije!\n");
				return EXIT_FAILURE;
			}break;

		case 'N':
			UnosStudenta(ime, prezime, &god, &br);
			if (UpisIza(ime, prezime, god, &prezime_p, head.next) == GRESKA_ALOKACIJE) {
				printf("Neispravna alokacija memorije!\n");
				return EXIT_FAILURE;
			}break;

		case 'B':
			UnosStudenta(ime, prezime, &god, &br);
			if (UpisIspred(ime, prezime, god, &prezime_i, head.next) == GRESKA_ALOKACIJE) {
				printf("Neispravna alokacija memorije!\n");
				return EXIT_FAILURE;
			}break;

		case 'T':			
			p = Trazi(&prezime, head.next);
			if (p != NULL)
				printf("%s\t%s\t%d\n", p->ime, p->prezime, p->god);
			break;
		
		case 'D':
			Brisi(&prezime, &head);
			break;
		case 'S':
			SortirajStudente(&head);
			break;
		case '0':
			printf("\tDovidenja\n");
			return EXIT_SUCCESS;
			break;
		case 'U':
			UpisListeUDatoteku(head.next);
			break;
		case 'C':
			CitajListuIzDatoteku(&head);
			break;
		default:
			printf("Pogresan unos!\n");
		}
	} while (1);
	
	return EXIT_SUCCESS;
}



int IspisMenu() {
	printf("\t*********MENU*********\n");
	printf("\tIzaberite jednu od opcija:");
	printf("\n\tI - ispis liste");						//ISPIS
	printf("\n\tP - unos studenta na pocetak liste");	//POCETAK
	printf("\n\tK - unos studenta na kraj liste");		//KRAJ
	printf("\n\tN - unos studenta poslije odredenog studenta liste");	//NAKON
	printf("\n\tB - unos studenta prije odredenog studenta liste");	//BEFORE
	printf("\n\tT - trazenje studenta po prezimenu");		//TRAZI
	printf("\n\tD - brisanje studenta po prezimenu");		//DELETE
	printf("\n\tS - sortiranje studenta po prezimenu");
	printf("\n\tU - upis studenta u datoteku");
	printf("\n\tC - citanje studenata iz datoteke");
	printf("\n\t0 - izlaz\n\t");
		return EXIT_SUCCESS;
}
int Ispis(Position p) {
	printf("\tISPIS LISTE\t\n");
	while (p != NULL)
	{
		printf("%s\t%s\t%d\n", p->ime, p->prezime, p->god);
		p = p->next;
	}
	return EXIT_SUCCESS;
}
int UnosStudenta(char* ime, char* prezime, int* god, int* br) {
	printf("Molimo unesite %d studenata!\n", *br);
	printf("Ime:\t");
	scanf(" %s", ime);
	printf("Prezime:\t");
	scanf(" %s", prezime);
	printf("Godina rodenja:\t");
	scanf("%d", god);
	(*br)++;
	return EXIT_SUCCESS;
}
int UpisP(char* ime2, char* prezime, int god, Position p) {
	Position q = NULL;
	q = (Position)malloc(sizeof(struct osoba));
	if (q == NULL)
		return GRESKA_ALOKACIJE;
	strcpy(q->ime, ime2);
	strcpy(q->prezime, prezime);
	q->god = god;
	q->next = p->next;
	p->next = q;
	return EXIT_SUCCESS;

}
int UpisK(char* ime2, char* prezime, int god, Position p) {
	Position q = NULL;
	while (p->next != NULL)
		p = p->next;
	q = (Position)malloc(sizeof(struct osoba));
	if (q == NULL)
		return GRESKA_ALOKACIJE;
	strcpy(q->ime, ime2);
	strcpy(q->prezime, prezime);
	q->god = god;
	p->next = q;
	q->next = NULL;
	return EXIT_SUCCESS;
}
int UpisIza(char* ime, char* prezime_up, int god, char* prezime_p, Position p) {
	printf("Prezime studenta iza kojeg upisivate trenutnog studenta:\t");
	scanf(" %s", prezime_p);
	Position q = NULL;
	while (p != NULL && strcmp(p->prezime, prezime_p))
		p = p->next;
	if (p == NULL) {
		printf("Nema tog studenta!\n");
		return GRESKA_PRONALASKA_STUDENTA;
	}
	q = (Position)malloc(sizeof(struct osoba));
	if (q == NULL) {
		printf("Neuspjesna alokacija!\n");
		return GRESKA_ALOKACIJE;
	}
	strcpy(q->ime, ime);
	strcpy(q->prezime, prezime_up);
	q->god = god;
	q->next = p->next;
	p->next = q;

	return EXIT_SUCCESS;
}
int UpisIspred(char* ime, char* prezime_up, int god, char* prezime_i, Position p) {
	printf("Prezime studenta ispred kojeg upisivate studenta:\t");
	scanf(" %s", prezime_i);
	Position q = NULL;
	while (p != NULL && strcmp(p->prezime, prezime_i))
		p = p->next;
	if (p == NULL) {
		printf("Nema tog studenta!\n");
		return GRESKA_PRONALASKA_STUDENTA;
	}
	q = (Position)malloc(sizeof(struct osoba));
	if (q == NULL) {
		printf("Neuspjesna alokacija!\n");
		return GRESKA_ALOKACIJE;
	}
	strcpy(q->ime, ime);
	strcpy(q->prezime, prezime_up);
	q->god = god;
	q->next = p->next;
	p->next = q;
	return EXIT_SUCCESS;
}
Position Trazi(char* prezime, Position p) {
	printf("Unesite prezime studenta kojeg zelite naci:\t");
	scanf(" %s", prezime);
	while (p != NULL && strcmp(p->prezime, prezime))
		p = p->next;
	if (p == NULL)
		printf("Nema tog studenta!\n");
	return p;
}
int Brisi(char* prezime, Position p) {
	printf("Unesite prezime studenta kojeg zelite izbrisati:\t");
	scanf(" %s", prezime);
	Position tmp = NULL;
	while (p->next != NULL && strcmp(p->next->prezime, prezime))
		p = p->next;
	if (p == NULL) {
		printf("Nema tog studenta");
		return GRESKA_ALOKACIJE;
	}
	tmp = p;
	p = p->next;
	tmp->next = p->next;
	free(p);
	return EXIT_SUCCESS;
}
int SortirajStudente(Position p) {
	Position j=NULL,prev_j=NULL,temp=NULL,end=NULL;

	while (p->next != end) {
		prev_j = p;
		j = p->next;
		while (j->next != end) {
			if (strcmp(j->prezime, j->next->prezime) > 0) {
				temp = j->next;
				prev_j->next = temp;
				j->next = temp->next;
				temp->next = j;
				j = temp;
			}
			prev_j = j;
			j = j->next;
		}
		end = j;
	}

	return EXIT_SUCCESS;
}
int UpisListeUDatoteku(Position p) {
	FILE* fp=NULL;
	fp = fopen("studenti.txt", "w+");
	if (fp == NULL)
		return GRESKA_OTVARANJA;
	while (p != NULL) {
		fprintf(fp,"%s\t%s\t%d\n", p->ime, p->prezime, p->god);
		p = p->next;
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
int CitajListuIzDatoteku(Position p) {
	FILE* fp = NULL;
	char ime_dat[MAX_FILE_NAME] = { 0 };
	char ime[MAX_FILE_NAME], prezime[MAX_FILE_NAME];
	int god = 0;
	printf("unesite ime datoteke:\t");
	scanf(" %s", ime_dat);	
	fp = fopen(ime_dat, "r");
	if (fp == NULL) {
		printf("Postovani, datoteka %s se nije ispravno otvorila!\r\n", ime_dat);
		return GRESKA_OTVARANJA;
	}
	while (fscanf(fp, "%s %s %d", ime, prezime, &god)>0) 
		UpisK(ime,prezime,god,p);	
	fclose(fp);
	return EXIT_SUCCESS;
}
