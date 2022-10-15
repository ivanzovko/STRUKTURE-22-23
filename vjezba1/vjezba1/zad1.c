#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_FILE_NAME (256)
#define GRESKA_OTVARANJA (-1)
#define MAX_RED (1024)
#define MAX_I_P (10)
#define max_br_bodova (150)

int brojiStudenteizDatoteke(char* ime_dat);
typedef struct Student {
	char Ime_studenta[MAX_I_P];
	char Prezime_studenta[MAX_I_P];
	int broj_bodova_aps;
	double broj_bodova_realno;
} Student;
void citajPodatke(Student*, char* ime_dat, int br_studenata);

int main() {
	char ime_dat[MAX_FILE_NAME] = { 0 };
	int br_studenata = 0;
	int i = 0;

	printf("Molimo unesite ime datoteke: ");
	scanf(" %s", ime_dat);
	if (brojiStudenteizDatoteke(ime_dat) != GRESKA_OTVARANJA) {
		br_studenata = brojiStudenteizDatoteke(ime_dat);
		printf("Broj studenata u datoteci %s je %d.\n", ime_dat, br_studenata);
		Student* st;
		st = (Student*)malloc(br_studenata * sizeof(Student));
		citajPodatke(st, ime_dat, br_studenata);
	}
	return EXIT_SUCCESS;
}

int brojiStudenteizDatoteke(char* ime_dat) {
	FILE* fp = NULL;
	int brojac = 0;
	char buffer[MAX_RED] = { 0 };

	fp = fopen(ime_dat, "r");
	if (fp == NULL) {
		printf("Postovani, datoteka %s se nije ispravno otvorila!\r\n", ime_dat);
		return GRESKA_OTVARANJA;
	}

	while (fgets(buffer, MAX_RED, fp)) {// sa !feof funkcijom imam problem ukoliko je zadnji redak prazan ocitava ga kao da je popunjen
		if (strcmp("\n", buffer) != 0)
			brojac++;
	}
	fclose(fp);
	return brojac;
}

void citajPodatke(Student* st, char* ime_dat, int br_studenata) {
	FILE* fp = NULL;
	int i = 0;

	fp = fopen(ime_dat, "r");
	if (fp == NULL)
		printf("Postovani, datoteka %s se nije ispravno otvorila!\r\n", ime_dat);

	for (i = 0; i < br_studenata; i++) {
		fscanf(fp, "%s %s %d", (st + i)->Ime_studenta, (st + i)->Prezime_studenta, &(st + i)->broj_bodova_aps);
		(st + i)->broj_bodova_realno = (st + i)->broj_bodova_aps / (double)max_br_bodova * 100;
	}

	printf("       Ime    Prezime Aps.bod  Rel.bod\n");
	for (i = 0; i < br_studenata; i++)
		printf("%*s%*s%*d%*.2f\n", 10, (st + i)->Ime_studenta, 11, (st + i)->Prezime_studenta, 8, (st + i)->broj_bodova_aps, 9, (st + i)->broj_bodova_realno);//brojevi 10,11,8,9 sluze kako bi se centrirao ispis

	fclose(fp);
}