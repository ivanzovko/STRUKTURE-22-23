#pragma once
#define MAX_FILENAME (256)
#define MAX_RED (256)
#define MAX_IME_DRZAVE (70)
#define MAX_IME_GRADA (90)
#define MAX_LINE_GRADA (100)
//ERROR
#define GRESKA_OTVARANJA (-1)
#define GRESKA_ALOKACIJE (-2)

struct Drzave;
typedef struct Drzave* PositionDrz;
struct Gradovi;
typedef struct Gradovi* PositionGrad;

typedef struct Gradovi {
	char ime_grada[MAX_IME_GRADA];
	int broj_stanovnika;
	PositionGrad left;
	PositionGrad right;
}Gradovi;
typedef struct Drzave {
	char ime_drzave[MAX_IME_DRZAVE];
	PositionGrad grad;
	PositionDrz next;
}Drzave;
typedef enum Greske {
	OK,
	ERROR_OPENNING_FILE=-1,
	ERROR_ALLOCATING_MEMORY=-2
}Greske;


int CitajDatoteku(PositionDrz lista_drzava);
int UpisDrzave(PositionDrz home_drzave, char* redak_drzave);
PositionGrad CitajGradove(char* ime_dat);
PositionGrad AlokacijaStabla(char* ime_grada, int br_stanovnika);
PositionGrad Unos(PositionGrad p, PositionGrad q);
int UnesiSortDrz(PositionDrz lista_drzava, PositionDrz q);
void Ispis_I(PositionGrad p);
int Ispis(PositionDrz p);
PositionDrz PronadiDrzavu(PositionDrz p, char* ime_drzave);
int TraziGradove(PositionGrad root, int br_stanovnika);
int TraziGlavna(PositionDrz drzava);
int BrisiStablo(PositionGrad root);
int BrisiDrzave(PositionDrz p);


