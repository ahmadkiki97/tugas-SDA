# 📚 DOKUMENTASI LENGKAP: BackpackAI - Travel Planner dengan AI & SDA

**Tanggal**: 4 Juli 2026  
**Proyek**: BackpackAI - Aplikasi pencarian rute travel menggunakan konsep Struktur Data & Algoritma  
**Tech Stack**: C++ (Core Logic) + Python FastAPI (Backend) + Flutter (Frontend)

---

## 🗂️ Daftar Dokumentasi

Proyek ini dilengkapi dengan 4 dokumentasi komprehensif untuk membantu Anda memahami konsep SDA:

### 1. **KONSEP_SDA_DIJELASKAN.md** 📖
**Isi**: Penjelasan detail setiap konsep SDA dengan kode nyata
- Graph (Struktur Data)
- BFS (Breadth-First Search)
- DFS (Depth-First Search)
- Searching (Linear Search)
- Sorting (Selection Sort)
- Big O Notation (Analisis Kompleksitas)
- Studi Kasus AI Travel Planning
- Manfaat SDA untuk AI

**Kapan dibaca**: Ketika ingin pemahaman mendalam tentang teori + implementasi

---

### 2. **QUICK_REFERENCE.md** ⚡
**Isi**: Cheat sheet singkat untuk review cepat
- Definisi singkat setiap konsep
- Rumus Big O
- File mapping (di mana kode berada)
- Test commands

**Kapan dibaca**: Ketika butuh referensi cepat atau refresh memory

---

### 3. **LEARNING_PATH.md** 🛤️
**Isi**: Panduan belajar step-by-step dengan eksekusi manual
- Step 1: Pahami Graph
- Step 2: Pahami Searching
- Step 3: Pahami Sorting
- Step 4: Pahami BFS
- Step 5: Pahami DFS
- Step 6: Pahami Big O
- Termasuk trace eksekusi manual setiap algoritma

**Kapan dibaca**: Ketika belajar dari nol dan ingin memahami eksekusi detail

---

### 4. **TESTING_GUIDE.md** 🧪
**Isi**: Panduan compile, run, dan test aplikasi
- Setup environment (C++, Python, Flutter)
- Compile C++ DLL
- Run Backend API
- Test semua endpoints dengan curl
- Verification checklist
- Debugging common errors

**Kapan dibaca**: Ketika ingin menjalankan aplikasi dan verifikasi konsep secara langsung

---

## 🚀 Quick Start (5 Menit)

### 1. Compile C++ Core
```bash
cd BackpackAI/cpp_core
g++ -fPIC -shared main.cpp -o core.dll
```

### 2. Run Backend
```bash
cd ../backend
pip install -r requirements.txt
python main.py
# Backend: http://127.0.0.1:8000
```

### 3. Test API
```bash
# Test BFS (shortest path)
curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=BFS"

# Test DFS (eksplorasi mendalam)
curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=DFS"

# Test Sorting
curl "http://127.0.0.1:8000/cities"

# Test Searching
curl "http://127.0.0.1:8000/search?keyword=Tokyo"

# Test Big O
curl "http://127.0.0.1:8000/bigo"
```

### 4. Run Frontend (Opsional)
```bash
cd ../frontend
flutter pub get
flutter run -d chrome
```

---

## 📍 Lokasi Konsep SDA dalam Kode

| Konsep | File | Baris | Deskripsi |
|--------|------|-------|-----------|
| **Graph** | `cpp_core/main.cpp` | 30-63 | Class Graph dengan adjacency list |
| **BFS** | `cpp_core/main.cpp` | 168-216 | Breadth-First Search (Queue FIFO) |
| **DFS** | `cpp_core/main.cpp` | 234-286 | Depth-First Search (Stack LIFO) |
| **Sorting** | `cpp_core/main.cpp` | 118-134 | Selection Sort O(n²) |
| **Searching** | `cpp_core/main.cpp` | 141-150 | Linear Search O(n) |
| **Big O** | `cpp_core/main.cpp` | 100-107 | Analisis kompleksitas |
| **Backend API** | `backend/main.py` | 1-156 | FastAPI wrapper |
| **C++ Wrapper** | `cpp_core/main.cpp` | 340-397 | DLL export functions |

---

## 🎯 Konsep Utama

### 1. Graph (Struktur Data)
```
Vertex = Kota
Edge = Jalur antar kota
Adjacency List = Map<Kota, List<Tetangga>>

Jakarta: [Singapura, Tokyo]
Singapura: [Jakarta, Dubai, Sydney]
...
```

### 2. BFS vs DFS
| | BFS | DFS |
|---|-----|-----|
| **Struktur** | Queue (FIFO) | Stack (LIFO) |
| **Eksplorasi** | Level per level | Sedalam mungkin |
| **Shortest Path** | ✅ Ya | ❌ Tidak dijamin |
| **Kompleksitas** | O(V + E) | O(V + E) |

### 3. Sorting & Searching
- **Selection Sort**: O(n²) - Lambat untuk dataset besar
- **Linear Search**: O(n) - Cek satu per satu

### 4. Big O Ranking
```
O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(2ⁿ) < O(n!)
Cepat ←──────────────────────────────────────────→ Lambat
```

---

## 💡 Manfaat SDA untuk AI

### 1. Graph untuk AI Knowledge
- Google Maps: Graph untuk pathfinding
- Social Networks: Graph untuk relationship
- Recommendation System: Graph untuk similarity

### 2. BFS/DFS untuk AI Navigation
- Robotics: Pathfinding di environment
- Game AI: Strategic planning
- Neural Networks: Backpropagation (DFS-like)

### 3. Big O untuk AI Scalability
- Model O(n²) tidak scalable untuk big data
- Modern ML dirancang O(n) atau O(n log n)
- Production AI harus analyze Big O

---

## 📊 API Endpoints

| Endpoint | Method | Deskripsi | Big O |
|----------|--------|-----------|-------|
| `/cari_rute` | GET | Cari rute dengan BFS/DFS | O(V + E) |
| `/cities` | GET | Dapatkan kota terurut A-Z | O(V²) |
| `/search` | GET | Cari kota berdasarkan keyword | O(V) |
| `/bigo` | GET | Dapatkan Big O summary | O(1) |

---

## 🎓 Learning Path

### Untuk Pemula
1. Baca **QUICK_REFERENCE.md** (5 menit) - Dapatkan overview
2. Baca **LEARNING_PATH.md** (30 menit) - Pahami detail konsep
3. Ikuti **TESTING_GUIDE.md** (15 menit) - Jalankan aplikasi
4. Eksperimen dengan API (15 menit) - Test berbagai skenario

### Untuk Advanced
1. Baca **KONSEP_SDA_DIJELASKAN.md** - Deep dive teori + implementasi
2. Trace kode di `cpp_core/main.cpp` - Pahami implementasi
3. Modifikasi graph di `buildSampleGraph()` - Tambah kota baru
4. Benchmark performance dengan dataset besar

---

## 🔧 Struktur Proyek

```
BackpackAI/
├── cpp_core/
│   ├── main.cpp              # ⭐ Implementasi Graph, BFS, DFS, Sorting, Searching
│   ├── core.dll              # Library hasil kompilasi
│   └── README.md             # Dokumentasi C++ Core
│
├── backend/
│   ├── main.py               # ⭐ FastAPI wrapper (Python ↔ C++)
│   ├── requirements.txt      # Python dependencies
│   └── README.md             # Dokumentasi Backend
│
├── frontend/
│   ├── lib/main.dart         # ⭐ Flutter UI
│   ├── web/                  # Web assets
│   ├── windows/              # Windows desktop
│   └── README.md             # Dokumentasi Frontend
│
├── web_frontend/
│   ├── index.html            # Simple web UI
│   ├── app.js                # JavaScript logic
│   └── styles.css            # CSS styling
│
├── KONSEP_SDA_DIJELASKAN.md  # 📖 Dokumentasi lengkap konsep SDA
├── QUICK_REFERENCE.md        # ⚡ Cheat sheet
├── LEARNING_PATH.md          # 🛤️ Panduan belajar step-by-step
├── TESTING_GUIDE.md          # 🧪 Panduan testing
├── README.md                 # 📄 Overview proyek
└── SYSTEM_PROMPT.md          # 🛠️ Development guide
```

---

## ✅ Verification Checklist

Pastikan Anda memahami setiap konsep:

### Teori
- [ ] Pahami apa itu Graph (Vertex + Edge)
- [ ] Pahami perbedaan BFS vs DFS
- [ ] Pahami kompleksitas O(1), O(n), O(n²)
- [ ] Pahami kenapa BFS menemukan shortest path
- [ ] Pahami kenapa Selection Sort O(n²)

### Implementasi
- [ ] Tahu di mana Graph class di kode (cpp_core/main.cpp:30)
- [ ] Tahu di mana BFS function (cpp_core/main.cpp:168)
- [ ] Tahu di mana DFS function (cpp_core/main.cpp:234)
- [ ] Tahu bagaimana Backend call C++ (backend/main.py:73)
- [ ] Tahu bagaimana DLL export functions (cpp_core/main.cpp:340)

### Praktik
- [ ] Berhasil compile C++ core.dll
- [ ] Berhasil run Backend API
- [ ] Berhasil test BFS endpoint
- [ ] Berhasil test DFS endpoint
- [ ] Berhasil test Sorting, Searching, Big O

---

## 🐛 Common Issues & Solutions

### Issue 1: core.dll tidak ditemukan
```
Solusi:
1. cd BackpackAI/cpp_core
2. g++ -fPIC -shared main.cpp -o core.dll
3. Restart backend
```

### Issue 2: Python ModuleNotFoundError
```
Solusi:
pip install -r requirements.txt
```

### Issue 3: Port 8000 already in use
```
Solusi:
python -m uvicorn backend.main:app --port 8001
```

---

## 📞 Bantuan Lebih Lanjut

### Dokumentasi Tambahan
- `README.md` - Overview proyek
- `cpp_core/README.md` - Detail C++ implementation
- `backend/README.md` - Detail Python backend
- `frontend/README.md` - Detail Flutter frontend

### FAQ
**Q: Kenapa BFS lebih cepat dari DFS untuk shortest path?**  
A: BFS eksplorasi level per level, jadi menemukan target di level terendah dulu.

**Q: Kapan pakai DFS?**  
A: DFS bagus untuk maze solving, topological sorting, cycle detection.

**Q: Kenapa Selection Sort O(n²)?**  
A: Karena nested loop: outer loop n kali, inner loop n kali = n × n.

**Q: Bagaimana cara menambah kota baru?**  
A: Edit function `buildSampleGraph()` di `cpp_core/main.cpp:308`, tambah `g.addEdge("KotaA", "KotaB")`.

---

## 🎉 Selamat!

Anda sekarang memiliki:
✅ Pemahaman konsep SDA (Graph, BFS, DFS, Sorting, Searching, Big O)  
✅ Implementasi real dalam aplikasi AI Travel Planner  
✅ Dokumentasi lengkap untuk referensi  
✅ Testing guide untuk verifikasi  

**Proyek BackpackAI siap untuk review dan demo! 🚀**

---

**Dibuat pada**: 4 Juli 2026  
**Versi**: 1.0.0  
**Lisensi**: MIT License - Bebas digunakan untuk keperluan edukasi
