#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>

#define GRESKA_ALOKACIJE (-2)
#define MAX_LENGTH (256)
#define GRESKA_OTVARANJA (-1)
#define MAX_RED (1024)
#define MAX_NAME (128)
//Lista artikala pojedinog racuna koja sadrži ime artikla,cijenu i kolicinu
struct Article;
typedef struct Article* ArtPosition;
typedef struct Article {
	char naziv_artikla[MAX_LENGTH];
	float cijena;
	float kolicina;
	ArtPosition next;
}Article;
//Lista u kojoj su spremljeni svi datumi
struct Date;
typedef struct Date* DatePosition;
typedef struct Date {
	int godina;
	int mjesec;
	int dan;
}Date;
//Lista sadrzaja svih racuna
struct Racun;
typedef struct Racun* RPosition;
typedef struct Racun {
	char naziv_racuna[MAX_LENGTH];
	DatePosition datum;
	Article artikli;
	RPosition next;
}Racun;

int IspisMenu();
int CitajIzDat(char* ime_dat, RPosition racun);
RPosition UpisiRacunaUListu(char* buffer, int* flag);
int InicRacun(RPosition racun);
int InicArticle(ArtPosition art);
DatePosition UnosDatuma(char* buffer2);
ArtPosition UnosArtikla(char* buffer2);
int UnesiSortArt(ArtPosition head, ArtPosition artikl);
int UnesiSortRacun(RPosition racun, RPosition pom);
int Datecmp(DatePosition datum1, DatePosition datum2);
int IspisRacuna(RPosition racun);
int IspisArtikli(ArtPosition p);
ArtPosition TraziArt(ArtPosition artikl, char* ime_artikl);
int BrisiSve(RPosition racun);
int ObrisiArtikle(ArtPosition artikl);