# 🛤️ LEARNING PATH: Pahami SDA Langkah demi Langkah

Panduan ini memandu Anda memahami setiap konsep dengan kode nyata dan eksekusi real.

---

## STEP 1: Pahami GRAPH (5 menit)

### Teori
Graph = Network. Vertex = node. Edge = connection.
Adjacency list = map yang menyimpan tetangga setiap vertex.

### Kode di `cpp_core/main.cpp:30-63`
```cpp
class Graph {
private:
  unordered_map<string, vector<string>> adjacency_list;

public:
  void addEdge(const string &from, const string &to) {
    adjacency_list[from].push_back(to);
    adjacency_list[to].push_back(from);
  }
};
```

### Visualisasi
```
adjacency_list = {
  "Jakarta": ["Singapura", "Tokyo"],
  "Singapura": ["Jakarta", "Dubai", "Sydney"],
  "Dubai": ["Singapura", "Paris"],
  "Paris": ["Dubai", "Tokyo"],
  "Tokyo": ["Jakarta", "Paris", "Sydney"],
  "Sydney": ["Singapura", "Tokyo"]
}
```

### Eksekusi Manual
```
addEdge("Jakarta", "Singapura"):
  ✓ adjacency_list["Jakarta"].push("Singapura")
  ✓ adjacency_list["Singapura"].push("Jakarta")

Hasil: Kedua arah terhubung (undirected graph)
```

### Quiz
Q: Berapa edge di graph ini?
A: 7 edge (Jakarta-Singapura, Singapura-Dubai, Dubai-Paris, Jakarta-Tokyo, Tokyo-Paris, Singapura-Sydney, Sydney-Tokyo)

---

## STEP 2: Pahami SEARCHING (5 menit)

### Teori
Linear search = cek satu per satu sampai ketemu.

### Kode di `cpp_core/main.cpp:141-150`
```cpp
string searchCity(const string &keyword) const {
  vector<string> cities = getUniqueCities();
  
  for (const string &city : cities) {
    if (city == keyword) {
      return "Searching ditemukan: " + city;
    }
  }
  return "Searching tidak menemukan kota: " + keyword;
}
```

### Eksekusi Manual: searchCity("Tokyo")
```
cities = [Jakarta, Singapura, Dubai, Paris, Tokyo, Sydney]

Iterasi 1: city = "Jakarta",    keyword = "Tokyo"? NO
Iterasi 2: city = "Singapura",  keyword = "Tokyo"? NO
Iterasi 3: city = "Dubai",      keyword = "Tokyo"? NO
Iterasi 4: city = "Paris",      keyword = "Tokyo"? NO
Iterasi 5: city = "Tokyo",      keyword = "Tokyo"? YES ✓
Return: "Searching ditemukan: Tokyo"

Kompleksitas: O(5) operations
```

### Quiz
Q: Worst case complexity?
A: O(6) - jika kota tidak ada atau di akhir daftar

---

## STEP 3: Pahami SORTING (10 menit)

### Teori
Selection sort = cari elemen terkecil, tukar, ulangi.

### Kode di `cpp_core/main.cpp:118-134`
```cpp
static void selectionSort(vector<string> &arr) {
  const size_t n = arr.size();
  
  for (size_t i = 0; i + 1 < n; ++i) {
    size_t min_index = i;
    
    for (size_t j = i + 1; j < n; ++j) {
      if (arr[j] < arr[min_index]) {
        min_index = j;
      }
    }
    
    if (min_index != i) {
      std::swap(arr[i], arr[min_index]);
    }
  }
}
```

### Eksekusi Manual: Sort [Tokyo, Jakarta, Dubai, Paris, Singapura]
```
BEFORE: [Tokyo, Jakarta, Dubai, Paris, Singapura]

i=0: Cari min dari index 0-5
     Bandingkan: Tokyo vs Jakarta vs Dubai vs Paris vs Singapura
     min = Dubai (index 2)
     SWAP: [Dubai, Jakarta, Tokyo, Paris, Singapura]

i=1: Cari min dari index 1-5
     Bandingkan: Jakarta vs Tokyo vs Paris vs Singapura
     min = Jakarta (index 1)
     NO SWAP: [Dubai, Jakarta, Tokyo, Paris, Singapura]

i=2: Cari min dari index 2-5
     Bandingkan: Tokyo vs Paris vs Singapura
     min = Paris (index 3)
     SWAP: [Dubai, Jakarta, Paris, Tokyo, Singapura]

i=3: Cari min dari index 3-5
     Bandingkan: Tokyo vs Singapura
     min = Singapura (index 4)
     SWAP: [Dubai, Jakarta, Paris, Singapura, Tokyo]

i=4: Loop selesai (i+1 >= n)

AFTER:  [Dubai, Jakarta, Paris, Singapura, Tokyo] ✓ SORTED A-Z

Total operations: 4 iterasi luar × ~5 perbandingan = ~20 ops
Kompleksitas: O(n²) = O(36) worst case
```

### Quiz
Q: Kenapa disebut O(n²)?
A: Karena ada nested loop: outer loop n kali, inner loop rata-rata n/2 kali = n × n/2 ≈ n²

---

## STEP 4: Pahami BFS (15 menit)

### Teori
BFS = breadth-first = level per level. Gunakan QUEUE (FIFO).
Menemukan shortest path.

### Kode di `cpp_core/main.cpp:168-216`
```cpp
string BFS(const string &start, const string &target) const {
  std::queue<vector<string>> q;
  unordered_set<string> visited;
  
  q.push({start});
  visited.insert(start);
  
  while (!q.empty()) {
    vector<string> path = q.front();
    q.pop();
    
    const string current = path.back();
    
    if (current == target) {
      return "Rute BFS ditemukan: " + joinPath(path);
    }
    
    for (const string &neighbor : adjacency_list[current]) {
      if (visited.find(neighbor) == visited.end()) {
        visited.insert(neighbor);
        vector<string> new_path = path;
        new_path.push_back(neighbor);
        q.push(new_path);
      }
    }
  }
  return "Rute tidak ditemukan.";
}
```

### Eksekusi Manual: BFS("Jakarta", "Paris")
```
GRAPH:
  Jakarta: [Singapura, Tokyo]
  Singapura: [Jakarta, Dubai, Sydney]
  Dubai: [Singapura, Paris]
  Paris: [Dubai, Tokyo]
  Tokyo: [Jakarta, Paris, Sydney]
  Sydney: [Singapura, Tokyo]

STEP 1: Initialize
  Queue: [Jakarta]
  Visited: {Jakarta}

STEP 2: Pop [Jakarta], current = "Jakarta"
  current != target, expand tetangga
  Tetangga: Singapura, Tokyo
  Queue: [Jakarta→Singapura, Jakarta→Tokyo]
  Visited: {Jakarta, Singapura, Tokyo}

STEP 3: Pop [Jakarta→Singapura], current = "Singapura"
  current != target, expand tetangga
  Tetangga baru: Dubai, Sydney (Jakarta sudah visited)
  Queue: [Jakarta→Tokyo, Jakarta→Singapura→Dubai, Jakarta→Singapura→Sydney]
  Visited: {Jakarta, Singapura, Tokyo, Dubai, Sydney}

STEP 4: Pop [Jakarta→Tokyo], current = "Tokyo"
  current != target, expand tetangga
  Tetangga baru: Paris (Jakarta, Sydney sudah visited)
  Queue: [Jakarta→Singapura→Dubai, Jakarta→Singapura→Sydney, Jakarta→Tokyo→Paris]
  Visited: {Jakarta, Singapura, Tokyo, Dubai, Sydney, Paris}

STEP 5: Pop [Jakarta→Singapura→Dubai], current = "Dubai"
  current != target, expand tetangga
  Tetangga baru: Paris (Singapura sudah visited, tapi Paris belum)
  Queue: [Jakarta→Singapura→Sydney, Jakarta→Tokyo→Paris, Jakarta→Singapura→Dubai→Paris]
  Visited: {Jakarta, Singapura, Tokyo, Dubai, Sydney, Paris}

STEP 6: Pop [Jakarta→Singapura→Sydney], current = "Sydney"
  current != target, expand tetangga
  Semua tetangga sudah visited
  Queue: [Jakarta→Tokyo→Paris, Jakarta→Singapura→Dubai→Paris]
  Visited: (no change)

STEP 7: Pop [Jakarta→Tokyo→Paris], current = "Paris"
  current == target ✓ FOUND!
  
RESULT: "Rute BFS ditemukan: Jakarta → Tokyo → Paris"

Path length: 2 hops (SHORTEST!)
Kompleksitas: O(V+E) = O(6+7) = O(13)
```

### Quiz
Q: Kenapa BFS menemukan shortest path?
A: Karena BFS eksplorasi level per level (breadth), jadi akan menemukan target di level terendah terlebih dahulu.

---

## STEP 5: Pahami DFS (15 menit)

### Teori
DFS = depth-first = sedalam mungkin. Gunakan STACK (LIFO).
Tidak dijamin shortest path, tapi eksplorasi lebih menyeluruh.

### Kode di `cpp_core/main.cpp:234-286`
```cpp
string DFS(const string &start, const string &target) const {
  vector<vector<string>> stack_paths;
  unordered_set<string> visited;
  
  stack_paths.push_back({start});
  
  while (!stack_paths.empty()) {
    vector<string> path = stack_paths.back();
    stack_paths.pop_back();
    
    const string current = path.back();
    
    if (current == target) {
      return "Rute DFS ditemukan: " + joinPath(path);
    }
    
    if (visited.find(current) != visited.end()) {
      continue;
    }
    visited.insert(current);
    
    for (auto rit = adjacency_list[current].rbegin(); 
         rit != adjacency_list[current].rend(); ++rit) {
      if (visited.find(*rit) == visited.end()) {
        vector<string> new_path = path;
        new_path.push_back(*rit);
        stack_paths.push_back(new_path);
      }
    }
  }
  return "Rute tidak ditemukan.";
}
```

### Eksekusi Manual: DFS("Jakarta", "Paris")
```
STEP 1: Initialize
  Stack: [Jakarta]
  Visited: {}

STEP 2: Pop [Jakarta], current = "Jakarta"
  visited = {} (tidak ada)
  Mark: visited = {Jakarta}
  Tetangga (reverse order): Tokyo, Singapura
  Stack: [Jakarta→Singapura, Jakarta→Tokyo]

STEP 3: Pop [Jakarta→Tokyo], current = "Tokyo" (LIFO = ambil terakhir!)
  visited = {Jakarta} (Tokyo tidak ada)
  Mark: visited = {Jakarta, Tokyo}
  Tetangga (reverse order): Sydney, Paris, Jakarta
  Stack: [Jakarta→Singapura, Jakarta→Tokyo→Jakarta(skip), Jakarta→Tokyo→Paris, Jakarta→Tokyo→Sydney]
  Setelah filter: [Jakarta→Singapura, Jakarta→Tokyo→Paris, Jakarta→Tokyo→Sydney]

STEP 4: Pop [Jakarta→Tokyo→Sydney], current = "Sydney"
  visited = {Jakarta, Tokyo} (Sydney tidak ada)
  Mark: visited = {Jakarta, Tokyo, Sydney}
  Tetangga (reverse order): Tokyo(visited), Singapura
  Stack: [Jakarta→Singapura, Jakarta→Tokyo→Paris, Jakarta→Tokyo→Sydney→Singapura]

STEP 5: Pop [Jakarta→Tokyo→Sydney→Singapura], current = "Singapura"
  visited = {Jakarta, Tokyo, Sydney} (Singapura tidak ada)
  Mark: visited = {Jakarta, Tokyo, Sydney, Singapura}
  Tetangga (reverse order): Sydney(visited), Dubai, Jakarta(visited)
  Stack: [Jakarta→Singapura, Jakarta→Tokyo→Paris, Jakarta→Tokyo→Sydney→Singapura→Dubai]

STEP 6: Pop [Jakarta→Tokyo→Sydney→Singapura→Dubai], current = "Dubai"
  visited = {Jakarta, Tokyo, Sydney, Singapura} (Dubai tidak ada)
  Mark: visited = {Jakarta, Tokyo, Sydney, Singapura, Dubai}
  Tetangga (reverse order): Paris, Singapura(visited)
  Stack: [Jakarta→Singapura, Jakarta→Tokyo→Paris, Jakarta→Tokyo→Sydney→Singapura→Dubai→Paris]

STEP 7: Pop [Jakarta→Tokyo→Sydney→Singapura→Dubai→Paris], current = "Paris"
  current == target ✓ FOUND!

RESULT: "Rute DFS ditemukan: Jakarta → Tokyo → Sydney → Singapura → Dubai → Paris"

Path length: 5 hops (TIDAK shortest!)
Kompleksitas: O(V+E) = O(6+7) = O(13) (sama dengan BFS)

⚠️ Catatan: Path lebih panjang dari BFS karena explore sedalam mungkin dulu.
```

### Quiz
Q: Kenapa DFS lebih panjang dari BFS?
A: Karena DFS eksplorasi sedalam mungkin (depth-first) sebelum mundur. Mungkin masuk "jalan buntu" sebelum menemukan target.

---

## STEP 6: Pahami BIG O (5 menit)

### Ranking
```
O(1)    = Instant (1 operation) TERCEPAT
O(log n) = Very fast (binary search)
O(n)    = Linear (6 operasi untuk 6 kota)
O(n log n) = Fast sorting (merge sort)
O(n²)   = Quadratic (36 operasi untuk 6² kota) LAMBAT!
O(2ⁿ)   = Exponential (64 operasi untuk 2⁶) SANGAT LAMBAT!
O(n!)   = Factorial (720 operasi untuk 6!) NIGHTMARE
```

### Di Proyek
```
Graph lookup (map):       O(1)
Linear search:            O(n)
BFS:                      O(n+m)
DFS:                      O(n+m)
Selection sort:           O(n²)
```

---

## ✅ Checklist Verifikasi

- [ ] Pahami Graph adjacency list
- [ ] Pahami Linear Search iterasi satu per satu
- [ ] Pahami Selection Sort dengan nested loop
- [ ] Pahami BFS dengan QUEUE (FIFO)
- [ ] Pahami DFS dengan STACK (LIFO)
- [ ] Pahami BFS menemukan shortest path
- [ ] Pahami DFS tidak dijamin shortest path
- [ ] Pahami O(n²) lebih lambat dari O(n)

---

**Selamat! Anda sudah memahami 5 core concepts dari SDA 🎉**
