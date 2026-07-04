# Holiday Planner AI - Frontend

Frontend aplikasi Holiday Planner AI dengan tema Travel Agent yang menarik untuk keluarga.

## Arsitektur Proyek

Proyek BackpackAI terdiri dari 3 komponen utama:

1. **Frontend (Flutter)** - Aplikasi mobile/web dengan UI Travel Agent theme
2. **Backend API (Python FastAPI)** - Wrapper yang menghubungkan Flutter ke C++
3. **Core Logic (C++)** - Implementasi Graph, BFS, DFS untuk pencarian rute

## Setup & Running

### Prerequisites
- Flutter SDK (latest version)
- Android Studio / Xcode (untuk mobile)
- Python 3.8+ (untuk backend)
- Visual Studio C++ (untuk compile C++)

### Frontend Setup

```bash
cd frontend
flutter pub get
flutter run
```

### Backend Setup (Required)

```bash
cd ../backend
pip install -r requirements.txt
python main.py
```

Backend akan berjalan di `http://127.0.0.1:8000`

### C++ Core Compilation

```bash
cd ../cpp_core
g++ -fPIC -shared main.cpp -o core.dll
```

## Fitur Utama

- **Rencanakan Liburan**: Pilih kota keberangkatan dan destinasi tujuan
- **Metode Rute**: BFS (Tercepat) atau DFS (Eksplorasi)
- **Tema Travel Agent**: Desain menarik dengan palet warna biru laut dan kuning matahari
- **Integrasi Backend**: Real-time data dari server C++ via Python API

## Komentar Kode

Semua baris logika, fungsi, dan tombol UI disertai komentar bahasa Indonesia untuk pembelajaran.

## Resource Tambahan

- [Flutter Documentation](https://docs.flutter.dev/)
- [FastAPI Documentation](https://fastapi.tiangolo.com/)
- [Graph Algorithms](https://www.geeksforgeeks.org/graph-data-structure-and-algorithms/)
