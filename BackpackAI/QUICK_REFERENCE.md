# 🎯 QUICK REFERENCE: Konsep SDA dalam BackpackAI

## 1️⃣ GRAPH = Peta Kota
```
Definisi: Struktur data dengan Vertex (kota) dan Edge (jalur)
Rumus: O(V + E) = O(6 kota + 7 jalur) = O(13)

Kode:
  adjacency_list["Jakarta"] = ["Singapura", "Tokyo"]
  adjacency_list["Tokyo"] = ["Jakarta", "Paris", "Sydney"]
  ...

Lokasi: cpp_core/main.cpp:30-63
```

---

## 2️⃣ BFS = Cari Rute Terpendek (Level per Level)
```
Cara: Queue FIFO → Jelajahi tetangga level demi level
Hasil: Jakarta → Tokyo → Paris (2 hop = TERPENDEK)
Rumus: O(V + E) = O(6 + 7) = 13 operations
Keunggulan: ✓ Shortest path, ✓ Optimal untuk unweighted graph

Queue Progress:
  [Jakarta] 
  → [Jakarta-Singapura, Jakarta-Tokyo]
  → [Jakarta-Tokyo-Paris] ← FOUND!

Lokasi: cpp_core/main.cpp:168-216
```

---

## 3️⃣ DFS = Cari Rute Eksplorasi (Sedalam Mungkin)
```
Cara: Stack LIFO → Jelajahi tetangga sedalam mungkin, baru backtrack
Hasil: Jakarta → Singapura → Dubai → Paris (3+ hop = LEBIH PANJANG)
Rumus: O(V + E) = O(6 + 7) = 13 operations (sama cepat!)
Keunggulan: ✓ Eksplorasi menyeluruh, ✓ Gunakan untuk maze solving

Stack Progress:
  [Jakarta]
  → [Jakarta-Singapura, Jakarta-Tokyo]
  → [Jakarta-Tokyo-Paris] (LIFO = ambil yang terakhir ditambah)
  → FOUND!

Lokasi: cpp_core/main.cpp:234-286
```

---

## 4️⃣ SEARCHING = Linear Search Kota
```
Cara: Cek satu per satu sampai ditemukan
Rumus: O(V) = O(6 kota) worst case

Contoh:
  searchCity("Tokyo")
  → Cek "Jakarta"? ✗
  → Cek "Singapura"? ✗
  → Cek "Dubai"? ✗
  → Cek "Tokyo"? ✓ FOUND!

Lokasi: cpp_core/main.cpp:141-150
```

---

## 5️⃣ SORTING = Selection Sort (A-Z Urut)
```
Cara: Cari minimum, tukar dengan posisi pertama, ulangi
Rumus: O(V²) = O(6²) = 36 operations (PALING LAMBAT!)

Sebelum: [Tokyo, Jakarta, Dubai, Paris, Singapura]
         
Step 1:  Cari min = Dubai, tukar
         [Dubai, Jakarta, Tokyo, Paris, Singapura]
         
Step 2:  Cari min dari Jakarta = Jakarta, tidak tukar
         [Dubai, Jakarta, Tokyo, Paris, Singapura]
         
Step 3:  Cari min dari Tokyo = Paris, tukar
         [Dubai, Jakarta, Paris, Tokyo, Singapura]
         
Step 4:  Cari min dari Tokyo = Singapura, tukar
         [Dubai, Jakarta, Paris, Singapura, Tokyo]

Hasil:   [Dubai, Jakarta, Paris, Singapura, Tokyo] ✓ SORTED A-Z

Lokasi: cpp_core/main.cpp:118-134
```

---

## 6️⃣ BIG O = Ranking Kecepatan
```
Ranking dari TERCEPAT ke TERLAMBAT:

O(1)    ← Ambil elemen dari map (instant)
O(log n) ← Binary search (jarang dipakai)
O(V)    ← Linear search (cek semua 6 kota)
O(V+E)  ← BFS & DFS (kunjungi semua vertex dan edge)
O(V²)   ← Selection sort (nested loop) LAMBAT!
O(2ⁿ)   ← Exponential (nightmare!)
O(n!)   ← Factorial (HELL!)

Dalam Proyek:
  Map lookup    = O(1)      (TERCEPAT)
  Linear search = O(V)      
  BFS/DFS       = O(V+E)    
  Sorting       = O(V²)     (TERLAMBAT)

Lokasi: cpp_core/main.cpp:100-107
```

---

## 🔗 Integration Architecture

```
Frontend (Flutter/Web)
    ↓ HTTP GET /cari_rute?asal=Jakarta&tujuan=Paris&algo=BFS
Backend (FastAPI Python)
    ↓ Call C++ DLL via ctypes
C++ Core (Graph, BFS, DFS, Sorting)
    ↓ Return result string
Backend (Convert to JSON)
    ↓ HTTP 200 OK {hasil_pencarian}
Frontend (Display on UI)
```

---

## 📍 File Mapping

| Konsep | File | Baris |
|--------|------|-------|
| Graph class | `cpp_core/main.cpp` | 30-299 |
| BFS function | `cpp_core/main.cpp` | 168-216 |
| DFS function | `cpp_core/main.cpp` | 234-286 |
| Sorting function | `cpp_core/main.cpp` | 118-134 |
| Searching function | `cpp_core/main.cpp` | 141-150 |
| Big O summary | `cpp_core/main.cpp` | 100-107 |
| Python wrapper | `backend/main.py` | 62-94 |
| DLL export | `cpp_core/main.cpp` | 340-397 |

---

## 🧪 Test Commands

```bash
# Compile C++ (ubah .dll jadi .so untuk Linux/Mac)
g++ -fPIC -shared cpp_core/main.cpp -o cpp_core/core.dll

# Run backend
python backend/main.py

# Test BFS (akan return Jakarta → Tokyo → Paris)
curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=BFS"

# Test DFS (akan return Jakarta → ... → Paris, path berbeda)
curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=DFS"

# Test sorting (cities A-Z)
curl "http://127.0.0.1:8000/cities"

# Test searching
curl "http://127.0.0.1:8000/search?keyword=Tokyo"

# Test Big O explanation
curl "http://127.0.0.1:8000/bigo"
```

---

## ✅ Review Checklist (5 Menit)

- [ ] Buka `cpp_core/main.cpp` dan pahami Graph class (line 30-63)
- [ ] Trace BFS: step-by-step cari Jakarta → Paris (line 168-216)
- [ ] Trace DFS: bandingkan dengan BFS (line 234-286)
- [ ] Run test commands di atas dan lihat hasil
- [ ] Pahami kenapa BFS faster than DFS untuk shortest path
- [ ] Pahami kenapa O(V²) Selection Sort LAMBAT untuk big data

---

**Selamat! Anda sudah memahami konsep SDA dalam AI Travel Planner 🎉**
