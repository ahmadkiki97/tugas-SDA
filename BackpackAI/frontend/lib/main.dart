// Mengimpor library material UI yang merupakan komponen dasar untuk membuat antarmuka di Flutter
import 'package:flutter/material.dart';
// Mengimpor library 'http' untuk memungkinkan aplikasi kita melakukan HTTP request ke API backend Python
// 'as http' digunakan sebagai alias agar kita memanggil fungsi dengan http.get atau http.post
import 'package:http/http.dart' as http;
// Mengimpor library dart:convert untuk mengonversi data, terutama mende-code teks JSON (JSON.decode)
import 'dart:convert';

// Fungsi utama aplikasi Flutter yang pertama kali dijalankan saat aplikasi dibuka
void main() {
  // Menjalankan aplikasi dengan widget MyApp sebagai akar (root) aplikasi
  runApp(const MyApp());
}

// MyApp adalah StatelessWidget yang berarti widget ini tidak memiliki state atau data yang berubah-ubah secara mandiri
class MyApp extends StatelessWidget {
  // Konstruktor standar (default constructor) dari MyApp
  const MyApp({super.key});

  // Fungsi build untuk merender (membangun) tampilan widget ini di layar
  @override
  Widget build(BuildContext context) {
    // MaterialApp adalah widget utama yang mengatur tema dan navigasi aplikasi
    return MaterialApp(
      // Judul aplikasi (biasanya muncul saat multitasking di smartphone)
      title: 'BackpackAI',
      // Mengaktifkan atau menonaktifkan banner debug di pojok kanan atas layar (false = sembunyikan)
      debugShowCheckedModeBanner: false,
      // Mengkonfigurasi tema (theme) aplikasi sesuai instruksi Cyberpunk (Purple/Cyan/Pink)
      theme: ThemeData(
        brightness: Brightness.light,
        primarySwatch: Colors.blue,
        colorScheme: const ColorScheme.light(
          primary: Color(0xFF0077B6), // Biru Laut (Travel)
          secondary: Color(0xFFFFB703), // Kuning Matahari (Holiday)
          surface: Color(0xFFF8F9FA), // Off-white
        ),
        scaffoldBackgroundColor: const Color(0xFFE9ECEF),
      ),
      // Menentukan halaman pertama (Home) yang dibuka adalah widget BackpackHomePage
      home: const BackpackHomePage(),
    );
  }
}

// BackpackHomePage adalah StatefulWidget karena halaman ini memiliki state (keadaan) yang bisa berubah (seperti hasil teks)
class BackpackHomePage extends StatefulWidget {
  const BackpackHomePage({super.key});

  @override
  State<BackpackHomePage> createState() => _BackpackHomePageState();
}

// Ini adalah kelas State yang menyimpan variabel dinamis untuk BackpackHomePage
class _BackpackHomePageState extends State<BackpackHomePage> {
  // Menampung daftar kota yang tersedia, default terisi jika API gagal diakses
  List<String> cities = ['Dubai', 'Jakarta', 'Paris', 'Singapura', 'Tokyo'];

  // Variabel untuk menyimpan pilihan kota asal dan kota tujuan dari dropdown
  String? selectedAsal = 'Jakarta';
  String? selectedTujuan = 'Paris';

  // Variabel untuk menyimpan pilihan algoritma, defaultnya adalah BFS
  String selectedAlgo = 'BFS';

  // Variabel untuk menyimpan teks rute hasil pencarian yang dikembalikan oleh API
  String hasilRute = "Silakan pilih kota dan algoritma, lalu klik tombol Cari Rute.";

  // Variabel bertipe boolean untuk melacak apakah pencarian rute sedang berjalan
  bool isLoading = false;

  // Variabel bertipe boolean untuk melacak apakah daftar kota sedang dimuat dari backend
  bool isCitiesLoading = false;

  @override
  void initState() {
    super.initState();
    // Memanggil fungsi untuk mengambil daftar kota terurut dari backend saat widget pertama kali dibuat
    fetchCities();
  }

  // Fungsi asynchronous untuk mengambil daftar kota terurut dari backend Python FastAPI
  Future<void> fetchCities() async {
    setState(() {
      // Mengaktifkan status memuat kota
      isCitiesLoading = true;
    });

    try {
      // Endpoint backend untuk mendapatkan daftar kota
      final url = Uri.parse('http://127.0.0.1:8000/cities');
      // Mengirimkan request HTTP GET secara asynchronous
      final response = await http.get(url);

      // Jika backend merespon dengan status sukses (200)
      if (response.statusCode == 200) {
        // Mendecode teks JSON respon menjadi objek Map
        final data = json.decode(response.body);
        // Mengambil daftar kota
        final List<dynamic> fetchedCities = data['cities'];
        
        setState(() {
          // Mengubah data dynamic list menjadi List<String> di Dart
          cities = fetchedCities.map((e) => e.toString()).toList();
          
          // Memastikan pilihan default (Jakarta & Paris) ada di list, jika tidak pilih elemen acak
          if (cities.contains('Jakarta')) {
            selectedAsal = 'Jakarta';
          } else if (cities.isNotEmpty) {
            selectedAsal = cities.first;
          }
          
          if (cities.contains('Paris')) {
            selectedTujuan = 'Paris';
          } else if (cities.isNotEmpty) {
            selectedTujuan = cities.last;
          }
        });
      }
    } catch (e) {
      // Jika terjadi error koneksi ke backend, gunakan fallback kota yang sudah terdefinisi
      debugPrint("Gagal mengambil kota dari backend, menggunakan list default: $e");
    } finally {
      setState(() {
        // Menonaktifkan status memuat kota
        isCitiesLoading = false;
      });
    }
  }

  // Fungsi ini bersifat asynchronous (async) untuk mencari rute dengan memanggil FastAPI Python
  Future<void> cariRute() async {
    // Mengubah state (keadaan) menggunakan setState agar tampilan layar (UI) bisa diperbarui
    setState(() {
      // Mengaktifkan tanda loading (menampilkan progress bar berputar)
      isLoading = true;
      // Menghapus teks hasil sebelumnya jika ada
      hasilRute = "Sedang mencari rute...";
    });

    // Mulai block try-catch untuk menangani kemungkinan terjadinya error saat HTTP request
    try {
      // Mengambil nilai asal, tujuan, dan algo dari state pilihan dropdown
      String asal = selectedAsal ?? 'Jakarta';
      String tujuan = selectedTujuan ?? 'Paris';
      String algo = selectedAlgo;

      // URL tujuan FastAPI (127.0.0.1 untuk localhost backend) dengan parameter query tambahan algo
      final url = Uri.parse(
        'http://127.0.0.1:8000/cari_rute?asal=$asal&tujuan=$tujuan&algo=$algo',
      );

      // Menggunakan await untuk menghentikan eksekusi baris ini sementara, sampai request HTTP GET selesai dan merespon
      final response = await http.get(url);

      // Mengecek apakah kode status HTTP merespon angka 200 (berarti berhasil)
      if (response.statusCode == 200) {
        // Mengonversi data JSON yang berupa teks utuh menjadi map key-value Dart (dictionary)
        final data = json.decode(response.body);
        // Memperbarui state (tampilan layar) lagi
        setState(() {
          // Memasukkan hasil dari API yang berada pada key 'hasil_pencarian' ke variabel tampilan
          hasilRute = data['hasil_pencarian'];
        });
      } else {
        // Jika status code bukan 200 (misalnya 404, 500), tampilkan pesan error dengan status code-nya
        setState(() {
          hasilRute =
              "Error API: Gagal terhubung (Status ${response.statusCode})";
        });
      }
    } catch (e) {
      // Menangkap exception (error internal) saat koneksi HTTP, contohnya server python belum dinyalakan
      setState(() {
        // Menampilkan pesan error detail
        hasilRute =
            "Error Koneksi: Tidak dapat terhubung ke Backend Python. Apakah server berjalan? Detail: $e";
      });
    } finally {
      // Blok finally ini akan selalu dieksekusi, entah itu sukses ataupun terjadi error
      setState(() {
        // Mematikan tanda loading agar pengguna tahu proses telah selesai
        isLoading = false;
      });
    }
  }

  // Fungsi utama untuk merender (membangun) tampilan widget
  @override
  Widget build(BuildContext context) {
    // Scaffold adalah struktur tata letak (layout) dasar untuk menyusun halaman
    return Scaffold(
      // Membuat batang aplikasi (App Bar) di paling atas layar
      appBar: AppBar(
        title: const Text(
          'Holiday Planner AI',
          style: TextStyle(fontWeight: FontWeight.bold, color: Colors.white),
        ),
        backgroundColor: const Color(0xFF0077B6),
        actions: [
          IconButton(
            icon: const Icon(Icons.refresh, color: Colors.white),
            onPressed: fetchCities,
            tooltip: 'Segarkan Kota',
          ),
        ],
      ),
      body: Container(
        decoration: const BoxDecoration(
          color: Color(0xFFE9ECEF),
        ),
        // Center digunakan untuk menempatkan semua elemen widget persis di tengah halaman
        child: Center(
          // SingleChildScrollView digunakan agar konten tidak terpotong (overflow) ketika keyboard muncul
          child: SingleChildScrollView(
            // Column digunakan untuk menata (me-layout) widget secara vertikal dari atas ke bawah
            child: Column(
              // Mengatur posisi kolom di tengah secara vertikal (sumbu Y)
              mainAxisAlignment: MainAxisAlignment.center,
              // Widget children adalah daftar-daftar elemen yang berada di dalam kolom
              children: <Widget>[
                // Teks besar bertuliskan "Cari Rute Destinasi Anda"
                const Text(
                  'Rencanakan Liburan Keluarga Anda',
                  style: TextStyle(
                    fontSize: 24,
                    fontWeight: FontWeight.bold,
                    color: Color(0xFF023E8A),
                  ),
                ),
                // Kotak kosong setinggi 20 piksel sebagai spasi (pemisah) jarak
                const SizedBox(height: 20),

                // Menampilkan indikator loading kecil jika daftar kota sedang dimuat dari backend
                if (isCitiesLoading)
                  const Padding(
                    padding: EdgeInsets.symmetric(vertical: 10),
                    child: Text(
                      'Memuat daftar destinasi...',
                      style: TextStyle(color: Color(0xFF0077B6), fontStyle: FontStyle.italic),
                    ),
                  ),

                DropdownButtonFormField<String>(
                  value: selectedAsal,
                  decoration: InputDecoration(
                    labelText: 'Kota Keberangkatan',
                    labelStyle: const TextStyle(color: Color(0xFF0077B6)),
                    border: OutlineInputBorder(
                      borderRadius: BorderRadius.circular(10),
                    ),
                    enabledBorder: OutlineInputBorder(
                      borderSide: const BorderSide(color: Color(0xFF0077B6), width: 1.0),
                      borderRadius: BorderRadius.circular(10),
                    ),
                    focusedBorder: OutlineInputBorder(
                      borderSide: const BorderSide(
                        color: Color(0xFFFFB703),
                        width: 2.0,
                      ),
                      borderRadius: BorderRadius.circular(10),
                    ),
                  ),
                  items: cities.map((String city) {
                    return DropdownMenuItem<String>(
                      value: city,
                      child: Text(city),
                    );
                  }).toList(),
                  onChanged: (String? newValue) {
                    setState(() {
                      selectedAsal = newValue;
                    });
                  },
                ),
                const SizedBox(height: 15),

                DropdownButtonFormField<String>(
                  value: selectedTujuan,
                  decoration: InputDecoration(
                    labelText: 'Destinasi Tujuan',
                    labelStyle: const TextStyle(color: Color(0xFF0077B6)),
                    border: OutlineInputBorder(
                      borderRadius: BorderRadius.circular(10),
                    ),
                    enabledBorder: OutlineInputBorder(
                      borderSide: const BorderSide(color: Color(0xFF0077B6), width: 1.0),
                      borderRadius: BorderRadius.circular(10),
                    ),
                    focusedBorder: OutlineInputBorder(
                      borderSide: const BorderSide(
                        color: Color(0xFFFFB703),
                        width: 2.0,
                      ),
                      borderRadius: BorderRadius.circular(10),
                    ),
                  ),
                  items: cities.map((String city) {
                    return DropdownMenuItem<String>(
                      value: city,
                      child: Text(city),
                    );
                  }).toList(),
                  onChanged: (String? newValue) {
                    setState(() {
                      selectedTujuan = newValue;
                    });
                  },
                ),
                const SizedBox(height: 20),

                Container(
                  padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 8),
                  decoration: BoxDecoration(
                    border: Border.all(color: const Color(0xFF0077B6)),
                    borderRadius: BorderRadius.circular(10),
                    color: const Color(0xFF0077B6).withValues(alpha: 0.1),
                  ),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      const Text(
                        'Metode Rute:',
                        style: TextStyle(
                          fontSize: 16,
                          fontWeight: FontWeight.bold,
                          color: Color(0xFF023E8A),
                        ),
                      ),
                      Row(
                        children: [
                          ChoiceChip(
                            label: const Text('BFS (Tercepat)'),
                            selected: selectedAlgo == 'BFS',
                            selectedColor: const Color(0xFFFFB703).withValues(alpha: 0.3),
                            labelStyle: TextStyle(
                              color: selectedAlgo == 'BFS' ? const Color(0xFFFFB703) : Colors.grey,
                              fontWeight: FontWeight.bold,
                            ),
                            shape: RoundedRectangleBorder(
                              side: BorderSide(
                                color: selectedAlgo == 'BFS' ? const Color(0xFFFFB703) : Colors.transparent,
                                width: 1.5,
                              ),
                              borderRadius: BorderRadius.circular(8),
                            ),
                            onSelected: (bool selected) {
                              if (selected) {
                                setState(() {
                                  selectedAlgo = 'BFS';
                                });
                              }
                            },
                          ),
                          const SizedBox(width: 8),
                          ChoiceChip(
                            label: const Text('DFS (Eksplorasi)'),
                            selected: selectedAlgo == 'DFS',
                            selectedColor: const Color(0xFF00B4D8).withValues(alpha: 0.3),
                            labelStyle: TextStyle(
                              color: selectedAlgo == 'DFS' ? const Color(0xFF00B4D8) : Colors.grey,
                              fontWeight: FontWeight.bold,
                            ),
                            shape: RoundedRectangleBorder(
                              side: BorderSide(
                                color: selectedAlgo == 'DFS' ? const Color(0xFF00B4D8) : Colors.transparent,
                                width: 1.5,
                              ),
                              borderRadius: BorderRadius.circular(8),
                            ),
                            onSelected: (bool selected) {
                              if (selected) {
                                setState(() {
                                  selectedAlgo = 'DFS';
                                });
                              }
                            },
                          ),
                        ],
                      ),
                    ],
                  ),
                ),
                const SizedBox(height: 25),

                isLoading
                    ? const CircularProgressIndicator(color: Color(0xFF0077B6))
                    : ElevatedButton(
                        onPressed: cariRute,
                        style: ElevatedButton.styleFrom(
                          backgroundColor: const Color(0xFF0077B6),
                          foregroundColor: Colors.white,
                          padding: const EdgeInsets.symmetric(
                            horizontal: 50,
                            vertical: 15,
                          ),
                          shape: RoundedRectangleBorder(
                            borderRadius: BorderRadius.circular(20),
                          ),
                          elevation: 5,
                        ),
                        child: const Text(
                          'Cari Rute Liburan',
                          style: TextStyle(
                            fontSize: 18,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ),

                const SizedBox(height: 30),

                Card(
                  color: const Color(0xFFE9ECEF),
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(15),
                  ),
                  margin: const EdgeInsets.symmetric(horizontal: 10),
                  child: Padding(
                    padding: const EdgeInsets.all(20.0),
                    child: Text(
                      hasilRute,
                      textAlign: TextAlign.center,
                      style: const TextStyle(
                        fontSize: 16,
                        color: Color(0xFF023E8A),
                        fontWeight: FontWeight.w500,
                      ),
                    ),
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
