# Programming Languages 1 2025/2026

## Information

+ Project developed within the scope of the Programming Languages 1 course at the faculty [ULHT](https://www.linkedin.com/school/universidade-lusofona-de-humanidades-e-tecnologias/posts/?feedView=all), taught by professor [Daniel 'Pai Mau' Silveira](https://www.linkedin.com/in/daniel-silveira-725a6551/)
+ This is not the original text; it was translated by ChatGPT based on the GPT-5.5 Instant model. The original is [Enunciado.md]()

## Final Project – Market & Investment (Linked Lists in C)

The submission deadline is June 8th, 2026 at 11:00 PM.  
Project evaluation and grading will take place in person on June 11th, 2026.

## Introduction

You will implement a C program that manages a "stock market" using mandatory linked lists for the following:

+ A **company list**
+ Each company has a **quote list** (history)

The program operates in two modes (read from `stdin`, without interactive menus):

1. FUNC MODE: The automatic evaluator (Pandora) sends commands to test your functionalities (add, list, edit, remove, suggest investments).
2. Pandora MODE: To be defined in the future.

## Pedagogical Notes

Some behaviors are intentionally not fully specified.  
The objective is for the student to use feedback from the Pandora platform to iterate and improve their solution.

+ It is recommended that the student first solves tests `F01` to `F20`.
+ Then move on to `F21` to `F26`.
+ Only at the end attempt the robustness tests `F27` to `F38`.
+ The `E..` tests should be reserved for evaluation without diff.

At the moment, Pandora does not provide test identification, so guide yourselves by the description given at the end of the statement.

## Pedagogical and Technical Requirements (Very Important)

As first-year students, you must ensure the following good practices:

+ **Mandatory Use of Linked Lists:** The use of static arrays or fixed-size static allocation for companies or quotes is not allowed. You must use the structures provided at the end of this statement.
+ **Memory Management (Valgrind):** Your program must not have memory leaks. At the end of execution, all memory allocated with `malloc/calloc` must be freed with `free`.
+ **Case Sensitivity:** Ticker names are case-sensitive (e.g. `"AAA"` is different from `"aaa"`). Handle everything exactly as it appears in the file.
+ **String Limits:** Assume the following maximum sizes: `TICKER` (10 chars), `NAME` (50 chars), `SECTOR` (30 chars).
+ **Ordering:** Your company list must always be sorted alphabetically by Ticker. Each company’s quote list must always be sorted by date (ascending).

## Input Reading Rules (`stdin`)

+ Each command is sent on an independent line.
+ There are no extra spaces at the beginning or end of lines.
+ Arguments are separated by a single space.
+ Empty lines must be ignored.
+ Any invalid or malformed command must produce `ERROR 0` and immediately terminate the program.
+ After the `END` command, the program must terminate normally.

## 1) Data: Market File

Pandora gives you a `.csv` file separated by semicolons (`;`):

*Company lines*

```text
C;TICKER;NAME;SECTOR
```

*Quote lines*

```text
Q;TICKER;YYYYMMDD;OPEN;HIGH;LOW;CLOSE;VOLUME
```

(The date format is an integer YYYYMMDD, where Y=Year, M=Month, D=Day)

*Example file* `market.csv`

```text
C;AAA;CompanyA;Tech
C;BBB;CompanyB;Retail
Q;AAA;20250101;10;10;10;10;100
Q;AAA;20250102;11;11;11;11;100
Q;BBB;20250101;20;20;20;20;100
Q;BBB;20250102;19;19;19;19;100
```

CSV files used by Pandora will be made available on Moodle.

## 2) FUNC MODE (Pandora Tests)

The first command sent by Pandora through `stdin` is the MODE, in this case FUNC MODE:

```text
MODE FUNC
... (multiple commands) ...
END
```

+ The first command must mandatory be `MODE FUNC`. Otherwise, it must print `ERROR 0` and immediately terminate the program.

+ Any invalid text for the program must output `ERROR 0`

+ **IMPORTANT:** Any error implies immediate program termination.

You must ensure that the *output is exactly the same* as the format requested here, without "prompts" like `Option:` or menus.

### 2.1 LOAD – Load cumulative file

```text
LOAD market.csv
OK
```

**LOAD Rule:** The `LOAD` command must **add** the file data to the existing lists (you may receive multiple LOADs).

+ If the company does not exist, create it.
+ If the file cannot be opened, print `ERROR 1`.

### 2.2 COMPANY.LIST – List companies

```text
COMPANY.LIST
COMPANIES 2
AAA;CompanyA;Tech;2
BBB;CompanyB;Retail;2
```

Format: `ticker;name;sector;quotes_count` (sorted by ticker).

If an already existing quote is loaded (same ticker and same date), the same `QUOTE.ADD` rule must be applied (1% devaluation), and a new node must not be created.

### 2.3 QUOTE.LIST – List quotes

Print only the `day` and the `close` (with 2 decimal places).

```text
QUOTE.LIST BBB ALL
QUOTES BBB 2
20250101;20.00
20250102;19.00
```

With date range (inclusive):

```text
QUOTE.LIST BBB RANGE 20250102 20250102
QUOTES BBB 1
20250102;19.00
```

If the ticker does not exist: `ERROR 2`.

### 2.4 QUOTE.ADD – Add quote (Duplicate Rule)

```text
QUOTE.ADD BBB 20250103 18 18 18 18 100
OK
```

**Duplicate Rule:** If company BBB already has a quote for the given day, do not create a new node. Instead, modify the existing quote by applying a devaluation: `new_close = old_close * 0.99`.
If the company does not exist: `ERROR 3`.
If the company data is inserted incorrectly: `ERROR 3`.

### 2.5 QUOTE.REMOVE and QUOTE.REMOVE.RANGE – Delete

Delete a day:

```text
QUOTE.REMOVE BBB 20250101
OK
```

Delete range (inclusive):

```text
QUOTE.REMOVE.RANGE BBB 20250102 20250103
REMOVED 2
```

(Note: If the limits are reversed, you must swap them internally and execute normally.)

### 2.6 COMPANY.ADD / COMPANY.EDIT / COMPANY.REMOVE

+ **COMPANY.ADD CCC CompanyC Finance** -> On success prints `OK`. If it already exists: `ERROR 4`.
+ **COMPANY.EDIT CCC NewName NewSector** -> On success prints `OK`. If it does not exist: `ERROR 5`.
+ **COMPANY.REMOVE CCC** -> On success prints `OK`. If it does not exist: `ERROR 6`.
    
**Rule:** When removing a company, you must also delete all its quotes (free the memory of the internal list).

### 2.7 MARKET.PURGE.BANKRUPT – Remove "Bankrupt" Companies

If a company has at least 6 quotes in its history and the last 6 closes (sorted by date) are strictly descending (`c1 > c2 > c3 > c4 > c5 > c6`), it went bankrupt.

When this is detected, the check must be performed immediately after each quote insertion (both during LOAD and QUOTE.ADD).

If the condition is met, the company must be immediately removed and all its quotes must be freed.

```text
MARKET.PURGE.BANKRUPT
REMOVED CCC
```

### 2.8 INVEST.SUGGEST – Investment Suggestions

Pandora will request purchase suggestions for 5 companies according to an amount and a strategy:

```text
INVEST.SUGGEST <CASH> <STRATEGY_ID>
```

The algorithm must divide `<CASH>` into 5 equal parts. For each of the 5 companies selected by the strategy, the quantity to buy is the integer division: `qty = (CASH / 5) / company_last_close`.

+ The `CASH` value is an integer.
+ The `close` value is a float.
+ The quantity (`qty`) must be calculated using real division and converted to integer (truncated).

**Available Strategies:**

+ `ID 1` (**Top Volume**): The 5 companies with the highest accumulated total volume (sum of the volume of the entire history).
+ `ID 2` (**Blue Chips**): The 5 companies with the highest average `close` value in their history.
+ `ID 3` (**Bargain Hunters**): The 5 companies with the lowest current `close` (most recent quote).

(Note: In case of a tie when selecting companies, the tie-breaker is alphabetical order of the Ticker).

Example (For CASH 5000, ID 1):

```text
INVEST.SUGGEST 5000 1
SUGGESTION 5
AAA;100
BBB;50
CCC;45
DDD;10
EEE;5
```

Companies without any quotes must not be considered in investment strategies.

## 3) Evaluation

To pass the course, a grade higher than 10 is required on the Pandora platform.

The evaluation consists of:

1) Submission of the solution on the Pandora platform.  
The platform will assign a grade from 0 to 20.  
🚨The minimum passing grade for the course requires a score higher than 10 assigned by the Pandora platform.  
This grade does not directly correspond to the final project grade.

2) Plagiarism  
Students whose submitted work is detected as plagiarism will fail the course (zero grade on the project).

3) Students who do not use the provided base code will receive a zero.

4) In-person evaluation

Students will be evaluated in person on June 11th.

3 modifications to the project will be requested to be completed within a 2-hour interval.

The final project grade is:

```text
PandoraGrade x NumberOfCorrectChanges x performance_rate
```

The performance rate is a value between 0.33 and 0.60 depending on the students’ performance in practical and theoretical classes.

The performance rates and their respective calculation will be published on June 7th, 2026, being a combination of test records, quizzes, and classping.

There is therefore the possibility of a grade higher than 20; these will be assigned the value 20.

## 4) Provided Base Code (`main.c`)

To ensure that you use dynamic structures and that your output does not fail on Pandora because of forgotten spaces, `you must mandatory use` this base code for your structs, headers, and prints:

This file will be made available on Moodle.

```c
#ifndef MARKET_H
#define MARKET_H

#define MAX_TICKER 11 // 10 chars + \0
#define MAX_NAME 51   // 50 chars + \0
#define MAX_SECTOR 31 // 30 chars + \0

/* --- REQUIRED STRUCTURES --- */

// Quote list node
typedef struct QuoteNode {
    int day; // YYYYMMDD format
    float open;
    float high;
    float low;
    float close;
    int volume;
    struct QuoteNode* next;
} QuoteNode;

// Company list node
typedef struct CompanyNode {
    char ticker[MAX_TICKER];
    char name[MAX_NAME];
    char sector[MAX_SECTOR];
    int quotes_count;
    QuoteNode* quotes_head; // Head of the linked quote list
    struct CompanyNode* next;
} CompanyNode;

// Main Market structure
typedef struct {
    CompanyNode* companies_head; // Head of the linked company list
    int company_count;
} Market;


/* --- PRINTF MACROS FOR PANDORA --- */
// Use these macros in your .c code to ensure tests do not fail because of formatting!

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

## 5) Compilation

The program must compile with:

```bash
gcc -Wall -Wextra -Werror -lc main.c
```

## 6) Test INDEX

This index helps the student quickly understand the objective of each test.

### How to read the names

+ `Fxx_...` = visible test, with diff
+ `Exx_...` = hidden test, without diff
+ `Pxx_...` = `MODE Pandora` tests

### 1. Startup and basic errors

+ `F01_load_missing_file`
+ `F02_company_list_empty_market`
+ `F28_invalid_initial_mode`
+ `F29_missing_mode_with_quote_list`
+ `F30_misspelled_command_quote_lista`
+ `F31_quote_list_invalid_all_token`

### 2. LOAD and listings

+ `F03_load_basic_and_company_list`
+ `F04_quote_list_all_basic`
+ `F05_quote_list_range_single_day`
+ `F06_quote_list_missing_ticker`
+ `F07_load_cumulative_long_list`
+ `F33_quote_list_range_swapped_bounds_long_market`

### 3. Companies

+ `F08_company_add_success`
+ `F09_company_add_duplicate`
+ `F10_company_edit_success`
+ `F11_company_edit_missing`
+ `F12_company_remove_success`
+ `F13_company_remove_missing`
+ `F32_company_add_then_quote_list_zero_then_quote_add`
+ `F34_company_add_middle_of_long_list`
+ `F35_company_remove_middle_of_long_list`

### 4. Quotes

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

### 5. Bankruptcy

+ `F21_bankrupt_triggered_by_quote_add`
+ `F22_bankrupt_triggered_during_load`

### 6. Investment

+ `F23_invest_suggest_strategy_1`
+ `F24_invest_suggest_strategy_2`
+ `F25_invest_suggest_strategy_3`
+ `F26_invest_suggest_tie_breaker`

### 7. Pandora

+ `P01_pandora_missing_market_file`
+ `P02_pandora_success_basic`

#### Grouping by functionality

##### LOAD / COMPANY.LIST

+ `F01, F02, F03, F07, F22`
+ `E22`

##### QUOTE.LIST

+ `F04, F05, F06, F31, F33`
+ `E31, E33`

##### COMPANY.ADD / COMPANY.EDIT / COMPANY.REMOVE

+ `F08, F09, F10, F11, F12, F13, F32, F34, F35`
+ `E10, E11, E12, E13, E32, E34, E35`

##### QUOTE.ADD

+ `F14, F15, F16, F36, F38`
+ `E14, E15, E16, E36, E38`

##### QUOTE.REMOVE / QUOTE.REMOVE.RANGE

+ `F17, F18, F19, F20, F37`
+ `E17, E18, E19, E20, E37`

##### BANKRUPT

+ `F21, F22`
+ `E21, E22`

##### INVEST.SUGGEST

+ `F23, F24, F25, F26`
+ `E23, E24`

##### Robustness / parsing

+ `F27, F28, F29, F30, F31`
+ `E27, E28, E29, E30, E31`

## Pedagogical Notes

Some behaviors are intentionally not fully specified.  
The objective is for the student to use feedback from the Pandora platform to iterate and improve their solution.

+ It is recommended that the student first solves tests `F01` to `F20`.
+ Then move on to `F21` to `F26`.
+ Only at the end attempt the robustness tests `F27` to `F38`.
+ The `E..` tests should be reserved for evaluation without diff.