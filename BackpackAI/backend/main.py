import os
os.add_dll_directory(r"C:\msys64\ucrt64\bin")
# Mengimpor modul FastAPI untuk membuat web server (API)
# pyrefly: ignore [missing-import]
from fastapi import FastAPI, HTTPException
# Mengimpor modul ctypes untuk memanggil fungsi dari file DLL (Shared Library) C++
import ctypes
# Mengimpor modul os untuk melakukan operasi sistem file seperti mendapatkan jalur (path) yang benar
import os
# Mengimpor modul asyncio agar kita dapat menjalankan proses secara asynchronous (tidak memblokir server)
import asyncio
# Mengimpor CORSMiddleware dari FastAPI agar aplikasi Flutter dapat memanggil API ini tanpa terkena error CORS
from fastapi.middleware.cors import CORSMiddleware

# Membuat instansiasi dari aplikasi FastAPI
app = FastAPI(title="BackpackAI Backend")

# Menambahkan middleware CORS ke dalam aplikasi FastAPI
app.add_middleware(
    CORSMiddleware,
    # Mengizinkan semua origin (domain/IP) untuk mengakses API (penting saat development lokal)
    allow_origins=["*"],
    # Mengizinkan kredensial seperti cookies
    allow_credentials=True,
    # Mengizinkan semua HTTP method (GET, POST, PUT, DELETE, dll)
    allow_methods=["*"],
    # Mengizinkan semua headers
    allow_headers=["*"],
)

# Menentukan lokasi absolut dari file DLL hasil kompilasi C++
# __file__ akan mengambil lokasi file main.py saat ini, lalu kita menggunakan os.path untuk menavigasi ke cpp_core
DLL_PATH = os.path.join(os.path.dirname(__file__), "..", "cpp_core", "core.dll")

# Mencoba memuat file DLL C++ menggunakan block try-catch untuk error handling
# Mencoba memuat file DLL C++ menggunakan block try-catch untuk error handling
try:
    # Memuat library C++ (DLL) ke dalam variabel cpp_library menggunakan ctypes.CDLL
    # Ditambahkan winmode=0 agar Python 3.14 mau mencari file dependency di folder Windows & MSYS2
    cpp_library = ctypes.CDLL(DLL_PATH, winmode=0)
    
    # Menentukan tipe kembalian (return type) dari fungsi cari_rute_wrapper di C++ yaitu C-string (c_char_p)
    cpp_library.cari_rute_wrapper.restype = ctypes.c_char_p
    # Menentukan tipe parameter (argumen) dari fungsi cari_rute_wrapper di C++ yaitu 3 buah C-string (asal, tujuan, algo)
    cpp_library.cari_rute_wrapper.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
    
    # Menentukan tipe kembalian dari fungsi get_all_cities_sorted_wrapper di C++ yaitu C-string (c_char_p)
    cpp_library.get_all_cities_sorted_wrapper.restype = ctypes.c_char_p
    # Menentukan tipe parameter dari fungsi get_all_cities_sorted_wrapper di C++ yaitu kosong (tanpa argumen)
    cpp_library.get_all_cities_sorted_wrapper.argtypes = []
except Exception as e:
    # Jika gagal memuat DLL (misalnya file belum dicompile), cetak pesan error ke console
    print(f"Peringatan: Gagal memuat DLL C++ dari {DLL_PATH}. Error: {e}")
    # Set nilai cpp_library ke None sebagai penanda bahwa library tidak tersedia
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
        # Jika terjadi error, kirim pesan error detail
        raise HTTPException(status_code=500, detail=f"Gagal mengambil daftar kota: {str(e)}")
