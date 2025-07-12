//Potências de dois devem ser evitadas
//deve ser um número primo distante de pequenas potências de dois

// Por enquando vai ser uma lista de IDs -> substituir por SET DE RRNs
typedef struct IntList {
    long offset;
    struct IntList* next;
} IntList;

// Lista ligada para as chaves que derem colisão
typedef struct Node{
    char* key;
    IntList* ids;
    struct Node *next;
} Node;

//Definição do tipo Hash
typedef struct HashTable{
    int qtd;
    int TABLE_SIZE;
    Node **lists;  // array de ponteiros para o início de cada lista ligada
} HashTable;

/*
typedef struct{
    Node *root;
    int qtd;
}LinkedList;
*/

HashTable* criaHashTable(int TABLE_SIZE);
void liberaHash(HashTable* ha);

int valorString(char *str);

int insereHashTable(HashTable* ha, Node *no);
Node* buscaHashTable(HashTable* ha, const char* key);

/*
int buscaHash_SemColisao(HashTable* ha, int mat, Node no);
int insereHash_EnderAberto(HashTable* ha, Node *no);
int buscaHash_EnderAberto(Hash* ha, int mat, struct aluno* al);
int chaveDivisao(int chave, int TABLE_SIZE);
int chaveDobra(int chave, int TABLE_SIZE);
int chaveMultiplicacao(int chave, int TABLE_SIZE);
int sondagemLinear(int pos, int i, int TABLE_SIZE);
int sondagemQuadratica(int pos, int i, int TABLE_SIZE);
int duploHashing(int pos, int chave, int i, int TABLE_SIZE);
*/
