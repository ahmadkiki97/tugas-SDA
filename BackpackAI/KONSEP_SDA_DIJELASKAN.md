# 📚 Panduan Lengkap: Konsep SDA dalam BackpackAI

Dokumen ini menjelaskan di mana letak setiap konsep **Struktur Data & Algoritma** dalam proyek BackpackAI dengan studi kasus **Pencarian Rute Travel AI**.

---

## 📍 1. GRAPH (Struktur Data)

### Apa itu Graph?
Graph adalah struktur data yang terdiri dari:
- **Vertex (Node)**: Kota-kota destinasi
- **Edge**: Jalur/penerbangan yang menghubungkan dua kota
- **Adjacency List**: Cara menyimpan graph dengan hemat memori

### Letak dalam Kode

**File**: `cpp_core/main.cpp` (Baris 21-299)

```cpp
class Graph {
private:
  // VERTEX + EDGE representation
  unordered_map<string, vector<string>> adjacency_list;
  
  void addEdge(const string &from, const string &to) {
    adjacency_list[from].push_back(to);    // Jalur dari A ke B
    adjacency_list[to].push_back(from);    // Jalur dari B ke A (undirected)
  }
};
```

### Visualisasi Graph dalam Proyek

```
        ┌────────────┐
        │  JAKARTA   │
        └──────┬─────┘
               │
          ┌────┴─────┐
          │           │
    ┌─────▼──┐   ┌────▼─────┐
    │SINGAPURA│   │  TOKYO   │
    └─────┬──┘   └──────┬────┘
          │             │
    ┌─────▼──┐          │
    │ DUBAI  │◄─────────┤
    └─────┬──┘          │
          │             │
    ┌─────▼──┐   ┌──────▼────┐
    │ PARIS  │◄──┘           │
    └────────┘               │
                      ┌──────▼─────┐
                      │   SYDNEY   │
                      └────────────┘
```

### Kompleksitas Ruang
- **O(V + E)** dimana V = jumlah vertex (kota), E = jumlah edge (jalur)
- Lebih efisien dibanding adjacency matrix O(V²) untuk graph sparse

---

## 🔍 2. BFS (Breadth-First Search)

### Apa itu BFS?
Algoritma pencarian yang mengeksplorasi **level per level** (breadth-wise), seperti gelombang air yang menyebar ke samping.

### Karakteristik
✅ Menemukan **shortest path** (jalur terpendek)
✅ Optimal untuk graph unweighted
✅ Kompleksitas: **O(V + E)**
⚠️ Memerlukan banyak memori untuk menyimpan queue

### Letak dalam Kode

**File**: `cpp_core/main.cpp` (Baris 152-216)

```cpp
string BFS(const string &start, const string &target) const {
  std::queue<vector<string>> q;              // FIFO Queue
  unordered_set<string> visited;             // Tracking kota yang sudah dikunjungi
  
  q.push({start});                           // Mulai dari start
  visited.insert(start);
  
  while (!q.empty()) {
    vector<string> path = q.front();         // Ambil path paling awal (breadth-wise)
    q.pop();
    
    const string current = path.back();      // Kota terakhir dalam path
    
    if (current == target) {
      return "Rute BFS ditemukan: " + joinPath(path);
    }
    
    // Jelajahi tetangga level saat ini
    for (const string &neighbor : adjacency_list[current]) {
      if (visited.find(neighbor) == visited.end()) {
        visited.insert(neighbor);
        vector<string> new_path = path;
        new_path.push_back(neighbor);
        q.push(new_path);                    // Tambah ke queue untuk dieksplorasi nanti
      }
    }
  }
  return "Rute tidak ditemukan.";
}
```

### Contoh Eksekusi BFS: Jakarta → Paris

```
Step 1: Queue = [Jakarta]
        Visited = {Jakarta}
        
Step 2: Eksplorasi Jakarta
        Queue = [Jakarta-Singapura, Jakarta-Tokyo]
        Visited = {Jakarta, Singapura, Tokyo}
        
Step 3: Eksplorasi Jakarta-Singapura
        Queue = [Jakarta-Tokyo, Jakarta-Singapura-Dubai, Jakarta-Singapura-Sydney]
        Visited = {Jakarta, Singapura, Tokyo, Dubai, Sydney}
        
Step 4: Eksplorasi Jakarta-Tokyo
        Queue = [Jakarta-Singapura-Dubai, Jakarta-Singapura-Sydney, Jakarta-Tokyo-Paris]
        Visited = {..., Paris}
        
Step 5: Temukan target!
        HASIL: Jakarta → Tokyo → Paris (2 hop = jalur terpendek)
```

### Implementasi di Backend

**File**: `backend/main.py` (Baris 62-94)

```python
@app.get("/cari_rute")
async def cari_rute(asal: str, tujuan: str, algo: str = "BFS"):
    hasil_bytes = await asyncio.to_thread(
        cpp_library.cari_rute_wrapper,     # Panggil fungsi C++
        asal.encode('utf-8'),
        tujuan.encode('utf-8'),
        algo.encode('utf-8')               # "BFS" atau "DFS"
    )
    hasil_teks = hasil_bytes.decode('utf-8')
    return {
        "status": "success",
        "hasil_pencarian": hasil_teks
    }
```

---

## 🏔️ 3. DFS (Depth-First Search)

### Apa itu DFS?
Algoritma pencarian yang mengeksplorasi **sedalam mungkin** (depth-wise) sebelum mundur (backtrack), seperti eksplorasi gua.

### Karakteristik
✅ Menemukan **rute eksploratif** (bisa lebih panjang dari BFS)
✅ Berguna untuk topological sorting dan cycle detection
✅ Kompleksitas: **O(V + E)** sama dengan BFS
⚠️ Belum tentu menemukan shortest path

### Letak dalam Kode

**File**: `cpp_core/main.cpp` (Baris 218-286)

```cpp
string DFS(const string &start, const string &target) const {
  vector<vector<string>> stack_paths;        // LIFO Stack (menggunakan vector)
  unordered_set<string> visited;             // Tracking kota yang sudah dikunjungi
  
  stack_paths.push_back({start});            // Mulai dari start
  
  while (!stack_paths.empty()) {
    vector<string> path = stack_paths.back();      // Ambil path paling atas (LIFO)
    stack_paths.pop_back();
    
    const string current = path.back();      // Kota terakhir dalam path
    
    if (current == target) {
      return "Rute DFS ditemukan: " + joinPath(path);
    }
    
    if (visited.find(current) != visited.end()) {
      continue;                              // Skip jika sudah dikunjungi
    }
    visited.insert(current);
    
    // Jelajahi tetangga secara mendalam (reverse order untuk LIFO)
    for (auto rit = adjacency_list[current].rbegin(); 
         rit != adjacency_list[current].rend(); ++rit) {
      if (visited.find(*rit) == visited.end()) {
        vector<string> new_path = path;
        new_path.push_back(*rit);
        stack_paths.push_back(new_path);     // Tambah ke stack untuk dieksplorasi lebih dalam
      }
    }
  }
  return "Rute tidak ditemukan.";
}
```

### Contoh Eksekusi DFS: Jakarta → Paris

```
Step 1: Stack = [Jakarta]
        Visited = {}
        
Step 2: Ambil Jakarta dari stack
        Stack = [Jakarta-Singapura, Jakarta-Tokyo]
        Visited = {Jakarta}
        
Step 3: Ambil Jakarta-Tokyo (dari atas stack - LIFO)
        Stack = [Jakarta-Singapura, Jakarta-Tokyo-Paris]
        Visited = {Jakarta, Tokyo}
        
Step 4: Ambil Jakarta-Tokyo-Paris
        Stack = [Jakarta-Singapura]
        Visited = {Jakarta, Tokyo, Paris}
        
Step 5: Temukan target!
        HASIL: Jakarta → Tokyo → Paris (2 hop, tapi eksplorasi berbeda dari BFS)
```

### Perbedaan BFS vs DFS

| Aspek | BFS | DFS |
|-------|-----|-----|
| **Struktur** | Queue (FIFO) | Stack (LIFO) |
| **Jalur** | Shortest path ✓ | Tidak dijamin |
| **Memori** | Banyak (menyimpan level) | Lebih sedikit |
| **Kegunaan** | Pencarian terpendek | Eksplorasi mendalam |
| **Kompleksitas** | O(V + E) | O(V + E) |

---

## 🔤 4. SEARCHING (Linear Search)

### Apa itu Searching?
Proses mencari suatu elemen dalam kumpulan data.

### Letak dalam Kode

**File**: `cpp_core/main.cpp` (Baris 136-150)

```cpp
string searchCity(const string &keyword) const {
  vector<string> cities = getUniqueCities();
  
  // Linear Search: cek setiap kota satu per satu
  for (const string &city : cities) {
    if (city == keyword) {
      return "Searching ditemukan: " + city;
    }
  }
  return "Searching tidak menemukan kota: " + keyword;
}
```

### Kompleksitas
- **O(n)** - Linear search, memeriksa setiap elemen
- Best case: **O(1)** (ditemukan di elemen pertama)
- Worst case: **O(n)** (tidak ditemukan atau ada di akhir)

### Implementasi di Backend

**File**: `backend/main.py` (Baris 126-135)

```python
@app.get("/search")
async def search_city(keyword: str):
    hasil_bytes = await asyncio.to_thread(
        cpp_library.search_city_wrapper,
        keyword.encode('utf-8')
    )
    hasil_teks = hasil_bytes.decode('utf-8')
    return {
        "status": "success",
        "keyword": keyword,
        "hasil_pencarian": hasil_teks
    }
```

### Contoh di Frontend

**File**: `frontend/lib/main.dart`
```dart
// User mengetik keyword di search box
// Frontend mengirim ke backend: /search?keyword=Tokyo
// Backend memanggil searchCity("Tokyo")
// Hasil: "Searching ditemukan: Tokyo"
```

---

## 🔤 5. SORTING (Selection Sort)

### Apa itu Sorting?
Proses mengurutkan data dalam urutan tertentu (ascending/descending).

### Letak dalam Kode

**File**: `cpp_core/main.cpp` (Baris 109-134)

```cpp
static void selectionSort(vector<string> &arr) {
  const size_t n = arr.size();
  
  // Loop eksternal: posisi mana yang akan diisi
  for (size_t i = 0; i + 1 < n; ++i) {
    size_t min_index = i;
    
    // Loop internal: cari elemen minimum dari posisi i ke akhir
    for (size_t j = i + 1; j < n; ++j) {
      if (arr[j] < arr[min_index]) {
        min_index = j;                    // Update indeks minimum
      }
    }
    
    // Tukar elemen minimum dengan elemen di posisi i
    if (min_index != i) {
      std::swap(arr[i], arr[min_index]);
    }
  }
}
```

### Cara Kerja Selection Sort

Mengurutkan kota: [Tokyo, Jakarta, Dubai, Paris, Singapura]

```
Iterasi 0: [Tokyo, Jakarta, Dubai, Paris, Singapura]
           Cari min: Dubai
           Tukar: [Dubai, Jakarta, Tokyo, Paris, Singapura]
           ↑ sorted

Iterasi 1: [Dubai, Jakarta, Tokyo, Paris, Singapura]
           Cari min dari Jakarta ke akhir: Jakarta
           Tidak perlu tukar
           [Dubai, Jakarta, Tokyo, Paris, Singapura]
           ↑ ↑ sorted

Iterasi 2: [Dubai, Jakarta, Tokyo, Paris, Singapura]
           Cari min dari Tokyo ke akhir: Paris
           Tukar: [Dubai, Jakarta, Paris, Tokyo, Singapura]
           ↑ ↑ ↑ sorted

Iterasi 3: [Dubai, Jakarta, Paris, Tokyo, Singapura]
           Cari min dari Tokyo ke akhir: Singapura
           Tukar: [Dubai, Jakarta, Paris, Singapura, Tokyo]
           ↑ ↑ ↑ ↑ sorted

HASIL: [Dubai, Jakarta, Paris, Singapura, Tokyo]
```

### Kompleksitas
- **O(n²)** - Quadratic, cocok untuk dataset kecil (< 100 elemen)
- Space: **O(1)** - In-place sorting, tidak perlu memori tambahan
- Worst case: **n(n-1)/2** comparisons

### Implementasi di Backend

**File**: `backend/main.py` (Baris 96-121)

```python
@app.get("/cities")
async def dapatkan_kota():
    hasil_bytes = await asyncio.to_thread(
        cpp_library.get_all_cities_sorted_wrapper
    )
    hasil_teks = hasil_bytes.decode('utf-8')
    daftar_kota = hasil_teks.split(";")      # Parse hasil sorting dari C++
    
    return {
        "status": "success",
        "cities": daftar_kota                 # Daftar kota terurut A-Z
    }
```

### Contoh di Frontend

**File**: `frontend/web_frontend/index.html` atau `frontend/lib/main.dart`
```
Dropdown "Kota Asal" & "Kota Tujuan" akan menampilkan:
- Dubai
- Jakarta
- Paris
- Singapura
- Tokyo
(Terurut alfabetis karena Selection Sort)
```

---

## 📊 6. BIG O NOTATION (Analisis Kompleksitas)

### Apa itu Big O?
Notasi matematis untuk menganalisis **efisiensi algoritma** dalam hal waktu dan ruang.

### Letak dalam Kode

**File**: `cpp_core/main.cpp` (Baris 96-107)

```cpp
string getBigONotationSummary() const {
  return "Big O Notasi:\n"
         "- Simpan graph: O(V + E)\n"
         "- BFS: O(V + E)\n"
         "- DFS: O(V + E)\n"
         "- Linear Search kota: O(V)\n"
         "- Selection Sort: O(V^2)";
}
```

### Implementasi di Backend

**File**: `backend/main.py` (Baris 140-149)

```python
@app.get("/bigo")
async def get_big_o():
    hasil_bytes = await asyncio.to_thread(
        cpp_library.big_o_wrapper
    )
    hasil_teks = hasil_bytes.decode('utf-8')
    return {
        "status": "success",
        "big_o_notation": hasil_teks
    }
```

### Penjelasan Big O dalam Proyek

| Operasi | Big O | Penjelasan |
|---------|-------|-----------|
| **Graph Construction** | O(V + E) | Menambah V vertex dan E edge |
| **BFS Search** | O(V + E) | Kunjungi setiap vertex dan edge sekali |
| **DFS Search** | O(V + E) | Kunjungi setiap vertex dan edge sekali |
| **Linear Search** | O(V) | Worst case: cek semua V kota |
| **Selection Sort** | O(V²) | Dua nested loop untuk V kota |
| **Graph Lookup** | O(1) avg | Unordered_map average O(1) |

### Ranking Efisiensi (dari tercepat ke terlambat)

```
O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(2ⁿ) < O(n!)

Dalam proyek:
O(1)        <- Lookup kota dari map
O(V)        <- Linear Search
O(V+E)      <- BFS, DFS
O(V²)       <- Selection Sort (PALING LAMBAT untuk dataset besar)
```

---

## 🎯 7. STUDI KASUS: Travel AI Planning

### Skenario Real-World
**Pengguna ingin mencari rute liburan dari Jakarta ke Paris dengan menggunakan Travel Planner AI.**

### Alur Lengkap dengan Konsep SDA

```
1. USER INPUT (Frontend)
   ├─ Pilih kota asal: Jakarta
   ├─ Pilih kota tujuan: Paris
   └─ Pilih algoritma: BFS (cari rute tercepat)

2. SORTING (Sorting konsep)
   ├─ Backend request: GET /cities
   ├─ C++ jalankan getSortedCities()
   ├─ Selection Sort: O(V²) mengurutkan 5 kota
   └─ Return: [Dubai, Jakarta, Paris, Singapura, Tokyo]
   └─ Frontend tampilkan dropdown terurut A-Z

3. GRAPH CONSTRUCTION (Graph konsep)
   ├─ C++ load buildSampleGraph()
   ├─ Adjacency List:
   │  ├─ Jakarta: [Singapura, Tokyo]
   │  ├─ Singapura: [Jakarta, Dubai, Sydney]
   │  ├─ Dubai: [Singapura, Paris]
   │  ├─ Paris: [Dubai, Tokyo]
   │  ├─ Tokyo: [Jakarta, Paris, Sydney]
   │  └─ Sydney: [Singapura, Tokyo]
   └─ Space: O(V + E) = O(6 + 7) = O(13)

4. BFS SEARCH (BFS konsep)
   ├─ Backend request: GET /cari_rute?asal=Jakarta&tujuan=Paris&algo=BFS
   ├─ C++ jalankan BFS("Jakarta", "Paris")
   ├─ Queue: {Jakarta} → {Tokyo, Singapura} → Found Paris!
   ├─ Shortest path: Jakarta → Tokyo → Paris (2 hops)
   └─ Time: O(V + E) = O(6 + 7) = O(13) operations

5. SEARCHING (Searching konsep)
   ├─ User search bar: ketik "To"
   ├─ Backend request: GET /search?keyword=Tokyo
   ├─ C++ jalankan searchCity("Tokyo")
   ├─ Linear Search: O(V) iterasi untuk cek setiap kota
   └─ Return: "Searching ditemukan: Tokyo"

6. BIG O ANALYSIS (Big O konsep)
   ├─ User ingin tahu efisiensi aplikasi
   ├─ Backend request: GET /bigo
   ├─ C++ return summary semua Big O
   └─ Display: 
   │  - Sorting: O(V²) = 25 operations
   │  - BFS: O(V+E) = 13 operations (LEBIH CEPAT!)
   │  - Linear Search: O(V) = 6 operations
   └─ Insight: BFS jauh lebih efisien untuk graph searching!

7. RESPONSE TO FRONTEND (Full Cycle)
   └─ JSON Response:
      {
        "status": "success",
        "asal": "Jakarta",
        "tujuan": "Paris",
        "algo": "BFS",
        "hasil_pencarian": "Rute BFS ditemukan: Jakarta → Tokyo → Paris",
        "efficiency": {
          "algorithm": "BFS",
          "time_complexity": "O(V+E)",
          "space_complexity": "O(V)"
        }
      }
```

---

## 💡 8. MANFAAT AI dalam Context BackpackAI

### Mengapa Konsep SDA Penting untuk AI?

#### 1. **Graph untuk Knowledge Representation**
- AI perlu merepresentasikan relasi kompleks (city connections)
- Graph memungkinkan AI "memahami" koneksi antar konsep
- Digunakan di: Recommendation systems, Social networks, Knowledge graphs

#### 2. **BFS/DFS untuk Pathfinding**
- AI routing (Google Maps, Waze) menggunakan BFS untuk shortest path
- Game AI menggunakan DFS untuk strategi bermain
- Robotics menggunakan keduanya untuk navigation

#### 3. **Sorting untuk Data Organization**
- AI machine learning memerlukan data terurut untuk pelatihan
- Preprocessing data adalah 80% dari ML pipeline
- Efficient sorting → Faster model training

#### 4. **Searching untuk Information Retrieval**
- AI search engines (Google) menggunakan advanced searching
- Natural Language Processing menggunakan searching untuk text matching
- Recommendation systems mencari item serupa dengan efficient searching

#### 5. **Big O untuk AI Scalability**
- AI model dengan O(n²) complexity tidak scalable untuk big data
- Modern ML algorithms dirancang untuk O(n) atau O(n log n)
- Big O analysis adalah kunci untuk production-grade AI

### Contoh Real-World AI

```
Google Maps (Pathfinding AI)
├─ Graph: 1 juta+ intersections, 10 juta+ roads
├─ Algorithm: Dijkstra + BFS/DFS hybrid
├─ Big O: O((V + E) log V) dengan priority queue
└─ Result: Real-time route optimization untuk 2 miliar users

Netflix Recommendation (Content AI)
├─ Graph: User-Movie graph, Tag graph
├─ Algorithm: BFS untuk "collaborative filtering"
├─ Big O: O(V + E) untuk finding similar movies
└─ Result: 80% of viewing dari recommendations

ChatGPT (NLP AI)
├─ Search: O(log n) binary search di token vocabulary
├─ Sort: O(n log n) untuk probability sorting
├─ Big O: Critical untuk inference speed (<100ms)
└─ Result: Real-time response untuk 100 juta users
```

---

## 🚀 9. Cara Menjalankan & Review Konsep

### Command untuk Compile & Run

```bash
# 1. Compile C++ Core
cd BackpackAI/cpp_core
g++ -fPIC -shared main.cpp -o core.dll

# 2. Install Python dependencies
cd ../backend
pip install -r requirements.txt

# 3. Run Backend API
python main.py
# Output: Uvicorn running on http://127.0.0.1:8000

# 4. Di terminal baru, jalankan Frontend
cd ../frontend
flutter pub get
flutter run -d chrome  # atau -d windows

# 5. Akses Aplikasi
# Web: http://localhost:port (Flutter akan show)
# API Docs: http://127.0.0.1:8000/docs (Swagger UI)
```

### Test API Langsung

```bash
# Test BFS
curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=BFS"

# Test DFS
curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=DFS"

# Test Sorting (get cities)
curl "http://127.0.0.1:8000/cities"

# Test Searching
curl "http://127.0.0.1:8000/search?keyword=Tokyo"

# Test Big O
curl "http://127.0.0.1:8000/bigo"
```

### Review Checklist

- [ ] **Graph**: Pahami adjacency list di `cpp_core/main.cpp:30-40`
- [ ] **BFS**: Trace eksekusi BFS di `cpp_core/main.cpp:168-216`
- [ ] **DFS**: Trace eksekusi DFS di `cpp_core/main.cpp:234-286`
- [ ] **Sorting**: Pahami Selection Sort di `cpp_core/main.cpp:118-134`
- [ ] **Searching**: Pahami Linear Search di `cpp_core/main.cpp:141-150`
- [ ] **Big O**: Lihat summary di `cpp_core/main.cpp:100-107`
- [ ] **Integration**: Lihat bagaimana Backend call C++ di `backend/main.py:62-94`
- [ ] **Frontend**: Lihat bagaimana UI consume API di `frontend/lib/main.dart`

---

## 📖 10. Referensi File Penting

| Konsep | File | Baris | Deskripsi |
|--------|------|-------|-----------|
| Graph | `cpp_core/main.cpp` | 21-299 | Class Graph + implementasi |
| BFS | `cpp_core/main.cpp` | 152-216 | Algoritma BFS |
| DFS | `cpp_core/main.cpp` | 218-286 | Algoritma DFS |
| Sorting | `cpp_core/main.cpp` | 109-134 | Selection Sort |
| Searching | `cpp_core/main.cpp` | 136-150 | Linear Search |
| Big O | `cpp_core/main.cpp` | 96-107 | Analisis kompleksitas |
| Backend | `backend/main.py` | 1-156 | FastAPI wrapper |
| Wrapper C++ | `cpp_core/main.cpp` | 340-397 | DLL export functions |

---

**🎓 Sekarang Anda tahu di mana setiap konsep SDA berada dan bagaimana mereka bekerja bersama dalam aplikasi AI Travel Planner yang real!**
