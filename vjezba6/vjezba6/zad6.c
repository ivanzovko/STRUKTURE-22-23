#include "header.h"

int main() {
	Racun head;
	InicRacun(&head);
	int pom,godina1, mjesec1, dan1, godina2, mjesec2, dan2,br_artikla=0;//Pom-hvatenje greske,a ostale za search dio
	char ime_dat[MAX_LENGTH],unos[MAX_LENGTH], ime_artikl[MAX_LENGTH] = { 0 };//unos-
	ArtPosition trazi_art, pom_art = NULL;
	DatePosition date1, date2 = NULL;
	float kolicina_artikala = 0;
	RPosition racun = &head;

	printf("\tUnesite ime glavne datoteke:\t");	
	gets(ime_dat);
	pom = CitajIzDat(ime_dat, racun);
	if (pom != 0)
			return pom;
	IspisRacuna(racun->next);
	IspisMenu();
	//SEARCH DIO
	do {
		pom_art = NULL;
		br_artikla = 0;
		kolicina_artikala = 0;
		racun = &head;
		printf("\tVas unos:   ");
		fgets(unos, MAX_LENGTH, stdin);
		sscanf(unos, "%s %d-%d-%d %d-%d-%d", ime_artikl,&godina1,&mjesec1,&dan1,&godina2,&mjesec2,&dan2);
		if (strcmp(ime_artikl, "exit") != 0) {
			date1=date2 = NULL;
			date1 = (DatePosition)malloc(sizeof(Date));
			if (date1 == NULL) {
				printf("Greska alokcije datuma prilikom trazenja!\n");
				return GRESKA_ALOKACIJE;
			}
			date1->godina = godina1;
			date1->mjesec = mjesec1;
			date1->dan = dan1;
			date2 = (DatePosition)malloc(sizeof(Date));
			if (date2 == NULL) {
				printf("Greska alokcije datuma prilikom trazenja!\n");
				return GRESKA_ALOKACIJE;
			}
			date2->godina = godina2;
			date2->mjesec = mjesec2;
			date2->dan = dan2;
			racun = racun->next;
			trazi_art = NULL;
			while (racun != NULL) {
				if ((Datecmp(racun->datum, date1) >= 0) && (Datecmp(racun->datum, date2) <= 0)) {
					trazi_art = TraziArt(&racun->artikli, ime_artikl);
					if (trazi_art != NULL) {
						pom_art = trazi_art;
						kolicina_artikala = kolicina_artikala + trazi_art->kolicina;
						br_artikla++;
					}
				}
				racun = racun->next;
			}
			 if (pom_art == NULL) {
				  printf("\tArtikl nije pronaden!\n");
			  } 
			 else {
					printf("\tArtikl %s je kupljen % d puta!\n\tUkupna kolicina: %.2f\n\tZarada:%.2f kn\n", pom_art->naziv_artikla, br_artikla, kolicina_artikala, kolicina_artikala * pom_art->cijena);
					printf("\t**********************************\n");
			 }
			 free(date1);
			 free(date2);
		}
	} while (strcmp(ime_artikl,"exit"));
	BrisiSve(&head);
	return EXIT_SUCCESS;
}

int IspisMenu() {
	printf("\t**********************************\n");
	printf("\t              SEARCH\n");
	printf("\t PRIMJER: kruh 2021-8-1 2021-8-8\n");
	return EXIT_SUCCESS;
}
int CitajIzDat(char* ime_dat, RPosition racun) {
	FILE* fp = NULL;
	fp = fopen(ime_dat, "r");
	if (fp == NULL) {
		printf("Postovani, datoteka %s se nije ispravno otvorila!\r\n", ime_dat);
		return GRESKA_OTVARANJA;
	}
	RPosition home = racun;
	int flag = 0;
	char buffer[MAX_RED] = { 0 };
	RPosition pom = NULL;
	while (!feof(fp)) {
		pom = 0;
		fscanf(fp, " %s", buffer);
			pom = UpisiRacunaUListu(buffer, &flag);
		if (pom ==NULL && flag==1) {
			fclose(fp);
			return GRESKA_OTVARANJA;
		}
		else if (pom == NULL && flag == 0) {
			fclose(fp);
			return GRESKA_ALOKACIJE;
		}
		UnesiSortRacun(racun, pom);
		
	}
	fclose(fp);
	return EXIT_SUCCESS;
}
RPosition UpisiRacunaUListu(char *buffer,int* flag ) {
	*flag = 0;
	FILE* fp = NULL;
	fp = fopen(buffer, "r");
	if (fp == NULL) {
		printf("Postovani, datoteka %s se nije ispravno otvorila!\r\n", buffer);
		*flag = 1;
		return NULL;
	}
	RPosition q = NULL;
	q = (RPosition)malloc(sizeof(Racun));
	if (q == NULL) {
		printf("Greska alokacije racuna!\n");
		fclose(fp);
		return NULL;
	}
	InicRacun(q);
	strcpy(q->naziv_racuna, buffer);
	char buffer2[MAX_RED] = { 0 };
	int br = 0;
	char* pok = buffer2;
	while (!feof(fp)) {
		ArtPosition	artikl = NULL;
		fgets(buffer2, MAX_RED, fp);
		if (br == 0) {
			q->datum = UnosDatuma(buffer2);
			if (q->datum == NULL) {
				fclose(fp);
				return NULL;
			}
			br++;
		}
		else {
			artikl = UnosArtikla(buffer2);
			if (artikl == NULL) {
				fclose(fp);
				return NULL;
			}
			UnesiSortArt(&q->artikli, artikl);
			//PREPOZNAVANJE DATUMA TJ. PRVI REDAK U RACUNU
		}
		
	}
	fclose(fp);
	return q;
}
int InicRacun(RPosition racun) {
	memset(racun->naziv_racuna, 0, MAX_NAME);
	racun->datum = NULL;
	racun->next = NULL;
	InicArticle(&racun->artikli);
	return EXIT_SUCCESS;
}
int InicArticle(ArtPosition art) {
	art->cijena = 0;
	art->kolicina = 0;
	memset(art->naziv_artikla, 0, MAX_NAME);
	art->next = NULL;
	return EXIT_SUCCESS;
}
DatePosition UnosDatuma(char* buffer) {
	char* pok = buffer;
	DatePosition p = NULL;
	p = (DatePosition)malloc(sizeof(Date));
	if (p == NULL) {
		printf("Greska alokacije datuma!\n");
		return NULL;
	}
	sscanf(pok, "%d-%d-%d", &p->godina, &p->mjesec, &p->dan);

	return p;
}
ArtPosition UnosArtikla(char* buffer) {
	ArtPosition art = NULL;
	art = (ArtPosition)malloc(sizeof(Article));
	if (art == NULL) {
		printf("Greska alokacije artikla!\n");
		return NULL;
	}
	InicArticle(art);
	sscanf(buffer, "%s %f %f", art->naziv_artikla,&art->kolicina,&art->cijena);
	return art;
}
int UnesiSortArt(ArtPosition head, ArtPosition artikl) {
	ArtPosition p = head;
	while (p->next != NULL && strcmp(p->next->naziv_artikla, artikl->naziv_artikla) < 0) {
		p = p->next;
	}
	artikl->next = p->next;
	p->next = artikl;
	return EXIT_SUCCESS;
}
int UnesiSortRacun(RPosition racun, RPosition pom) {
	RPosition p = racun;
	while (p->next != NULL && Datecmp(p->next->datum, pom->datum) < 0)
		p = p->next;
	pom->next = p->next;
	p->next = pom;
	return EXIT_SUCCESS;
}
int Datecmp(DatePosition datum1, DatePosition datum2) {
	//pozitivan broj ako je prvi datum mladi,negativan ako je stariji
	int pom = datum1->godina - datum2->godina;
	if (pom == 0) {
		pom = datum1->mjesec - datum2->mjesec;
		if (pom == 0) {
			pom = datum1->dan - datum2->dan;
		}
	}
	return pom;
}
int IspisRacuna(RPosition racun) {
	printf("\n");
	while (racun != NULL) {
		printf("\tIme racuna: %s\n", racun->naziv_racuna);
		printf("\tDatum: %d-%d-%d\n", racun->datum->godina,racun->datum->mjesec,racun->datum->dan);
		printf("\tArtikli: naziv\tkolicina  cijena\n");
		IspisArtikli(&racun->artikli);
		printf("\n");
		racun = racun->next;
	}
	return EXIT_SUCCESS;
}
int IspisArtikli(ArtPosition p) {
	p = p->next;
	while (p != NULL) {
		printf("%*s ",22, p->naziv_artikla);
		printf("%*.2f ",5, p->kolicina);
		printf("%*.2f ",11, p->cijena);
		printf("\n");
		p = p->next;
	}
	return EXIT_SUCCESS;
}
ArtPosition TraziArt(ArtPosition art ,char* ime_artikl) {
	ArtPosition artikl = art;
	while (artikl != NULL) {
		if (strcmp(artikl->naziv_artikla, ime_artikl) == 0) 
			return artikl;
		artikl = artikl->next;
	}
	return NULL;
}
int BrisiSve(RPosition racun) {
	RPosition q;
	while (racun->next != NULL) {
		free(racun->datum);
		ObrisiArtikle(&racun->artikli);
		q = racun->next;
		racun->next = q->next;
		free(q);
	}
	return EXIT_SUCCESS;
}
int ObrisiArtikle(ArtPosition artikl) {
	ArtPosition q;
	while (artikl->next != NULL) {
		q = artikl->next;
		artikl->next = q->next;
		free(q);
	}
	return EXIT_SUCCESS;
}