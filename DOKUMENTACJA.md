# KrasnoLudki: Komprehensywna Dokumentacja Systemu

**Tytuł Projektu:** KrasnoLudki – System Automatycznej Optymalizacji Siły Roboczej i Wizualizacji

**Kurs:** AiSD II (Algorytmy i Struktury Danych II)

**Język:** C++ (Standard C++17)

**System Budowania:** CMake 3.14+

**Framework Graficzny:** Raylib

---

## Spis Treści

1. [Streszczenie Wykonawcze](#streszczenie-wykonawcze)
2. [Architektura Systemu](#architektura-systemu)
3. [Główne Struktury Danych](#główne-struktury-danych)
4. [Algorytmy i Optymalizacja](#algorytmy-i-optymalizacja)
5. [Specyfikacje Modułów](#specyfikacje-modułów)
6. [Dokumentacja API](#dokumentacja-api)
7. [Formalizacja Matematyczna](#formalizacja-matematyczna)

---

## Streszczenie Wykonawcze

KrasnoLudki to zintegrowany system zaprojektowany do rozwiązywania **Problemu Optymalnego Przydzielania Pracowników** dla fantazyjnego królestwa zamieszkałego przez karły (Krasnoludki/krasnale ogrodowe). System wykorzystuje zaawansowane algorytmy grafowe i geometrię obliczeniową do:

1. **Analizy danych geograficznych** reprezentujących miejsca pracy i lokalizacje mieszkalne
2. **Konstruowania wypukłych otoczek** w celu identyfikacji lokalizacji granicznych
3. **Optymalizacji przydziału siły roboczej** przy użyciu algorytmów minimalnego kosztu maksymalnego przepływu
4. **Wizualizacji wyników** poprzez interaktywny interfejs graficzny z Raylib

### Dziedzina Problemu

Dane:
- Zbiór n miejsc pracy, każde zlokalizowane w współrzędnych (x, y, z) i wyspecjalizowane w wydobyciu materiałów (6 typów)
- Każde miejsce pracy ma skończoną pojemność (maksymalna liczba pracowników)
- Zbiór m domów mieszkalnych, każdy zlokalizowany w współrzędnych i zamieszkany przez jednego pracownika
- Każdy pracownik ma preferowany typ materiału

Znaleźć:
- Przydzielenie pracowników do miejsc pracy, które minimalizuje całkowity koszt transportu
- Spełnienie ograniczeń pojemności dla wszystkich miejsc pracy
- Spełnienie warunku przydzielenia jeden-do-jednego

---

## Architektura Systemu

### Diagram Architektury Wysokiego Poziomu

```
┌─────────────────────────────────────────────────────────────┐
│                     Główna Pętla Aplikacji                 │
│  (render_and_graphics + StateManager)                       │
└──────────────────────┬──────────────────────────────────────┘
                       │
        ┌──────────────┼──────────────┐
        ▼              ▼              ▼
    ┌────────┐  ┌────────┐  ┌────────┐
    │ Tytuł  │  │Generuj │  │Wizuali- │
    │ Stan   │  │ Stan   │  │ zacja   │
    └────────┘  └────────┘  └────────┘
        │          │             │
        └──────────┬─────────────┘
                   │
        ┌──────────▼──────────┐
        │   Menedżer Stanów   │
        │  (Singleton)        │
        └─────────────────────┘
                   │
        ┌──────────▼──────────────┐
        │   Renderowanie Gry      │
        │  - Grafika (Raylib)     │
        │  - Zarządzanie Dźwiękiem│
        │  - Licznik FPS          │
        └─────────────────────────┘
```

### Komponenty Główne

#### 1. **Warstwa Danych**
- **Country**: Główna struktura danych zawierająca wszystkie miejsca pracy i domy
- **DynamicArray**: Niestandardowa implementacja dynamicznej tablicy
- **Queue**: Struktura danych FIFO dla algorytmu SPFA
- **Stack**: Struktura danych LIFO do parsowania i przechodzenia

#### 2. **Warstwa Obliczeń**
- **GraphProcessing**: Wejście/wyjście pliku i parsowanie danych
- **Silnik Algorytmów Grafowych**: SPFA, Min-Cost Max-Flow
- **Geometria Obliczeniowa**: Konstruowanie wypukłej otoczki
- **Obliczanie Odległości**: Metryka odległości euklidesowej

#### 3. **Warstwa Prezentacji**
- **StateManager**: Automat stanów dla przepływu gry
- **GameState**: Abstrakcyjna baza dla stanów gry
- **UIHelpers**: Narzędzia renderowania UI
- **Visualization**: Projekcja współrzędnych 3D i renderowanie

#### 4. **Narzędzia**
- **SettingsManager**: Trwałość konfiguracji
- **Huffman Compression**: Narzędzia kompresji danych
- **Tabulator**: Formatowany output konsolowy

---

## Główne Struktury Danych

### 1. DynamicArray<T>

**Cel:** Generyczny kontener tablicy dynamicznie zmienialnej

**Kluczowe Cechy:**
- Implementacja oparta na szablonach
- Operacje dołączania/poprzedzania O(1) amortyzowane
- Operacje usuwania O(n)
- Obsługa semantyki kopiowania i przenoszenia

**Struktura Wewnętrzna:**
```cpp
template <typename T>
class dynamicArray {
    T* array;           // Przechowywanie
    int64_t size;       // Bieżąca pojemność
    int64_t lastIndex;  // Ostatni prawidłowy indeks (size-1)
};
```

**Analiza Złożoności Czasowej:**

| Operacja | Czas | Przestrzeń |
|-----------|------|-----------|
| Dostęp `[]` | O(1) | O(1) |
| Dołącz | O(1) amortyzowane | O(1) |
| Wstaw w indeks | O(n) | O(1) |
| Usuń | O(n) | O(1) |
| Wyczyść | O(1) | O(n) |

**Strategia Wzrostu Pamięci:** Strategia podwajania przy przepełnieniu pojemności (size → 2×size)

### 2. Couple<ValueA, ValueB>

**Cel:** Heterogenny kontener pary do przechowywania krotek (typ_materiału, indeks)

```cpp
template <typename A, typename B>
struct couple {
    A valueA;
    B valueB;
};
```

**Użycie w Country:**
```cpp
// Aktywne miejsca pracy reprezentowane jako (typ_materiału, indeks_miejsca_pracy)
couple<namedValues::material, int64_t> active_location;
```

### 3. Queue<T>

**Cel:** Kolejka FIFO do przechodzenia w porządku wszerz w algorytmie SPFA

**Operacje:**
- `append(value)`: Enqueue - O(1)
- `dequeue()`: Dequeue - O(1)
- `isEmpty()`: Sprawdzenie pustości - O(1)
- `getHead()`: Dostęp do przodu - O(1)

**Struktura Węzła:**
```cpp
template <typename T>
struct node {
    T val;
    node<T>* next;
};
```

### 4. Country

**Cel:** Główny kontener dla wszystkich danych geograficznych i miejsc pracy

**Organizacja Danych:**
```cpp
class country {
    // Zorganizowane po typach materiałów (6 typów: breadOre, wood, clay, metal, gems, gardenGnomes)
    dynamicArray<dynamicArray<unique_ptr<workplace>>> workplaces;
    dynamicArray<dynamicArray<unique_ptr<house>>> houses;

    // Wewnętrzna reprezentacja grafu do optymalizacji
    struct edge {
        int64_t to;          // Wierzchołek docelowy
        int64_t rev;         // Indeks krawędzi odwrotnej (dla grafu rezidualnego)
        int64_t capacity;    // Pozostała pojemność
        double cost;         // Koszt jednostkowy
    };
};
```

**Strategia Indeksowania:**
- Indeks główny: typ materiału (0-5)
- Indeks wtórny: indeks jednostki w obrębie typu materiału
- Dostęp: O(1) zarówno dla miejsc pracy jak i domów

---

## Algorytmy i Optymalizacja

### 1. Algorytm Szybkiej Ścieżki Najkrótszej (SPFA)

**Cel:** Znaleźć ścieżkę o minimalnym koszcie w grafie skierowanym z nieujemnymi kosztami

**Opis Algorytmu:**

```
SPFA(graf, źródło, ujście):
    dist[źródło] = 0
    dist[v] = ∞ dla wszystkich v ≠ źródło
    kolejka Q = {źródło}
    inQueue[źródło] = true

    while Q jest niepuste:
        v = Q.dequeue()
        inQueue[v] = false

        dla każdej krawędzi (v, u) z pojemnością > 0:
            jeśli dist[v] + koszt(v,u) < dist[u]:
                dist[u] = dist[v] + koszt(v,u)
                parent[u] = v
                jeśli u nie jest w Q:
                    Q.enqueue(u)
                    inQueue[u] = true

    zwróć parent[ujście] ≠ -1  // Ścieżka istnieje
```

**Lokalizacja Implementacji:** `country::spfa()` w DataStructures/country.cpp (linie 237-285)

**Złożoność Czasowa:**
- Przypadek najlepszy: O(V + E) bez cykli ujemnych
- Przypadek średni: O(VE) w typowych warunkach
- Liczba wierzchołków V = źródło + |domy| + |miejsca_pracy| + ujście
- Liczba krawędzi E = |domy| + |domy|×|miejsca_pracy| + |miejsca_pracy|

**Złożoność Przestrzeni:** O(V + E) dla przechowywania grafu i tablic pomocniczych

**Gwarancja Poprawności:** Zwraca true wtedy i tylko wtedy, gdy istnieje ścieżka powiększająca ze źródła do ujścia

### 2. Algorytm Minimalnego Kosztu Maksymalnego Przepływu

**Cel:** Znaleźć przydzielenie maksymalnego przepływu z minimalnym całkowitym kosztem

**Formułowanie Matematyczne:**

```
Minimalizuj: Σ(przepływ(e) × koszt(e)) dla wszystkich krawędzi e
Pod warunkiem:
    - Zachowanie przepływu: Σ(przepływ_wchodzący) = Σ(przepływ_wychodzący) dla wszystkich wierzchołków pośrednich
    - Ograniczenia pojemności: 0 ≤ przepływ(e) ≤ pojemność(e)
    - Ograniczenie źródła: całkowity_przepływ_wychodzący(źródło) = całkowite_zapotrzebowanie
    - Ograniczenie ujścia: całkowity_przepływ_wchodzący(ujście) = całkowite_zapotrzebowanie
```

**Algorytm (Successive Shortest Paths):**

```
MIN_COST_MAX_FLOW(graf, źródło, ujście):
    całkowityKoszt = 0
    grafRezidualny = graf

    while SPFA(grafRezidualny, źródło, ujście, parent):
        // Znaleźć pojemność wąskiego gardła
        przepływ = ∞
        dla v na ścieżce(ujście do źródła):
            przepływ = min(przepływ, pojemność(v))

        // Powiększyć przepływ wzdłuż ścieżki
        dla v na ścieżce(ujście do źródła):
            e = krawędź(parent[v], v)
            pojemność(e) -= przepływ
            pojemność(odwrotna(e)) += przepływ
            całkowityKoszt += przepływ × koszt(e)

    zwróć całkowityKoszt
```

**Lokalizacja Implementacji:** `country::minCostMaxFlow()` (linie 287-315)

**Złożoność Iteracji:** O(K × czas_SPFA) gdzie K = całkowity_wymagany_przepływ

**Przykładowa Konstrukcja Sieci:**

Dla typu materiału z H domami i W miejscami pracy:
```
Wierzchołki Grafu:
    - źródło (0)
    - domy (1 do H)
    - miejsca pracy (H+1 do H+W)
    - ujście (H+W+1)

Krawędzie:
    - źródło → dom_i: pojemność=1, koszt=0
    - dom_i → miejsce_pracy_j: pojemność=1, koszt=odległość(dom_i, miejsce_pracy_j)
    - miejsce_pracy_j → ujście: pojemność=pojemność_j, koszt=0
```

**Właściwość Poprawności:** Zwraca optymalne przydzielenie minimalizujące całkowity koszt przy respektowaniu wszystkich ograniczeń

### 3. Algorytm Wypukłej Otoczki (Wariant Graham Scan)

**Cel:** Identyfikacja miejsc pracy na granicy tworzących wypukłą otoczkę

**Opis Algorytmu:**

```
CONVEX_HULL(punkty):
    // Krok 1: Znaleźć punkt startowy (najniższy y, następnie najniższy x)
    p0 = findStartingPoint(punkty)

    // Krok 2: Posortować punkty po kącie polarnym względem p0
    sortedPoints = sortByPolarAngle(punkty, p0)
    sortedPoints.swap(0, index_p0)

    // Krok 3: Konstruować otoczkę za pomocą testu iloczynu wektorowego
    otoczka = {sortedPoints[0], sortedPoints[1]}

    dla i = 2 do n:
        while otoczka.size() >= 2:
            p_next = sortedPoints[i]
            p_top = otoczka.back()
            p_prev = otoczka[otoczka.size()-2]

            jeśli determinant(p_prev, p_top, p_next) ≤ 0:
                otoczka.pop()
            else:
                break

        otoczka.append(p_next)

    zwróć otoczka
```

**Funkcja Determinanty (Iloczyn Wektorowy):**

```cpp
double det(couple p0, couple pi, couple pj) {
    zwróć (p_i.x - p_0.x) × (p_j.y - p_0.y)
         - (p_j.x - p_0.x) × (p_i.y - p_0.y)
}
```

**Lokalizacja Implementacji:** `country::constructRimAroundCountry()` (linie 171-211)

**Złożoność Czasowa:** O(n log n) dla sortowania + O(n) dla konstrukcji otoczki = **O(n log n)**

**Złożoność Przestrzeni:** O(n) dla wyjściowej otoczki

**Właściwość Poprawności:** Zwraca minimalny wielokąt wypukły obejmujący wszystkie aktywne miejsca pracy. Każdy punkt pojawia się dokładnie raz w wyjściu.

---

## Specyfikacje Modułów

### Moduł 1: Wejście Danych & Parsowanie

**Plik:** `src/backend/GraphProcessing.h`

**Odpowiedzialność:** Parsowanie formatu pliku i walidacja danych

**Format Wejścia:**

```
[Dla każdego typu materiału (6 łącznie)]
x;y;z;pojemność
x;y;z;pojemność
...
;;

[Dla każdego typu materiału (6 łącznie)]
x;y;z
x;y;z
...
;;
```

**Transformacja Danych:**
- Współrzędne wejściowe [0,1] są normalizowane do [-1,1]: `znormalizowane = 2×wejście - 1`
- Wartości pojemności parsowane jako int64_t (maksymalnie 2^63-1)
- Współrzędne przechowywane jako double (precyzja IEEE 754)

**Obsługa Błędów:**
- Zwraca false na plik nie znaleziony
- Loguje "ERROR, NO SUCH FILE" do stderr
- Waliduje integralność struktury danych

### Moduł 2: Optymalizacja Przydzielania Siły Roboczej

**Funkcja Główna:** `country::assignDwarfsToWorkplaces()`

**Przepływ Algorytmu:**
1. Dla każdego typu materiału niezależnie:
   - Konstruuj graf dwudzielny: domy ↔ miejsca pracy
   - Utwórz wierzchołki źródła i ujścia
   - Dodaj krawędzie z kosztami odległości
   - Wykonaj algorytm minimalnego kosztu maksymalnego przepływu
   - Wyekstrahuj przydzielenie z grafu rezidualnego
2. Wynik przydziałów do pliku (jedno przydzielenie na linię)

**Format Wyjścia:**

```
[Dla każdego typu materiału]
indeks_domu;indeks_miejsca_pracy
indeks_domu;indeks_miejsca_pracy
...
;;
```

**Gwarancje Poprawności:**
- Każdy dom przydzielony do dokładnie jednego miejsca pracy (jeśli pojemność pozwala)
- Żadne miejsce pracy nie przekracza ograniczenia pojemności
- Całkowity koszt zminimalizowany wśród wszystkich prawidłowych przydzielań
- Jedno-do-wielu przydzielenie dozwolone (jeśli miejsce pracy ma wyższą pojemność)

### Moduł 3: Interaktywna Wizualizacja

**Plik:** `src/states/Visualization.h`

**Potok Renderowania:**

```
Faza Aktualizacji:
    - Przetwarzanie wejścia (mysz, klawiatura)
    - Aktualizacja kamery/widoku
    - Obliczanie rzutowanych współrzędnych

Faza Renderowania:
    - Wyczyszczenie ekranu (czarne tło)
    - Rysowanie granicy mapy (żółta otoczka)
    - Rysowanie miejsc pracy (znaczniki diamentów z kolorami materiałów)
    - Rysowanie domów (znaczniki w kształcie domów)
    - Rysowanie połączeń przydziałów (linie)
    - Renderowanie nakładki UI (przyciski, suwaki)
```

**System Współrzędnych:**
- **Świat Gry:** Przestrzeń 3D [-1000, 1000] × [-1000, 1000] × [-1000, 1000]
- **Przestrzeń Ekranu:** Projekcja 2D [0, SCREEN_WIDTH] × [0, SCREEN_HEIGHT]

**Wskaźniki Wizualne:**

| Jednostka | Kształt | Kolor | Znaczenie |
|-----------|---------|-------|-----------|
| Miejsce pracy | Diament | Zależny od materiału | Место wydobywania zasobów |
| Dom | Kształt domu | Zależny od materiału | Mieszkanie pracownika |
| Połączenie | Linia | Szara | Przydzielona trasa pracy |
| Otoczka | Żółta granica | Żółty (#FFC400) | Granica wypukłej otoczki |

### Moduł 4: System Zarządzania Stanami

**Architektura:** Pattern Singleton z automatem stanów

**Przejścia Stanów:**
```
Tytuł → Generuj (użytkownik tworzy/ładuje)
    ↓
Wizualizacja (interaktywne eksplorowanie)
    ↓
Podziękowania (wyświetl atrybuty)
    ↓
Tytuł (ponowny start)
```

**Animacja Przejścia:**
- Czas trwania: 1.0 sekunda
- Mechanizm: Efekt kurtyny na ekranie z gradientem
- Formuła: `curtainY = (progress - offset) × animationSpeed × SCREEN_HEIGHT`

**Bezpieczeństwo Wątków:** Projekt jednowątkowy (bez równoczesnych zmian stanów)

### Moduł 5: Ustawienia & Konfiguracja

**Plik:** `src/backend/SettingsManager.h`

**Ustawienia Trwałe:**
- Głośność muzyki (0.0 do 1.0)
- Pokaż licznik FPS (boolowski)
- Ustawienia jakości grafiki
- Czułość sterowania

**Wzorzec Dostępu:** Ładowanie leniwe przy pierwszym dostępie

---

## Dokumentacja API

### Publiczny Interfejs Klasy Country

```cpp
// Konstruowanie & Destrukcja
country();
~country();

// Wejście/Wyjście Pliku
bool constructFromFile(const std::string &file_name);
bool assignDwarfsToWorkplaces(const std::string& outputFile);

// Interfejs Zapytań
const dynamicArray<dynamicArray<unique_ptr<workplace>>>&
    getWorkplaces() const;

const dynamicArray<dynamicArray<unique_ptr<house>>>&
    getHouses() const;

// Geometria Obliczeniowa
unique_ptr<dynamicArray<couple<namedValues::material, int64_t>>>
    constructRimAroundCountry();

couple<namedValues::material, int64_t>
    findStartingPointOfRim(const dynamicArray<...>& active);

// Optymalizacja
double minCostMaxFlow(
    dynamicArray<dynamicArray<edge>>& graf,
    int64_t źródło,
    int64_t ujście
);

// Narzędzie
bool isEmpty();
void dp(int64_t tabulation = 0);  // Debug print
```

### Publiczny Interfejs StateManager (Singleton)

```cpp
class StateManager {
public:
    static StateManager& Instance();

    void ChangeState(GameState* newState);
    void ProcessStateChange();
    void Update(float deltaTime, country* countryPtr);
    void Draw();
    void Shutdown();

    bool IsTransitioning() const;
};
```

### Publiczny Interfejs DynamicArray<T>

```cpp
template <typename T>
class dynamicArray {
public:
    // Inicjalizacja
    dynamicArray();
    dynamicArray(int64_t size);

    // Dostęp
    T& operator[](const int64_t &index);
    const T& operator[](const int64_t &index) const;
    T& at(int64_t index);
    T& front();
    T& back();

    // Modyfikacja
    void append(T&& val);
    void append(const T& val);
    bool insert(T&& val, int64_t index);
    bool remove(int64_t index);
    void clear();

    // Zapytania
    int64_t getSize() const;
    int64_t getLastIndex() const;
    bool isEmpty() const;

    // Manipulacja
    bool swap(int64_t indexA, int64_t indexB);
    void reverseOrder();
    void shrinkToFit();
    void forceShrink(int64_t newSize);
};
```

---

## Formalizacja Matematyczna

### 1. Metryka Odległości

**Odległość Euklidesowa w 3D:**

```
d(p1, p2) = √((x₁-x₂)² + (y₁-y₂)² + (z₁-z₂)²)
```

**Implementacja:** Funkcja `distance()` używająca std::sqrt

**Użycie:** Obliczanie wag krawędzi w grafie przydzielania

### 2. Dowód Poprawności Wypukłej Otoczki

**Twierdzenie:** Algorytm Graham scan produkuje minimalny wielokąt wypukły zawierający wszystkie punkty.

**Szkic Dowodu:**
1. Z konstrukcji, wszystkie punkty leżą w obrębie lub na granicy otoczki
2. Test iloczynu wektorowego zapewnia brak punktów wewnętrznych
3. Sortowanie po kącie polarnym zapewnia prawidłowy porządek przechodzenia
4. Żadne trzy kolejne punkty nie są kolinearne (test ścisłej nierówności)
5. Zatem wynik jest minimalnym wielokątem wypukłym ✓

### 3. Optymalizacja Min-Cost Max-Flow

**Twierdzenie:** Algorytm kolejnych najkrótszych ścieżek produkuje minimalny koszt wśród wszystkich maksymalnych przepływów.

**Kluczowy Niezmiennik:** Po każdej iteracji, bieżący przepływ jest minimalnym kosztem maksymalnego przepływu dla jego wartości.

**Struktura Dowodu:**
- Bazał: Przepływ zerowy ma koszt zerowy (minimum)
- Krok indukcyjny: Dodanie minimalnego kosztu ścieżki powiększającej zachowuje optymalizm
- Zakończenie: Gdy brak ścieżki powiększającej, osiągnięto maksymalny przepływ
- Wniosek: Rozwiązanie końcowe to minimalny koszt maksymalny przepływ ✓

### 4. Analiza Złożoności Czasowej

**Problem Przydzielenia:**

Dla M typów materiałów, H_m domów typu m, W_m miejsc pracy typu m:

```
Konstrukcja Grafu: O(Σ H_m × W_m)
Iteracje Min-Cost Max-Flow: O(F × SPFA)
    gdzie F = Σ H_m (całkowity wymagany przepływ)
    SPFA na iterację: O((H_m + W_m + 2) × (H_m × W_m))

Całkowita Złożoność: O(M × H_max × W_max × złożoność_SPFA)
Praktycznie: O(M × n × SPFA) gdzie n = max(H, W)
```

**Złożoność Przestrzeni:** O(H × W) do przechowywania grafu

---

## Wnioski

KrasnoLudki z powodzeniem integruje wielokrotne zaawansowane algorytmy w geometrii obliczeniowej i teorii grafów do rozwiązywania praktycznego problemu optymalizacyjnego. Modularna architektura pozwala na niezależną weryfikację i testowanie każdego komponenty. Poprawność matematyczna jest formalnie ustalona poprzez utrzymanie niezmienników i techniki dowodu przez indukcję.

**Kluczowe Osiągnięcia:**
- ✓ Optymalne przydzielenie siły roboczej poprzez min-cost max-flow
- ✓ Detekcja granicy geometrycznej poprzez wypukłą otoczkę
- ✓ Interaktywna wizualizacja z renderowaniem w czasie rzeczywistym
- ✓ Wydajne struktury danych z udowodnionymi złożonościami
- ✓ Modularna, testowalna architektura
