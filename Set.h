#ifndef SET_H
#define SET_H

typedef struct set Set;

Set* criaSet();
void liberaSet(Set* s);
int insereSet(Set* s, int num);
int removeSet(Set* s, int num);
int tamanhoSet(Set* s);
void imprimeSet(Set* s);
int consultaSet(Set* s, int num);

Set* uniaoSet(Set* A, Set* B);
Set* interseccaoSet(Set* A, Set* B);

void beginSet(Set *s);
int endSet(Set *s);
void nextSet(Set *s);
void getItemSet(Set *s, int *num);

Set* resolveOR(Set* A, Set* B);
Set* resolveAND(Set* A, Set* B);
Set* diferencaSet(Set* A, Set* B);
void resolveNOT(Set* s);

#endif // SET_H