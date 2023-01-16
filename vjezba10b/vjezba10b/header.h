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
typedef enum Greske {
	OK,
	ERROR_OPENNING_FILE = -1,
	ERROR_ALLOCATING_MEMORY = -2
}Greske;
typedef struct Gradovi {
	char ime_grada[MAX_IME_GRADA];
	int broj_stanovnika;
	PositionGrad next;
}Gradovi;
typedef struct Drzave {
	char ime_drzave[MAX_IME_DRZAVE];
	PositionDrz left;
	PositionDrz right;
	Gradovi grad;
}Drzave;



PositionDrz CitajDatoteku(PositionDrz stablo_drzava, int* status);
PositionDrz Kreiranje(char* redak_drzave);
PositionDrz AlokacijaStabla(char* ime_drz);
int InicGrad(PositionGrad p);
int CitajGradove(PositionDrz q, char* ime_dat);
PositionGrad AlokacijaGrada(char* ime_grada, int br_stan);
int UnosGradSort(PositionGrad grad, PositionGrad q);
int GradCompare(PositionGrad p, PositionGrad q);
PositionDrz UnesiDrz(PositionDrz p, PositionDrz q);
void Ispis_I(PositionDrz p);
int Ispis(PositionGrad p);
int TraziGlavna(PositionDrz drzava);
PositionDrz PronadiDrzavu(PositionDrz p, char* ime_drzave);
int TraziGradove(PositionGrad p, int br_stanovnika);
int BrisiGrad(PositionGrad p);
PositionDrz BrisiDrzave(PositionDrz root);



