# Linguagens de Programação 1 2025/2026

## Projeto Final – Mercado & Investimento (Listas em C)

O prazo de entrega é dia 8 de Junho de 2026 às 23h00.
Avaliação e atribuição do projecto é presencial no dia 11 de Junho de 2026.

## Introdução

Vais implementar um programa em C que gere um "mercado bolsista" usando obrigatoriamente listas ligadas para o seguinte:

+ Uma **lista de empresas**
+ Cada empresa tem uma **lista de cotações** (histórico)

O programa funciona em dois modos (lidos por `stdin`, sem menus interativos):

1. MODE FUNC: O avaliador automático (Pandora) envia comandos para testar as tuas funcionalidades (adicionar, listar, editar, apagar, sugerir investimentos).
2. MODE Pandora: A ser definido no futuro.

## Notas pedagógicas

Alguns comportamentos não estão totalmente especificados de propósito.
O objetivo é que o aluno utilize o feedback da plataforma Pandora para iterar e melhorar a sua solução.

+ Recomenda-se que o aluno resolva primeiro os testes `F01` a `F20`.
+ Depois avance para `F21` a `F26`.
+ Só no fim tente os testes de robustez `F27` a `F38`.
+ Os testes `E..` devem ser reservados para avaliação sem diff.

De momento o Pandora não fornece identificação dos testes, guiem se pela sua descrição dada no fim do enunciado.

## Requisitos Pedagógicos e Técnicos (Muito Importante)

Como alunos do 1.º ano, devem garantir as seguintes boas práticas:

+ **Uso Obrigatório de Listas Ligadas:** Não é permitido o uso de arrays estáticos ou alocação estática de tamanho fixo para as empresas ou cotações. Devem usar as estruturas fornecidas no final deste enunciado.
+ **Gestão de Memória (Valgrind):** O vosso programa não pode ter memory leaks. No fim da execução, toda a memória alocada com `malloc/calloc` tem de ser libertada com free.
+ **Case Sensitivity:** Os nomes dos Tickers são sensíveis a maiúsculas/minúsculas (ex: "AAA" é diferente de "aaa"). Tratem tudo exatamente como vem do ficheiro.
+ **Limites de Strings:** Assumam os seguintes tamanhos máximos: `TICKER` (10 chars), `NOME` (50 chars), `SECTOR` (30 chars).
+ **Ordenação:** A vossa lista de empresas deve estar sempre ordenada alfabeticamente pelo Ticker. A lista de cotações de cada empresa deve estar sempre ordenada por data (crescente).

## Regras de Leitura de Input (stdin)

+ Cada comando é enviado numa linha independente.
+ Não existem espaços extra no início ou fim das linhas.
+ Os argumentos são separados por um único espaço.
+ Linhas vazias devem ser ignoradas.
+ Qualquer comando inválido ou mal formatado deve produzir `ERROR 0` e terminar imediatamente o programa.
+ Após o comando `END`, o programa deve terminar normalmente.

## 1) Dados: Ficheiro de mercado

O Pandora dá-te um ficheiro `.csv` separado por ponto e vírgula (`;`):

*Linhas de empresa*

```
C;TICKER;NOME;SECTOR
```

*Linhas de cotação*

```
Q;TICKER;YYYYMMDD;OPEN;HIGH;LOW;CLOSE;VOLUME
```

(O formato da data é um número inteiro YYYYMMDD, onde Y=Ano, M=Mês, D=Dia)

*Exemplo de ficheiro* `mercado.csv`

```
C;AAA;EmpresaA;Tech
C;BBB;EmpresaB;Retail
Q;AAA;20250101;10;10;10;10;100
Q;AAA;20250102;11;11;11;11;100
Q;BBB;20250101;20;20;20;20;100
Q;BBB;20250102;19;19;19;19;100
```

Os ficheiros no formato CSV usados pelo Pandora serão disponibilizados no moodle.

### 2) MODE FUNC (Testes do Pandora)

O primeiro comando enviado pelo Pandora pelo `stdin` é o MODE, neste caso MODE FUNC:

```
MODE FUNC
... (vários comandos) ...
END
```

+ O primeiro comando deve ser obrigatoriamente MODE FUNC. Caso contrário, deve escrever ERROR 0 e terminar imediatamente o programa.

+ Qualquer texto inválido para o programa deve dar ERROR 0

+ **IMPORTANTE:** Qualquer erro implica a terminação imediata do programa.

Tu tens de garantir que o *output é exatamente igual* ao formato pedido aqui, sem "prompts" `tipo Opcao:` ou menus.

#### 2.1 LOAD – Carregar ficheiro cumulativo

```
LOAD mercado.csv
OK
```

**Regra do LOAD:** O comando `LOAD` deve **adicionar** os dados do ficheiro às listas existentes (podes receber vários LOADs).

+ Se a empresa não existir, cria-a.
+ Se não conseguir abrir o ficheiro, escreve ERROR 1.

#### 2.2 COMPANY.LIST – Listar empresas

```
COMPANY.LIST
COMPANIES 2
AAA;EmpresaA;Tech;2
BBB;EmpresaB;Retail;2
```

Formato: `ticker;nome;sector;quotes_count` (ordenado por ticker).

Se for carregada uma cotação já existente (mesmo ticker e mesma data), deve ser aplicada a mesma regra do QUOTE.ADD (desvalorização de 1%), não devendo ser criado um novo nó.

#### 2.3 QUOTE.LIST – Listar cotações

Imprime só o `day` e o `close` (com 2 casas decimais).

```
QUOTE.LIST BBB ALL
QUOTES BBB 2
20250101;20.00
20250102;19.00
```

Com intervalo de datas (inclusive):

```
QUOTE.LIST BBB RANGE 20250102 20250102
QUOTES BBB 1
20250102;19.00
```

Se o ticker não existir: `ERROR 2`.

#### 2.4 QUOTE.ADD – Adicionar cotação (Regra do Duplicado)

```
QUOTE.ADD BBB 20250103 18 18 18 18 100
OK
```

**Regra do Duplicado:** Se a empresa BBB já tem uma cotação para o dia enviado, não crias um novo nó. Em vez disso, alteras a cotação existente aplicando uma desvalorização: `close_novo = close_antigo * 0.99`.
Se a empresa não existir: `ERROR 3`.
Se os dados da empresa forem inseridos incorretamente `ERROR 3`.

#### 2.5 QUOTE.REMOVE e QUOTE.REMOVE.RANGE – Apagar

Apagar um dia:

```
QUOTE.REMOVE BBB 20250101
OK
```

Apagar intervalo (inclusive):

```
QUOTE.REMOVE.RANGE BBB 20250102 20250103
REMOVED 2
```

(Nota: Se os limites vierem ao contrário, deves trocá-los internamente e executar na mesma).

#### 2.6 COMPANY.ADD / COMPANY.EDIT / COMPANY.REMOVE

+ **COMPANY.ADD CCC EmpresaC Finance** -> Sucesso imprime `OK`. Se já existir: `ERROR 4`.
+ **COMPANY.EDIT CCC NovoNome NovoSector** -> Sucesso imprime `OK`. Se não existir: `ERROR 5`.
+ **COMPANY.REMOVE CCC** -> Sucesso imprime OK. Se não existir: ERROR 6.
    
**Regra:** Ao remover uma empresa, tens de apagar também todas as cotações dela (libertar a memória da lista interna).

#### 2.7 MARKET.PURGE.BANKRUPT – Remover "Falidas"

Se uma empresa tiver pelo menos 6 cotações no seu histórico e os últimos 6 closes (ordenados por data) forem estritamente a descer (`c1 > c2 > c3 > c4 > c5 > c6`), ela faliu.

Ao detetar isto, a verificação deve ser feita imediatamente após cada inserção de cotação (quer durante LOAD, quer em QUOTE.ADD).

Se a condição se verificar, a empresa deve ser removida imediatamente e devem ser libertadas todas as suas cotações.

```
MARKET.PURGE.BANKRUPT
REMOVED CCC
```

#### 2.8 INVEST.SUGGEST – Sugestões de Investimento

O Pandora vai pedir sugestões de compra para 5 empresas consoante um valor e uma estratégia:

```
INVEST.SUGGEST <CASH> <ID_ESTRATEGIA>
```

O algoritmo deve dividir o `<CASH>` em 5 partes iguais. Para cada uma das 5 empresas escolhidas pela estratégia, a quantidade a comprar é a divisão inteira: `qty = (CASH / 5) / ultimo_close_da_empresa`.

+ O valor de `CASH` é inteiro.
+ O valor de `close` é float.
+ A quantidade (`qty`) deve ser calculada usando divisão real e convertida para inteiro (truncada).

**Estratégias disponíveis:**

+ `ID 1` (**Top Volume**): As 5 empresas com o maior volume total acumulado (soma do volume de todo o histórico).
+ `ID 2` (**Blue Chips**): As 5 empresas com a média de valor de `close` mais alta do seu histórico.
+ `ID 3` (**Bargain Hunters**): As 5 empresas com o `close` atual (cotação mais recente) mais baixo.

(Nota: Em caso de empate na seleção das empresas, o desempate é feito por ordem alfabética do Ticker).

Exemplo (Para CASH 5000, ID 1):

```
INVEST.SUGGEST 5000 1
SUGGESTION 5
AAA;100
BBB;50
CCC;45
DDD;10
EEE;5
```

Empresas sem qualquer cotação não devem ser consideradas nas estratégias de investimento.

### 3) Avaliação

Para aprovação na disciplina é necessária uma classificação superior a 10 valores na plataforma Pandora.

A avaliação consiste em:

1) Submissão da solução na plataforma Pandora.
A plataforma irá dar uma nota de 0 a 20.
🚨Nota mínima de aprovação da disciplina exige uma graduação superior a 10 atribuída plataforma Pandora
Esta classificação não corresponde diretamente à nota final do projeto

2) Plágio
Alunos cujo trabalho submetido é detetado como plágio serão reprovados na disciplina (nota zero no projeto)

3) Alunos que não usem o código base fornecido terão nota zero.

4) Avaliação presencial

Os alunos serão avaliados no dia 11 de Junho presencialmente.

Serão pedidas 3 alterações ao projeto a serem realizadas num intervalo de 2 horas.

A nota final do projecto é NotaPandora x Numero Alterações corretas x taxa de aproveitamento.

A taxa aproveitamento é um valor entre 0.33 e 0.60 dependente do aproveitamento dos alunos nas aulas práticas e teóricas.

As taxas de aproveitamento com respetivo cálculo serão publicadas no dia 7 de Junho 2026, sendo uma combinação dos registos dos testes, quizzes e classping.

Existo então a possibilidade de nota superior 20, essas serão atribuídas o valor 20.
### 4) Código Base Fornecido (`main.c`)

Para garantirmos que usas estruturas dinâmicas e que o teu output não falha no Pandora por causa de espaços esquecidos, `deves usar obrigatoriamente` este código base para os teus structs, headers e prints:

Este ficheiro será disponibilizado no moodle..

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

### 5) Compilação

O programa deve compilar com:

```
gcc -Wall -Wextra -Werror -lc main.c
```

### 6) INDEX dos testes

Este índice serve para o aluno perceber rapidamente o objetivo de cada teste.
Como ler os nomes

+ `Fxx_...` = teste visível, com diff
+ `Exx_...` = teste escondido, sem diff
+ `Pxx_...` = testes de `MODE Pandora`

#### 1. Arranque e erros básicos

+ `F01_load_missing_file`
+ `F02_company_list_empty_market`
+ `F28_invalid_initial_mode`
+ `F29_missing_mode_with_quote_list`
+ `F30_misspelled_command_quote_lista`
+ `F31_quote_list_invalid_all_token`

#### 2. LOAD e listagens

+ `F03_load_basic_and_company_list`
+ `F04_quote_list_all_basic`
+ `F05_quote_list_range_single_day`
+ `F06_quote_list_missing_ticker`
+ `F07_load_cumulative_long_list`
+ `F33_quote_list_range_swapped_bounds_long_market`

#### 3. Empresas

+ `F08_company_add_success`
+ `F09_company_add_duplicate`
+ `F10_company_edit_success`
+ `F11_company_edit_missing`
+ `F12_company_remove_success`
+ `F13_company_remove_missing`
+ `F32_company_add_then_quote_list_zero_then_quote_add`
+ `F34_company_add_middle_of_long_list`
+ `F35_company_remove_middle_of_long_list`

#### 4. Cotações

+ `F14_quote_add_success`
+ `F15_quote_add_missing_ticker`
+ `F16_quote_add_duplicate_discount`
+ `F17_quote_remove_single_day`
+ `F18_quote_remove_range_normal`
+ `F19_quote_remove_range_swapped_bounds`
+ `F20_quote_remove_range_missing_ticker`
+ `F36_quote_add_earlier_date_insertion`
+ `F37_quote_remove_missing_day`
+ `F38_quote_add_duplicate_long_market`

#### 5. Falência

+ `F21_bankrupt_triggered_by_quote_add`
+ `F22_bankrupt_triggered_during_load`

#### 6. Investimento

+ `F23_invest_suggest_strategy_1`
+ `F24_invest_suggest_strategy_2`
+ `F25_invest_suggest_strategy_3`
+ `F26_invest_suggest_tie_breaker`

#### 7. Pandora

+ `P01_pandora_missing_market_file`
+ `P02_pandora_success_basic`

##### Agrupamento por funcionalidade**

###### LOAD / COMPANY.LIST

+ `F01, F02, F03, F07, F22`
+ `E22`

###### QUOTE.LIST

+ `F04, F05, F06, F31, F33`
+ `E31, E33`

###### COMPANY.ADD / COMPANY.EDIT / COMPANY.REMOVE

+ `F08, F09, F10, F11, F12, F13, F32, F34, F35`
+ `E10, E11, E12, E13, E32, E34, E35`

###### QUOTE.ADD

+ `F14, F15, F16, F36, F38`
+ `E14, E15, E16, E36, E38`

###### QUOTE.REMOVE / QUOTE.REMOVE.RANGE

+ `F17, F18, F19, F20, F37`
+ `E17, E18, E19, E20, E37`

###### BANKRUPT

+ `F21, F22`
+ `E21, E22`

###### INVEST.SUGGEST

+ `F23, F24, F25, F26`
+ `E23, E24`

###### Robustez / parsing

+ `F27, F28, F29, F30, F31`
+ `E27, E28, E29, E30, E31`

Notas pedagógicas

Alguns comportamentos não estão totalmente especificados de propósito.
O objetivo é que o aluno utilize o feedback da plataforma Pandora para iterar e melhorar a sua solução.

+ Recomenda-se que o aluno resolva primeiro os testes `F01` a `F20`.
+ Depois avance para `F21` a `F26`.
+ Só no fim tente os testes de robustez `F27` a `F38`.
+ Os testes `E..` devem ser reservados para avaliação sem diff.
