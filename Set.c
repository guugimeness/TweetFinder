#include <stdio.h>
#include <stdlib.h>
#include "Set.h" 
#include "ArvoreAVL.h"

// Definição do tipo Set
struct set{
    ArvAVL* arv;
    int qtd;
    struct iterator *iter;
    int negacao;    // 0 = sem negacao, 1 = com negacao
};

Set* criaSet(){
    Set* s = (Set*) malloc(sizeof(Set));
    if(s != NULL){
        s->arv = cria_ArvAVL();
        s->qtd = 0;
        s->iter = NULL;
        s->negacao = 0;
    }
    return s;
}

void liberaSet(Set* s){
    if(s != NULL){
        libera_ArvAVL(s->arv);

        struct iterator* no;
        while(s->iter != NULL){
            no = s->iter;
            s->iter = s->iter->prox;
            free(no);
        }

        free(s);
    }
}

int insereSet(Set* s, int num){
    if(s == NULL)
        return 0;

    if(insere_ArvAVL(s->arv,num)){
        s->qtd++;
        return 1;
    }else
        return 0;
}

int removeSet(Set* s, int num){
    if(s == NULL)
        return 0;

    if(remove_ArvAVL(s->arv,num)){
        s->qtd--;
        return 1;
    }else
        return 0;
}

int tamanhoSet(Set* s){
    if(s == NULL)
        return 0;

    return s->qtd;
}

int consultaSet(Set* s, int num){
    if(s == NULL)
        return 0;

    int contem = consulta_ArvAVL(s->arv, num);
    return s->negacao ? !contem : contem;
}


void imprimeSet(Set* s){
    if(s == NULL)
        return;

    emOrdem_ArvAVL(s->arv);
}

void beginSet(Set *s){
    if(s == NULL)
        return;

    s->iter = NULL;
    iterator_ArvAVL(s->arv, &(s->iter));
}

int endSet(Set *s){
    if(s == NULL)
        return 1;
    if(s->iter == NULL)
        return 1;
    else
        return 0;
}

void nextSet(Set *s){
    if(s == NULL)
        return;
    if(s->iter != NULL){
        struct iterator *no = s->iter;
        s->iter = s->iter->prox;
        free(no);
    }
}

void getItemSet(Set *s, int *num){
    if(s == NULL)
        return;
    if(s->iter != NULL)
        *num = s->iter->valor;
}


Set* uniaoSet(Set* A, Set* B){
    if(A == NULL || B == NULL)
        return NULL;
    int x;
    Set *C = criaSet();

    for(beginSet(A); !endSet(A); nextSet(A)){
        getItemSet(A, &x);
        insereSet(C,x);
    }

    for(beginSet(B); !endSet(B); nextSet(B)){
        getItemSet(B, &x);
        insereSet(C,x);
    }
    C->negacao = 0;
    return C;
}

Set* interseccaoSet(Set* A, Set* B){
    if(A == NULL || B == NULL)
        return NULL;
    int x;
    Set *C = criaSet();

    if(tamanhoSet(A) < tamanhoSet(B)){
        for(beginSet(A); !endSet(A); nextSet(A)){
            getItemSet(A, &x);
            if(consultaSet(B,x))
                insereSet(C,x);
        }
    }else{
        for(beginSet(B); !endSet(B); nextSet(B)){
            getItemSet(B, &x);
            if(consultaSet(A,x))
                insereSet(C,x);
        }
    }
    C->negacao = 0;
    return C;
}

// Códigos criados pelos alunos para o trabalho

void resolveNOT(Set* s){
    if(s == NULL)
        return;
    s->negacao = !s->negacao;
}

Set* diferencaSet(Set* A, Set* B){
    if(A == NULL || B == NULL) return NULL;
    int x;
    Set* C = criaSet();

    for(beginSet(A); !endSet(A); nextSet(A)){
        getItemSet(A, &x);
        if(!consultaSet(B, x))
            insereSet(C, x);
    }
    C->negacao = 0;
    return C;
}

Set* resolveAND(Set* A, Set* B){
    if(A == NULL || B == NULL) return NULL;

    if(A->negacao && B->negacao){
        // ~A ∩ ~B = ~(A ∪ B)
        Set* tmp = uniaoSet(A, B);
        resolveNOT(tmp);
        return tmp;
    }
    else if(A->negacao){
        // ~A ∩ B = B - A
        return diferencaSet(B, A);
    }
    else if(B->negacao){
        // A ∩ ~B = A - B
        return diferencaSet(A, B);
    }
    else {
        return interseccaoSet(A, B);
    }
}

Set* resolveOR(Set* A, Set* B){
    if(A == NULL || B == NULL) return NULL;

    if(A->negacao && B->negacao){
        // ~A ∪ ~B = ~(A ∩ B)
        Set* tmp = interseccaoSet(A, B);
        resolveNOT(tmp);
        return tmp;
    }
    else if(A->negacao){
        // ~A ∪ B = ~(A - B)
        Set* tmp = diferencaSet(A, B);
        resolveNOT(tmp);
        return tmp;
    }
    else if(B->negacao){
        Set* tmp = diferencaSet(B, A);
        resolveNOT(tmp);
        return tmp;
    }
    else {
        return uniaoSet(A, B);
    }
}

