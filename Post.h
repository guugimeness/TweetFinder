#ifndef POST_H
#define POST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TabelaHash.h"

#define MAX_POST_LEN 200
#define MAX_POSTS 2000000

// Estrutura para armazenar posts
typedef struct {
    int id;
    char content[MAX_POST_LEN];
    long offset;
} Post;

extern HashTable *Hash;
extern Post posts[MAX_POSTS];
extern int total_posts;

void limpa_post(char* str);
void adiciona_offset(Node* node, long offset);
void processa_post(int index, char* content);
void carrega_posts(const char* filename);
void imprime_resultados(Set* result, const char* filename);

#endif // POST_H