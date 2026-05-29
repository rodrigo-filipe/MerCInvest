#ifndef MARKET_H
#define MARKET_H

#define MAX_TICKER 11 // 10 chars + \0
#define MAX_NAME 51   // 50 chars + \0
#define MAX_SECTOR 31 // 30 chars + \0

/* --- ESTRUTURAS OBRIGATÓRIAS --- */

// Nó da lista de cotações
typedef struct QuoteNode {
    int day; // Formato YYYYMMDD
    float open;
    float high;
    float low;
    float close;
    int volume;
    struct QuoteNode* next;
} QuoteNode;

// Nó da lista de empresas
typedef struct CompanyNode {
    char ticker[MAX_TICKER];
    char name[MAX_NAME];
    char sector[MAX_SECTOR];
    int quotes_count;
    QuoteNode* quotes_head; // Cabeça da lista ligada de cotações
    struct CompanyNode* next;
} CompanyNode;

// Estrutura principal do Mercado
typedef struct {
    CompanyNode* companies_head; // Cabeça da lista ligada de empresas
    int company_count;
} Market;


/* --- MACROS DE PRINTF PARA O PANDORA --- */
// Usa estas macros no teu código .c para garantir que os testes não falham por causa de formatação!

#define PRINT_OK()                  printf("OK\n")
#define PRINT_ERROR(code)           printf("ERROR %d\n", code)
#define PRINT_ERROR_PANDORA()       printf("ERROR PANDORA\n")
#define PRINT_COMPANIES_HEADER(n)   printf("COMPANIES %d\n", n)
#define PRINT_COMPANY(t, n, s, c)   printf("%s;%s;%s;%d\n", t, n, s, c)
#define PRINT_QUOTES_HEADER(t, n)   printf("QUOTES %s %d\n", t, n)
#define PRINT_QUOTE(d, c)           printf("%08d;%.2f\n", d, c)
#define PRINT_REMOVED(n)            printf("REMOVED %d\n", n)
#define PRINT_BANKRUPT(n)           printf("REMOVED %s\n", n)
#define PRINT_SUGGEST_HEADER(n)     printf("SUGGESTION %d\n", n)
#define PRINT_SUGGESTION(t, q)      printf("%s;%d\n", t, q)

#endif