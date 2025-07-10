// Trabalho Final - ORI (2025/1)
//   TweetFinder
//
// Gustavo de Oliveira Gimenes - 820759
// Vinícius Marto da Veiga - 821252
// Guilherme Bartoletti Oliveira - 821881
// Tiago de Paula Evangelista -

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "corpus.csv"
#define MAX_QUERY_LEN 100
#define MAX_WORD_LEN 100
#define MAX_POST_LEN 200
#define HASH_SIZE 10000
#define MAX_POSTS 2000000

// Estrutura para armazenar posts
typedef struct {
    int id;
    char content[MAX_POST_LEN];
} Post;

// Variáveis globais
Post posts[MAX_POSTS];
int total_posts = 0;

// Limpar pontuações e espaços no início e fim das postagens
void clean_content(char* str) {
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

// Carregar posts do arquivo
void load_posts(const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    char line[500];
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "docID,") != NULL) continue;   // Ignorar cabeçalho

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
            clean_content(content);
            strcpy(posts[total_posts].content, content);
            // process_post(total_posts, content);
            total_posts++;
        }
    }
    fclose(file);
}

int main()
{
    load_posts(FILE_NAME);      
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

        // printf("%s\n", query);
        // for (int i=0; i<10; i++) printf("%d | %s\n", posts[i].id, posts[i].content);
    }

    return 0;
}