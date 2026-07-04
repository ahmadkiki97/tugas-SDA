# Holiday Planner AI - Backend API

Backend API menggunakan Python FastAPI sebagai wrapper untuk memanggil fungsi C++ core logic.

## Fitur

- **Endpoint `/cari_rute`**: Mencari rute terbaik antara dua kota menggunakan algoritma BFS atau DFS
- **Endpoint `/cities`**: Mendapatkan daftar kota terurut alfabetis dari C++ core
- **CORS Middleware**: Memungkinkan Flutter frontend mengakses API tanpa error CORS
- **Async Processing**: Semua proses berjalan asynchronous untuk performa optimal

## Setup

```bash
pip install -r requirements.txt
```

## Running

```bash
uvicorn main:app --reload --host 127.0.0.1 --port 8000
```

Backend akan berjalan di `http://127.0.0.1:8000`

## API Documentation

Setelah server berjalan, akses dokumentasi interaktif di:
- Swagger UI: `http://127.0.0.1:8000/docs`
- ReDoc: `http://127.0.0.1:8000/redoc`

## Prasyarat

File `core.dll` (atau `core.so` di Linux/Mac) harus sudah dikompilasi di folder `../cpp_core/`

Compile C++ core dengan:
```bash
cd ../cpp_core
g++ -fPIC -shared main.cpp -o core.dll
```

## Dependencies

- FastAPI: Framework web modern untuk Python
- Uvicorn: ASGI server untuk menjalankan FastAPI
- ctypes: Library Python standar untuk memanggil fungsi dari DLL/SO C++
