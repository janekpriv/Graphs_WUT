# Dokumentacja techniczna aplikacji do generowania grafów

## Spis treści

1. Wstęp
2. Struktura projektu
3. Opis plików
   - Makefile
   - graph_generator.c i graph_generator.h
   - llm.c i llm.h
   - main.c
   - user_input.c i user_input.h

## Wstęp

Ten projekt to aplikacja konsolowa służąca do tworzenia grafów. Umożliwia  interakcję z użytkownikiem w celu manipulacji strukturami grafowymi.

## Struktura projektu

Projekt składa się z następujących plików:

- `Makefile`
- `graph.c`
- `graph.h`
- `llm.c`
- `llm.h`
- `main.c`
- `user_input.c`
- `user_input.h`

## Opis plików

### Makefile

Plik `Makefile` zawiera instrukcje do kompilacji projektu. Definiuje zależności między plikami źródłowymi a wynikowym plikiem wykonywalnym, umożliwiając automatyzację procesu kompilacji.

### graph.c i graph.h

Pliki `graph.c` i `graph.h` odpowiadają za implementację oraz deklarację funkcji związanych z operacjami na grafach. Zawierają struktury danych reprezentujące graf oraz elementarne funkcje umożliwiające tworzenie grafów czy jego przeszukiwanie.

```C
typedef enum {
    UNDIRECTED,
    DIRECTED
} GraphType;

typedef struct Node {
    int id;
    int ne;
    struct Node **links;
} *Node;

typedef struct Graph {
    GraphType type;
    int n;  
    Node *nodes;
} Graph;
```

### llm.c i llm.h

Pliki `llm.c` i `llm.h` zawierają implementację oraz deklarację funkcji związanych z łączeniem i pozyskiwaniem informacji od wybranego llm, parsowania oraz wykorzystywania ich w celu tworzenia i generowania grafów za pomocą promtów

```C
Graph* parse_chat_reply(char rep[]);
char * send_query(char * payload, CURL *curl);
char * ask_llm(char *, char*);
static size_t write_callback(char *data, size_t size, size_t nmemb, void *clientp);
```

### main.c

Plik `main.c` zawiera funkcję `main`, będącą punktem wejścia do programu. Odpowiada za inicjalizację aplikacji, obsługę wejścia użytkownika oraz wywoływanie odpowiednich funkcji w zależności od poleceń użytkownika.

### user_input.c i user_input.h

Pliki `user_input.c` i `user_input.h` odpowiadają za obsługę wejścia od użytkownika. Zawierają funkcje umożliwiające pobieranie i przetwarzanie danych wprowadzanych przez użytkownika, co pozwala na interakcję z aplikacją i manipulację grafami zgodnie z poleceniami użytkownika.

## Obsługa skompilowanego programu

Po uruchomieniu programu, użytkownik zostanie zapytany o preferowany sposób generowania grafu.
- `1` generowanie za pomocą zwyklego tekstu(analizowanego przez llm)
- `2` generowanie podając dokładne właściwości.

### Generowanie grafów promptem
program wyświetli '>>>' i będzie oczekiwał inputu użytkownika, pisząc można specyfikować wszystkie specyfikacje grafu. Historia konwersacji jest zapisywana(ale tylko podczas gdy program jest uruchomiony), więc utworzone grafy można modyfikować.

### Generowanie grafów manualnie
Następnie użytkownik zostanie zapytany o to czy graf ma byc randomowy, czy stworzony manualnie
`1` - randomowy `2` - manualny
Użytkownik następnie wybiera ilość wierzchołków.
(Nie ma ograniczeń co do ilości po za tym, że podana liczba musi byc `int` większym od 0)
Ostatnia własnościa jest wybieraną przez użytkownika jest rodzaj grafu.
`1` - skierowany `2` - nieskierowany

### Dodawanie krawędzi
Aby dodać krawędzie należy w odpowiedniej kolejności wpisać 2 wierzchołki pomiędzy którymi chcemy utworzyć krawędź. np:

```
1 2
```
co będzie skutkowało utworzeniem krawędzi 1->2 w przypadku grafu skierowanego oraz 1-2 w przypadku grafu nieskierowanego.

Program posiada wbudowany mechanizm sprawdzania czy dana krawędź jest legalna i czy ilość krawędzi już nie przekroczyła limitu

jeśli użytkownik chce zakończyć dodawanie krawędzi należy wpisać:

```
-1 -1 
```
## Reprezentacja grafu
Graf reprezentowany jest poprzez listę sąsiedztwa.

dla grafu skierowanego o 3 wierzchołkach utworzonego za pomocą następujących komend:

```
0 1
1 2
0 2
```

Wynik będzie następujący:

```
0: 1 2
1: 2
2 :
```


---

Niniejsza dokumentacja ma na celu ułatwienie zrozumienia struktury oraz funkcjonalności projektu.

