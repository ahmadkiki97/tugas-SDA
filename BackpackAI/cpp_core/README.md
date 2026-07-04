# Holiday Planner AI - C++ Core

Implementasi algoritma Graph, BFS, DFS, dan Selection Sort untuk pencarian rute destinasi liburan.

## Fitur

- **Graph Implementation**: Struktur data graf tidak berarah menggunakan adjacency list
- **BFS (Breadth-First Search)**: Mencari rute terpendek dengan menjelajahi level per level
- **DFS (Depth-First Search)**: Mencari rute dengan menjelajahi sedalam mungkin terlebih dahulu
- **Selection Sort**: Pengurutan kota secara alfabetis (A-Z)
- **DLL Export**: Fungsi wrapper untuk dipanggil dari Python via ctypes

## Struktur Data

### Class Graph
- `adjList`: Hash map menyimpan daftar ketetanggaan (kota -> list kota tetangga)
- `addEdge()`: Menambahkan sisi antara dua kota (bidirectional)
- `BFS()`: Pencarian rute terpendek
- `DFS()`: Pencarian rute eksplorasi mendalam
- `getUniqueCities()`: Mengambil semua kota unik dari graf

### Rute Tersedia
```
Jakarta <-> Singapura
Singapura <-> Dubai
Dubai <-> Paris
Jakarta <-> Tokyo
Tokyo <-> Paris
```

## Kompilasi

### Windows (MinGW/MSYS2)
```bash
g++ -fPIC -shared main.cpp -o core.dll
```

### Linux/Mac
```bash
g++ -fPIC -shared main.cpp -o core.so
```

## Fungsi Export (DLL API)

### `cari_rute_wrapper(asal, tujuan, algo)`
Mencari rute antara dua kota dengan algoritma tertentu.

**Parameter:**
- `asal` (const char*): Nama kota keberangkatan
- `tujuan` (const char*): Nama kota destinasi
- `algo` (const char*): "BFS" atau "DFS"

**Return:**
- String format: "Rute BFS/DFS ditemukan: Jakarta -> Singapura -> Dubai -> Paris"

### `get_all_cities_sorted_wrapper()`
Mendapatkan daftar semua kota terurut alfabetis.

**Return:**
- String format: "Dubai;Jakarta;Paris;Singapura;Tokyo"

## Contoh Penggunaan

### BFS (Breadth-First Search)
```
Input: Jakarta -> Paris (BFS)
Output: Rute BFS ditemukan: Jakarta -> Tokyo -> Paris
```
BFS menemukan rute terpendek (2 hop).

### DFS (Depth-First Search)
```
Input: Jakarta -> Paris (DFS)
Output: Rute DFS ditemukan: Jakarta -> Singapura -> Dubai -> Paris
```
DFS mengeksplorasi jalur mendalam terlebih dahulu.

## Catatan Penting

1. **Buffer Statis**: Hasil disimpan di `result_buffer[4096]` agar aman dikembalikan ke Python
2. **extern "C"**: Mencegah name-mangling agar fungsi bisa ditemukan oleh ctypes
3. **__declspec(dllexport)**: Directive Windows untuk mengekspor fungsi ke DLL
4. **Cross-Platform**: Kode mendukung Windows (strncpy_s) dan Linux/Mac (strncpy)

## Dependencies

- Tidak ada dependency eksternal (hanya library standar C++)
- `<cstring>`, `<iostream>`, `<queue>`, `<string>`, `<unordered_map>`, `<unordered_set>`, `<vector>`
