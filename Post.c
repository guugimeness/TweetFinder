#include "Post.h"

#define TIME_LIMIT 5

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
    insereSet(node->offsets, offset);
}

// Processa o post, adicionando ele na HashTable
void processa_post(int index, char* content) {
    char* palavra = strtok(content, " ,.;!?()[]{}<>\"'\t\n\r");
    while (palavra) {
        limpa_post(palavra);    // Remove pontuação nas pontas

        for (int i = 0; palavra[i]; i++)
            palavra[i] = tolower(palavra[i]);       // Normaliza para minúsculas

        if (strlen(palavra) > 0) {
            Node* existente = buscaHashTable(Hash, palavra);
            if (existente) {
                adiciona_offset(existente, posts[index].offset);
            } else {
                Node* novo = malloc(sizeof(Node));
                novo->key = strdup(palavra);
                novo->offsets = criaSet();
                adiciona_offset(novo, posts[index].offset);
                novo->next = NULL;
                insereHashTable(Hash, novo);
            }
        }
        palavra = strtok(NULL, " ,.;!?()[]{}<>\"'\t\n\r");
    }
}

// Carrega os posts do arquivo
void carrega_posts(const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    char line[500];
    long offset = 0;
    clock_t inicio = clock();

    while (fgets(line, sizeof(line), file))
    {

        // Controle do tempo para processar o arquivo -> notificar o usuário
        clock_t agora = clock();
        double tempo_decorrido = (double)(agora - inicio) / CLOCKS_PER_SEC;

        if (tempo_decorrido > TIME_LIMIT) {
            printf("Processamento do arquivo está demorando mais do que o esperado.\nAguarde...\n");
            inicio = agora;
        }

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

// Imprime as postagens do set resultante acessando o arquivo
void imprime_resultados(Set* result, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return;
    }

    if (result == NULL || tamanhoSet(result) == 0) {
        printf("Nenhum resultado encontrado.\n");
    } else {
        char line[500];
        int offset;

        for (beginSet(result); !endSet(result); nextSet(result)) {
            getItemSet(result, &offset);
            fseek(file, offset, SEEK_SET);
            if (fgets(line, sizeof(line), file)) {
                printf("→ %s", line);
            }
        }
    }

    fclose(file);
}