# MercInvest Worklog

## Sobre

O objetivo deste workolg é simples, registar tudo o que seja relacionado com o projeto:

+ Processo de criação
+ Pensamentos
+ Dificuldades encontradas
+ Soluções encontradas
+ Aprendizagens

Desta maneira qualquer um que está a ver o projeto e/ou a tentar perceber por que raio é que o codigo está tão mau pode meter-se na minha cabeça e perceber o porque.

Assima de tudo é uma boa maneira de eu voltar atrás e perceber o quão eu progredi (ou não 😬).

Irei registar tudo aqui em portugês e posteriormente traduzir para inglês, desta maneira, mesmo orgulhando me de falar bem iglês, o processo de escrita fica mais rapido e gostaria de que no final estivesse em ingles de modo a mais gente poder ver o projeto.

---

## Inicio

### Criação do repositorio:
Antes de tudo é necessario criar um repositorio, assim feito criei três documentos:
+ [README.md](/README.md)
+ [Worklog.md](/Worklog.md)
+ [Enunciado.md](/Enunciado.md)

**README.md**

No README.md irá ficar uma breve introdução do projeto, como executá-lo e outras informaçoes relevantes.

**Worklog.md**

É mesmo necessario explicar? :/

**Enunciado.md**

O enunciado do projeto dado pelo professor, de modo a qualquer um que veja este projeto possa tenta-lo ou perceber o tipo de restriçoes que me foram impostas na criaçao desta aplicaçao.

Criados estes 3 ficheiros está na hora de os preencher. O [README](/README.md) tenciono apenas preencher no final do projeto de modo a ficar tudo bem estruturado. O [Worklog.md](/Worklog.md) vai ser preenchido ao longo do desenvolvimento, logo comecei pelo [Enunciado.md](/Enunciado.md)

#### Criação do Enunciado.md

Peguei no enunciado dado pelo professor e colei no ficheiro, é o que eu gostaria de dizer, no entanto como apenas da para copiar texto e nao estava com intençoes de fazer webscraping no [Pandora](https://pandora.onthewifi.com) (a aplicaçao criada para avaliaçao de codigo em C)

Tive de colar o texto, e utilizando markdown estruturei tudo direitinho de modo a ser bem estruturado e consequentemente bem estruturado.

Para aprender como estelizar e organizar informação em MarkDown consultei o GitHub Doc [Basic writing and formatting syntax](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax) uma vez que já estava enferrujado. Tenho a dizer que agoro escrever em ficheiros md, simples e bem estruturado.

#### Repositorio em Inglês/Português

Feito isto eu ía traduzir o enunciado para inglês, no entanto tive um plano.

De modo a não ter uma decomentação em portugês e uma em inglês no mesmo sitio, tornando muito confusa a navegaçao, tenciono fazer dois branches, o main branch vai estar em ingles, uma vez que e considerada a linguagem internacional e depois crio um branch chamado pt / pt translation (vou pensar no nome) em que a documentaçao vai estar na minha lingua nativa. Esta informaçao vai estar no [README](/README.md) de modo a que qualquer portuges saiba que pode ler isto sem ter de pagar cursos caros de ingles.

### Criação do ambiente de desenvolvimento

Tendo o meu repositorio criado, vamos fazer o mais importante, o projeto.

Abri o codespace apelidado de expert-waddle:

![expert-waddle](images/expert_waddle.png)

> representação do chat gpt

#### Extenções utilizadas

De seguida adicionei todas as extenções que costumo usar na criação de programas em C:.

+ [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
+ [C/C++ DevTools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpp-devtools)
+ [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)
+ [C/C++ Themes](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-themes)
+ [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
+ [Makefile Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.makefile-tools)
+ [C/C++ Project Generator](https://marketplace.visualstudio.com/items?itemName=danielpinto8zz6.c-cpp-project-generator)

#### Criação da estrutura do projeto

Tendo as extenções necessarias no meu codespace, era hora de criar a estrutura do projeto, tranalho esse facilitado pela extenção [C/C++ Project Generator](https://marketplace.visualstudio.com/items?itemName=danielpinto8zz6.c-cpp-project-generator) que utilizando o Quick Access do VS code e escrevendo `> Create C project` ele gera automaticamento a estrutura correta para um projeto nesta linguagem.

#### Codigo inicial

Como dito no enunciado, é obrigatorio utilizar o codigo dado pelo professor, de modo a que todos os alunos usem a mesma estrutura de lista ligada. Alem disso, uma vez que o pandora e muito piquinhas com os prints, para verificar se estao corretos, e nos dados uns macros para nos facilitar.

```
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
```



---

