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
#include "Token.h"
#include "Set.h"
#include "Post.h"

#define FILE_NAME "corpus-reduzido.csv"
#define MAX_QUERY_LEN 100
#define MAX_WORD_LEN 100
#define HASH_SIZE 10000
#define MAX_POSTS 2000000
#define TABLE_SIZE 12289

// Variáveis globais
Post posts[MAX_POSTS];
HashTable *Hash;
int total_posts = 0;

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
        query[strcspn(query, "\n")] = '\0'; 

        if (strcmp(query, "help") == 0)
        {
            printf("\n             Bem-vindo ao Tweet Finder - seu buscador de tweets\n \
            Você pode digitar uma lista de palavras e será exibido todas as postagens que contém as palavras da lista.\n \
            Você pode usar os operadores AND e OR para NOT para compor a sua busca.\n \
            Você deve digitar a lista de palavras buscadas em minúsculo, e os operadores em maiúsculo.\n \
            Exemplo: (usa AND NOT(winner)) OR (brazil AND carnaval)\n" );
            continue;
        }

        if (strcmp(query, "sair") == 0) break;

        // Tokeniza a querry do usuário
        token tokens[MAX_QUERY_LEN];
        int n = tokeniza(query, tokens, MAX_QUERY_LEN);

        // Tokens que são palavras recebem seu set vindo da Hash
        associaSetsAosTokens(tokens, n, Hash);

        // Sets dos tokens sofrem as operações da querry
        Set* resultado = avaliaExpressao(tokens, n);
        
        imprime_resultados(resultado, FILE_NAME);
    }

    return 0;
}