# ============================================================================
# BACKEND API - FastAPI Wrapper untuk C++ Graph DLL
# ============================================================================
# File ini berfungsi sebagai jembatan (bridge) antara frontend (web/mobile)
# dengan logika C++ yang berisi implementasi algoritma graph, BFS, DFS, dan sorting.
# 
# Arsitektur:
#   Frontend (HTTP) -> Python FastAPI (ctypes) -> C++ DLL (Graph Algorithms)
# ============================================================================

import os
os.add_dll_directory(r"C:\msys64\ucrt64\bin")

from fastapi import FastAPI, HTTPException
import ctypes
import asyncio
from fastapi.middleware.cors import CORSMiddleware

# ============================================================================
# KONFIGURASI APLIKASI FASTAPI
# ============================================================================

app = FastAPI(
    title="BackpackAI Backend",
    description="API untuk Travel Planner AI dengan algoritma Graph, BFS, DFS, Sorting, dan Searching",
    version="1.0.0"
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# ============================================================================
# MEMUAT LIBRARY C++ (DLL)
# ============================================================================
DLL_PATH = os.path.join(os.path.dirname(__file__), "..", "cpp_core", "core.dll")

try:
    cpp_library = ctypes.CDLL(DLL_PATH, winmode=0)
    
    cpp_library.cari_rute_wrapper.restype = ctypes.c_char_p
    cpp_library.cari_rute_wrapper.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
    
    cpp_library.get_all_cities_sorted_wrapper.restype = ctypes.c_char_p
    cpp_library.get_all_cities_sorted_wrapper.argtypes = []
    
    cpp_library.search_city_wrapper.restype = ctypes.c_char_p
    cpp_library.search_city_wrapper.argtypes = [ctypes.c_char_p]
    
    cpp_library.big_o_wrapper.restype = ctypes.c_char_p
    cpp_library.big_o_wrapper.argtypes = []
except Exception as e:
    print(f"Peringatan: Gagal memuat DLL C++ dari {DLL_PATH}. Error: {e}")
    cpp_library = None

# Membuat endpoint API dengan HTTP GET pada alamat "/cari_rute"
# Menggunakan 'async def' agar fungsi ini berjalan secara asynchronous
@app.get("/cari_rute")
async def cari_rute(asal: str, tujuan: str, algo: str = "BFS"):
    # Memeriksa apakah library C++ berhasil dimuat atau tidak
    if cpp_library is None:
        # Jika tidak, kembalikan HTTP Exception dengan status 500 (Internal Server Error)
        raise HTTPException(status_code=500, detail="Library C++ (core.dll) tidak ditemukan atau belum dicompile.")
    
    # Memulai block try-catch untuk menangani error saat proses pencarian rute berlangsung
    try:
        # Karena kita memanggil fungsi C++ yang mungkin berat, kita gunakan asyncio.to_thread
        # Ini akan menjalankan fungsi sinkron (ctypes) di thread terpisah sehingga tidak memblokir antrean server
        hasil_bytes = await asyncio.to_thread(
            cpp_library.cari_rute_wrapper, 
            asal.encode('utf-8'), # Mengubah string Python (asal) menjadi bytes (UTF-8) yang dipahami oleh C++
            tujuan.encode('utf-8'), # Mengubah string Python (tujuan) menjadi bytes (UTF-8) yang dipahami oleh C++
            algo.encode('utf-8') # Mengubah string Python (algo) menjadi bytes (UTF-8) yang dipahami oleh C++
        )
        
        # Fungsi C++ mengembalikan teks berformat bytes (karena ctypes.c_char_p), jadi kita harus decode kembali ke string biasa
        hasil_teks = hasil_bytes.decode('utf-8')
        
        # Mengembalikan data dalam bentuk dictionary Python, FastAPI akan otomatis mengubahnya menjadi format JSON
        return {
            "status": "success",
            "asal": asal,
            "tujuan": tujuan,
            "algo": algo,
            "hasil_pencarian": hasil_teks
        }
    # Jika terjadi exception/error pada saat pemanggilan fungsi
    except Exception as e:
        # Lemparkan HTTP Exception dengan kode 500 dan pesan error yang detail
        raise HTTPException(status_code=500, detail=f"Terjadi kesalahan saat mencari rute: {str(e)}")

# Membuat endpoint API baru dengan HTTP GET pada alamat "/cities"
# Endpoint ini digunakan untuk mengambil daftar kota unik yang sudah terurut alfabetis dari C++
@app.get("/cities")
async def dapatkan_kota():
    # Memeriksa apakah library C++ berhasil dimuat
    if cpp_library is None:
        # Jika tidak, kembalikan HTTP Exception
        raise HTTPException(status_code=500, detail="Library C++ (core.dll) tidak ditemukan.")
    
    try:
        # Memanggil fungsi get_all_cities_sorted_wrapper dari C++ di thread terpisah agar tidak memblokir
        hasil_bytes = await asyncio.to_thread(cpp_library.get_all_cities_sorted_wrapper)
        
        # Mengubah bytes dari C++ menjadi string Python
        hasil_teks = hasil_bytes.decode('utf-8')
        
        # Memisahkan string berdasarkan karakter titik koma (;) untuk menghasilkan list nama kota
        daftar_kota = hasil_teks.split(";") if hasil_teks else []
        
        # Mengembalikan status sukses beserta daftar kota dalam bentuk list JSON
        return {
            "status": "success",
            "cities": daftar_kota
        }
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Gagal mengambil daftar kota: {str(e)}")

# ============================================================================
# ENDPOINT SEARCHING KOTA
# ============================================================================
@app.get("/search")
async def search_city(keyword: str):
    if cpp_library is None:
        raise HTTPException(status_code=500, detail="Library C++ (core.dll) tidak ditemukan.")
    try:
        hasil_bytes = await asyncio.to_thread(cpp_library.search_city_wrapper, keyword.encode('utf-8'))
        hasil_teks = hasil_bytes.decode('utf-8')
        return {"status": "success", "keyword": keyword, "hasil_pencarian": hasil_teks}
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Gagal mencari kota: {str(e)}")

# ============================================================================
# ENDPOINT BIG O NOTATION
# ============================================================================
@app.get("/bigo")
async def get_big_o():
    if cpp_library is None:
        raise HTTPException(status_code=500, detail="Library C++ (core.dll) tidak ditemukan.")
    try:
        hasil_bytes = await asyncio.to_thread(cpp_library.big_o_wrapper)
        hasil_teks = hasil_bytes.decode('utf-8')
        return {"status": "success", "big_o_notation": hasil_teks}
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Gagal mengambil Big O: {str(e)}")

# ============================================================================
# MENJALANKAN SERVER
# ============================================================================
if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8000)
