# BackpackAI

Aplikasi struktur data dan algoritma bertema artificial intelligence untuk pencarian rute antar kota. Di dalamnya terdapat konsep graph, implementasi BFS dan DFS, Big O notasi, searching, dan sorting.

## Arsitektur

```
┌─────────────────┐     HTTP API      ┌─────────────────┐     ctypes      ┌─────────────────┐
│  Flutter App    │ ─────────────────>│  Python FastAPI │ ───────────────>│  C++ Core DLL   │
│  (Frontend)     │                    │  (Backend)      │                 │  (Graph, BFS,   │
│  Travel Theme   │ <───────────────── │  Wrapper        │ <─────────────── │   DFS, Sort)    │
└─────────────────┘     JSON Data      └─────────────────┘     C-string   └─────────────────┘
```

## Tech Stack

- **Frontend**: Flutter (Dart) - Android, iOS, Web
- **Backend**: Python (FastAPI) - API Bridge
- **Core Logic**: C++ - Graph Algorithms

## Quick Start

### 1. Kompilasi C++ Core

```bash
cd cpp_core
g++ -fPIC -shared main.cpp -o core.dll
```

### 2. Jalankan Backend API

```bash
cd backend
pip install -r requirements.txt
python main.py
```

Backend berjalan di: `http://127.0.0.1:8000`

### 3. Jalankan Flutter App

```bash
cd frontend
flutter pub get
flutter run
```

## Fitur Utama

- **Pencarian Rute**: Pilih kota keberangkatan dan destinasi tujuan
- **Metode Algoritma**: BFS (Tercepat) atau DFS (Eksplorasi)
- **Tema Travel Agent**: Desain menarik dengan warna biru laut dan kuning matahari
- **Real-time Data**: Integrasi langsung dengan backend C++ via Python API

## Rute Tersedia

```
Jakarta     <-> Singapura
Singapura   <-> Dubai
Dubai       <-> Paris
Jakarta     <-> Tokyo
Tokyo       <-> Paris
```

## API Endpoints

| Endpoint | Method | Deskripsi |
|----------|--------|-----------|
| `/cari_rute?asal={kota}&tujuan={kota}&algo={BFS/DFS}` | GET | Mencari rute liburan |
| `/cities` | GET | Mendapatkan daftar kota terurut |

## Struktur Folder

```
BackpackAI/
├── cpp_core/
│   ├── main.cpp          # Implementasi Graph, BFS, DFS
│   ├── core.dll          # Library hasil kompilasi
│   └── README.md         # Dokumentasi C++
├── backend/
│   ├── main.py           # FastAPI wrapper
│   ├── requirements.txt  # Dependencies Python
│   └── README.md         # Dokumentasi Backend
├── frontend/
│   ├── lib/
│   │   └── main.dart     # Flutter UI
│   └── README.md         # Dokumentasi Frontend
└── SYSTEM_PROMPT.md      # Panduan development
```

## Dokumentasi

- [Frontend README](frontend/README.md)
- [Backend README](backend/README.md)
- [C++ Core README](cpp_core/README.md)

## Komentar Kode

Semua baris logika, fungsi, dan UI disertai komentar bahasa Indonesia untuk pembelajaran.

## License

MIT License - Bebas digunakan untuk keperluan edukasi dan pengembangan.
