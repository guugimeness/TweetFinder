// Trabalho Final - ORI (2025/1)
//   TweetFinder
//
// Gustavo de Oliveira Gimenes - 820759
// Vinícius Marto da Veiga - 821252
// Guilherme Bartoletti Oliveira - 821881
// Tiago de Paula Evangelista - 824369

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TabelaHash.h"

#define FILE_NAME "corpus-reduzido.csv"
#define MAX_QUERY_LEN 100
#define MAX_WORD_LEN 100
#define MAX_POST_LEN 200
#define HASH_SIZE 10000
#define MAX_POSTS 2000000
#define TABLE_SIZE 12289

// Estrutura para armazenar posts
typedef struct {
    int id;
    char content[MAX_POST_LEN];
    long offset;
} Post;

// Variáveis globais
Post posts[MAX_POSTS];
HashTable *Hash;
int total_posts = 0;

// Limpar pontuações e espaços no início e fim das postagens
void limpa_post(char* str) {
    if (!str || *str == '\0') return;

    char* content = str; 

    // Remove espaços/pontuações no início
    while (*content && (isspace((unsigned char)*content) || ispunct((unsigned char)*content))) {
        content++;
    }

    // Remove espaços/pontuações no final
    char* end = content + strlen(content) - 1;
    while (end >= content && (isspace((unsigned char)*end) || ispunct((unsigned char)*end))) {
        end--;
    }
    *(end + 1) = '\0';

    if (content != str) {
        memmove(str, content, strlen(content) + 1);
    }
}

// Adiciona posição do post no arquivo à lista de ocorrências daquela palavra
void adiciona_offset(Node* node, long offset) {
    IntList* atual = node->ids;
    while (atual) {
        if (atual->offset == offset)
            return;     // já existe, não insere duplicado
        atual = atual->next;
    }
    // insere no início
    IntList* novo = malloc(sizeof(IntList));
    novo->offset = offset;
    novo->next = node->ids;
    node->ids = novo;
}

// Processa o post, adicionando ele na HashTable
void processa_post(int index, char* content) {
    char* palavra = strtok(content, " ,.;!?()[]{}<>\"'\t\n\r");
    while (palavra) {
        limpa_post(palavra);     // removendo pontuações
        for (int i = 0; palavra[i]; i++) palavra[i] = tolower(palavra[i]);      // normalizando

        if (strlen(palavra) > 0) {
            // printf("Adicionando palavra %s na hash", palavra);
            Node* existente = buscaHashTable(Hash, palavra);
            if (existente) {
                adiciona_offset(existente, posts[index].offset); 
            } else {
                Node* novo = malloc(sizeof(Node));
                novo->key = strdup(palavra);
                novo->ids = NULL;
                novo->next = NULL;
                adiciona_offset(novo, posts[index].offset);
                insereHashTable(Hash, novo);
            }
        }
        palavra = strtok(NULL, " ,.;!?()[]{}<>\"'\t\n\r");
    }
}

// Carregar posts do arquivo
void carrega_posts(const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    char line[500];
    long offset = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "docID,") != NULL) {
            offset = ftell(file);
            continue;   // Ignorar cabeçalho
        }

        line[strcspn(line, "\n")] = '\0';   // Substituir \n por \0
        
        // Extraindo ID
        char* id_str = strtok(line, ",");
        if (!id_str) continue;
        
        // Ignorando segundo campo
        strtok(NULL, ",");

        // Extraindo a postagem 
        char* content = strtok(NULL, "");
        if (content) {
            posts[total_posts].id = atoi(id_str);
            posts[total_posts].offset = offset;
            limpa_post(content);
            strcpy(posts[total_posts].content, content);
            processa_post(total_posts, content);
            total_posts++;
        }
        offset = ftell(file);
    }
    fclose(file);
}

int main()
{
    Hash = criaHashTable(TABLE_SIZE);
    carrega_posts(FILE_NAME);      
    char query[MAX_QUERY_LEN];

    // Loop de interação com o usuário
    while(1)
    {
        printf("\n Digite sua consulta ('sair' para terminar | 'help' para ajuda ):\n ");
        fgets(query, MAX_QUERY_LEN, stdin);
        query[strcspn(query, "\n")] = '\0';     // Substituir \n por \0

        if (strcmp(query, "help") == 0)
        {
            printf("\n             Bem-vindo ao Tweet Finder - seu buscador de tweets\n \
            Você pode digitar uma lista de palavras e será exibido todas as postagens que contém as palavras da lista.\n \
            Você pode usar os operadores AND e OR para NOT para compor a sua busca.\n \
            Exemplo: (usa AND NOT(winner)) OR (brazil AND carnaval)\n" );
            continue;
        }

        if (strcmp(query, "sair") == 0) break;

        FILE* file = fopen(FILE_NAME, "r");
        Node* no = buscaHashTable(Hash, query);
        if (no && file) {
            IntList* atual = no->ids;
            char line[500];
            while (atual) {
                fseek(file, atual->offset, SEEK_SET);
                if (fgets(line, sizeof(line), file)) {
                    printf("→ %s", line);
                }
                atual = atual->next;
            }
            fclose(file);
        }
    }

    return 0;
}