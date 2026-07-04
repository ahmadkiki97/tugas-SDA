# 🧪 TESTING GUIDE: Jalankan & Test Semua Konsep SDA

Panduan ini membimbing Anda menjalankan aplikasi dan test setiap konsep secara langsung.

---

## 🛠️ SETUP: Persiapan Awal (5 menit)

### Prasyarat
- Windows dengan MinGW/MSYS2 atau Linux/Mac
- Python 3.8+
- Flutter (opsional, untuk mobile/web UI)

### Step 1: Buka Terminal
```bash
cd D:\tugas SDA
# atau di Linux/Mac:
cd ~/tugas\ SDA
```

---

## 📦 PART 1: Kompilasi C++ Core (3 menit)

### Step 1: Masuk ke direktori C++
```bash
cd BackpackAI/cpp_core
```

### Step 2: Kompilasi
```bash
# Windows (MinGW)
g++ -fPIC -shared main.cpp -o core.dll

# Output yang diharapkan:
# (tidak ada error, file core.dll dibuat)

# Verifikasi:
ls -la core.dll
# Output: -rw-r--r-- ... core.dll
```

### Step 3: Coba Test C++ Local (Opsional)
```bash
# Compile program interaktif
g++ main.cpp -o test.exe

# Jalankan
./test.exe

# Output:
# === BackpackAI - Local C++ Demo ===
# 1. Tampilkan graph
# 2. BFS cari rute
# 3. DFS cari rute
# 4. Searching kota
# 5. Sorting kota A-Z
# 6. Big O notasi
# 0. Keluar
# Pilih: 

# Test opsi 1 (Tampilkan graph)
# 1 [ENTER]
# Output:
# Data graph AI Travel Planner:
# - Jakarta <-> Singapura
# - Singapura <-> Dubai
# - Dubai <-> Paris
# - Jakarta <-> Tokyo
# - Tokyo <-> Paris
# - Singapura <-> Sydney
# - Sydney <-> Tokyo
```

✅ **C++ Core berhasil dikompilasi!**

---

## 🐍 PART 2: Setup & Run Backend API (5 menit)

### Step 1: Masuk ke direktori backend
```bash
cd ../backend
```

### Step 2: Install Python dependencies
```bash
pip install -r requirements.txt

# Output:
# Successfully installed fastapi-0.xx.x uvicorn-0.xx.x ...
```

### Step 3: Jalankan Backend
```bash
python main.py

# Output:
# INFO:     Uvicorn running on http://127.0.0.1:8000
# INFO:     Application startup complete
```

✅ **Backend API running at http://127.0.0.1:8000**

### Step 4: Verifikasi Backend Online
```
Buka browser: http://127.0.0.1:8000/docs
Anda akan melihat Swagger UI dengan semua API endpoints
```

⚠️ **JANGAN TUTUP terminal backend ini!** Buka terminal baru untuk step berikutnya.

---

## 🧪 PART 3: Test API Endpoints (Manual)

### Buka Terminal Baru (Jangan tutup backend terminal!)

### TEST 1: Sorting dengan /cities endpoint
```bash
# Test 1: Ambil daftar kota terurut (Selection Sort O(n²))
curl "http://127.0.0.1:8000/cities"

# Expected Output:
# {
#   "status": "success",
#   "cities": [
#     "Dubai",
#     "Jakarta",
#     "Paris",
#     "Singapura",
#     "Sydney",
#     "Tokyo"
#   ]
# }

# ✓ Perhatikan: Kota terurut ALFABETIS (A-Z)
# Ini hasil dari Selection Sort di C++
```

### TEST 2: Linear Searching dengan /search endpoint
```bash
# Test 2a: Search kota yang ada
curl "http://127.0.0.1:8000/search?keyword=Tokyo"

# Expected Output:
# {
#   "status": "success",
#   "keyword": "Tokyo",
#   "hasil_pencarian": "Searching ditemukan: Tokyo"
# }

# ✓ Linear Search berhasil menemukan kota

# Test 2b: Search kota yang tidak ada
curl "http://127.0.0.1:8000/search?keyword=Bali"

# Expected Output:
# {
#   "status": "success",
#   "keyword": "Bali",
#   "hasil_pencarian": "Searching tidak menemukan kota: Bali"
# }

# ✓ Linear Search tidak menemukan kota yang tidak ada
```

### TEST 3: BFS dengan /cari_rute endpoint
```bash
# Test 3: BFS search Jakarta → Paris (shortest path)
curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=BFS"

# Expected Output:
# {
#   "status": "success",
#   "asal": "Jakarta",
#   "tujuan": "Paris",
#   "algo": "BFS",
#   "hasil_pencarian": "Rute BFS ditemukan: Jakarta → Tokyo → Paris"
# }

# ✓ BFS menemukan SHORTEST PATH (2 hops)
# Graph di C++:
#   Jakarta ←→ Tokyo ←→ Paris
#   Total edge: 2 (TERPENDEK!)
```

### TEST 4: DFS dengan /cari_rute endpoint
```bash
# Test 4: DFS search Jakarta → Paris (eksplorasi mendalam)
curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=DFS"

# Expected Output:
# {
#   "status": "success",
#   "asal": "Jakarta",
#   "tujuan": "Paris",
#   "algo": "DFS",
#   "hasil_pencarian": "Rute DFS ditemukan: Jakarta → Singapura → Dubai → Paris"
# }

# ⚠️ Perhatikan: Path LEBIH PANJANG dari BFS
# Jakarta → Singapura → Dubai → Paris (3 hops)
# Dibanding:
# Jakarta → Tokyo → Paris (2 hops) dari BFS

# Ini normal! DFS eksplorasi sedalam mungkin dulu
# sebelum menemukan target di path lain.
```

### TEST 5: Big O Notation dengan /bigo endpoint
```bash
# Test 5: Dapatkan analisis kompleksitas
curl "http://127.0.0.1:8000/bigo"

# Expected Output:
# {
#   "status": "success",
#   "big_o_notation": "Big O Notasi:\n- Simpan graph: O(V + E)\n- BFS: O(V + E)\n- DFS: O(V + E)\n- Linear Search kota: O(V)\n- Selection Sort: O(V^2)"
# }

# ✓ Big O summary menunjukkan kompleksitas setiap operasi
```

### TEST 6: Test rute yang tidak ada
```bash
# Test 6a: Kota asal tidak ada
curl "http://127.0.0.1:8000/cari_rute?asal=Bali&tujuan=Paris&algo=BFS"

# Expected Output:
# {
#   "status": "success",
#   "asal": "Bali",
#   "tujuan": "Paris",
#   "algo": "BFS",
#   "hasil_pencarian": "Rute tidak ditemukan: kota asal atau tujuan tidak ada dalam graph"
# }

# ✓ Error handling: kota yang tidak ada dideteksi

# Test 6b: Rute dari kota ke dirinya sendiri
curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Jakarta&algo=BFS"

# Expected Output:
# {
#   "status": "success",
#   "asal": "Jakarta",
#   "tujuan": "Jakarta",
#   "algo": "BFS",
#   "hasil_pencarian": "Rute BFS ditemukan: Jakarta"
# }

# ✓ Path langsung: hanya 1 kota (mulai = tujuan)
```

---

## 📊 COMPARISON TEST: BFS vs DFS

### Perbandingan untuk setiap rute

| Start | End | BFS Result | DFS Result | Catatan |
|-------|-----|-----------|-----------|---------|
| Jakarta | Paris | Jakarta → Tokyo → Paris | Jakarta → Singapura → Dubai → Paris | BFS lebih pendek |
| Jakarta | Sydney | Jakarta → Tokyo → Sydney | Jakarta → Tokyo → Sydney | Sama panjang |
| Singapura | Tokyo | Singapura → Sydney → Tokyo | Singapura → Sydney → Tokyo | Sama panjang |
| Dubai | Tokyo | Dubai → Paris → Tokyo | Dubai → Singapura → Sydney → Tokyo | BFS lebih pendek |

### Test All Routes
```bash
# Automation script untuk test semua kombinasi (save as test_all.sh)
#!/bin/bash

routes=(
  "Jakarta:Paris"
  "Jakarta:Sydney"
  "Singapura:Tokyo"
  "Dubai:Tokyo"
  "Tokyo:Jakarta"
  "Paris:Singapura"
)

for route in "${routes[@]}"; do
  IFS=':' read -r start end <<< "$route"
  
  echo "=== Testing $start → $end ==="
  
  echo "BFS:"
  curl -s "http://127.0.0.1:8000/cari_rute?asal=$start&tujuan=$end&algo=BFS" | grep hasil_pencarian
  
  echo "DFS:"
  curl -s "http://127.0.0.1:8000/cari_rute?asal=$start&tujuan=$end&algo=DFS" | grep hasil_pencarian
  
  echo ""
done
```

---

## 🎨 PART 4: Run Frontend (Opsional)

### Step 1: Jalankan Flutter Frontend
```bash
cd ../frontend
flutter pub get
flutter run -d chrome

# atau untuk Windows desktop:
flutter run -d windows

# Output:
# The application will launch at http://localhost:xxxxx
```

### Step 2: Test di Frontend
- Pilih kota asal di dropdown (sudah terurut A-Z dari Sorting)
- Pilih kota tujuan
- Pilih algoritma (BFS/DFS)
- Klik "Cari Rute"
- Lihat hasil di UI (dari backend API)

---

## 📈 PERFORMANCE TEST: Big O Comparison

### Bandingkan eksekusi time

```bash
# Test 1: BFS (O(V+E))
time curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=BFS"
# Output: real 0m0.050s (sangat cepat!)

# Test 2: DFS (O(V+E))
time curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=DFS"
# Output: real 0m0.048s (hampir sama cepat)

# Test 3: Sorting (O(V²))
time curl "http://127.0.0.1:8000/cities"
# Output: real 0m0.045s (untuk 6 kota terasa instant)

# Test 4: Searching (O(V))
time curl "http://127.0.0.1:8000/search?keyword=Tokyo"
# Output: real 0m0.043s (terasa instant untuk 6 kota)

# ⚠️ Catatan: Untuk dataset kecil (6 kota), semua terasa cepat
# Perbedaan Big O terasa di dataset besar (1000+ kota)
```

---

## 🐛 DEBUGGING: Jika Ada Error

### Error 1: core.dll tidak ditemukan
```
Error: Library C++ (core.dll) tidak ditemukan atau belum dicompile.

Solusi:
1. cd BackpackAI/cpp_core
2. g++ -fPIC -shared main.cpp -o core.dll
3. Verifikasi: ls -la core.dll
4. Restart backend: python main.py
```

### Error 2: Port 8000 sudah digunakan
```
Error: Address already in use

Solusi 1: Tutup program lain yang menggunakan port 8000
Solusi 2: Jalankan backend di port berbeda:
  python -m uvicorn backend.main:app --port 8001
```

### Error 3: Python dependencies error
```
Error: ModuleNotFoundError: No module named 'fastapi'

Solusi:
1. pip install -r requirements.txt
2. Pastikan Python versi 3.8+: python --version
```

---

## ✅ VERIFICATION CHECKLIST

Test setiap item di bawah dan pastikan semuanya ✓:

### C++ Core
- [ ] core.dll berhasil dikompilasi (file ada di cpp_core/)
- [ ] test.exe berjalan dengan menu interaktif
- [ ] Opsi "Tampilkan graph" menampilkan 7 edge

### Backend API
- [ ] Backend running di http://127.0.0.1:8000
- [ ] Swagger UI accessible di http://127.0.0.1:8000/docs
- [ ] Semua 5 endpoints visible di Swagger

### Sorting (O(V²))
- [ ] /cities return 6 kota terurut A-Z
- [ ] Urutan: Dubai, Jakarta, Paris, Singapura, Sydney, Tokyo

### Searching (O(V))
- [ ] /search?keyword=Tokyo return "ditemukan: Tokyo"
- [ ] /search?keyword=Bali return "tidak menemukan: Bali"

### Graph & BFS (O(V+E))
- [ ] /cari_rute?asal=Jakarta&tujuan=Paris&algo=BFS return Jakarta → Tokyo → Paris
- [ ] Path panjang: 2 hops (SHORTEST)

### DFS (O(V+E))
- [ ] /cari_rute?asal=Jakarta&tujuan=Paris&algo=DFS return path (bisa berbeda dari BFS)
- [ ] Path panjang: >= 2 hops

### Big O
- [ ] /bigo return kompleksitas semua operasi

### Error Handling
- [ ] /cari_rute?asal=Bali&tujuan=Paris return error message
- [ ] /search?keyword=Bali return "tidak menemukan"

---

## 📝 CONTOH HASIL TESTING LENGKAP

Jika semua test ✓, output Anda akan mirip ini:

```bash
$ curl "http://127.0.0.1:8000/cities"
{"status":"success","cities":["Dubai","Jakarta","Paris","Singapura","Sydney","Tokyo"]}

$ curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=BFS"
{"status":"success","asal":"Jakarta","tujuan":"Paris","algo":"BFS","hasil_pencarian":"Rute BFS ditemukan: Jakarta → Tokyo → Paris"}

$ curl "http://127.0.0.1:8000/cari_rute?asal=Jakarta&tujuan=Paris&algo=DFS"
{"status":"success","asal":"Jakarta","tujuan":"Paris","algo":"DFS","hasil_pencarian":"Rute DFS ditemukan: Jakarta → Singapura → Dubai → Paris"}

$ curl "http://127.0.0.1:8000/search?keyword=Tokyo"
{"status":"success","keyword":"Tokyo","hasil_pencarian":"Searching ditemukan: Tokyo"}

$ curl "http://127.0.0.1:8000/bigo"
{"status":"success","big_o_notation":"Big O Notasi:\n- Simpan graph: O(V + E)\n- BFS: O(V + E)\n- DFS: O(V + E)\n- Linear Search kota: O(V)\n- Selection Sort: O(V^2)"}
```

**Selamat! Anda berhasil test semua konsep SDA 🎉**

---

## 🎓 Kesimpulan Testing

Melalui testing ini, Anda telah memverifikasi:

1. ✅ **Graph Construction** - Kota dan jalur terhubung dengan benar
2. ✅ **Selection Sort (O(V²))** - Kota terurut A-Z
3. ✅ **Linear Search (O(V))** - Pencarian kota berhasil
4. ✅ **BFS (O(V+E))** - Shortest path ditemukan
5. ✅ **DFS (O(V+E))** - Path alternatif ditemukan
6. ✅ **Big O Analysis** - Kompleksitas setiap operasi terdokumentasi

**Aplikasi BackpackAI Anda sudah fully functional! 🚀**
