#include <cstring> // Library standar C untuk memanipulasi string (seperti strcpy)
#include <iostream> // Library standar untuk input/output
#include <queue> // Library standar untuk menggunakan struktur data antrean (queue) yang dibutuhkan oleh BFS
#include <string> // Library standar untuk menggunakan tipe data teks (string)
#include <unordered_map> // Library standar untuk menggunakan struktur data Hash Map (key-value)
#include <unordered_set> // Library standar untuk menggunakan struktur data Hash Set (menyimpan data unik)
#include <vector> // Library standar untuk menggunakan struktur data dinamis bernama vector

using namespace std; // Menggunakan namespace standard agar tidak perlu menulis
                     // std:: terus menerus

// Mendefinisikan struktur data Graph
class Graph {
private:
  // Menyimpan daftar ketetanggaan (adjacency list) di mana kunci adalah nama
  // kota, dan nilainya adalah daftar kota tetangga
  unordered_map<string, vector<string>> adjList;

public:
  // Fungsi untuk menambahkan sisi (edge) antara dua kota
  void addEdge(string asal, string tujuan) {
    // Menambahkan kota 'tujuan' ke dalam daftar tetangga kota 'asal'
    adjList[asal].push_back(tujuan);
    // Menambahkan kota 'asal' ke dalam daftar tetangga kota 'tujuan' (karena
    // graf ini tidak berarah / undirected)
    adjList[tujuan].push_back(asal);
  }

  // Fungsi pencarian menggunakan algoritma Breadth-First Search (BFS)
  string BFS(string start, string target) {
    // Jika graf tidak memiliki kota awal atau kota target, langsung kembalikan
    // pesan error
    if (adjList.find(start) == adjList.end() ||
        adjList.find(target) == adjList.end()) {
      return "Rute tidak ditemukan: Kota tidak ada dalam graf";
    }

    // Antrean (queue) untuk melacak jalur (path) yang sedang dieksplorasi
    queue<vector<string>> q;
    // Himpunan (set) untuk melacak kota-kota yang sudah pernah dikunjungi agar
    // tidak terjadi perulangan (cycle)
    unordered_set<string> visited;

    // Memasukkan kota awal sebagai jalur pertama ke dalam antrean
    q.push({start});
    // Menandai kota awal sebagai sudah dikunjungi
    visited.insert(start);

    // Selama antrean masih memiliki jalur yang bisa dicek
    while (!q.empty()) {
      // Mengambil jalur yang berada di paling depan antrean
      vector<string> path = q.front();
      // Menghapus jalur tersebut dari antrean
      q.pop();

      // Mendapatkan kota terakhir dari jalur yang sedang dicek
      string current = path.back();

      // Jika kota terakhir ini adalah kota target yang kita cari
      if (current == target) {
        // Kita gabungkan jalur tersebut menjadi satu string teks
        string result = "Rute BFS ditemukan: ";
        for (int i = 0; i < path.size();
             i++) {          // Melakukan iterasi pada setiap kota di jalur
          result += path[i]; // Menambahkan nama kota ke hasil
          if (i != path.size() - 1)
            result +=
                " -> "; // Menambahkan tanda panah jika bukan kota terakhir
        }
        // Mengembalikan teks hasil pencarian
        return result;
      }

      // Mengecek semua kota tetangga dari kota saat ini
      for (string tetangga : adjList[current]) {
        // Jika kota tetangga ini belum pernah dikunjungi
        if (visited.find(tetangga) == visited.end()) {
          // Tandai kota tetangga ini sebagai sudah dikunjungi
          visited.insert(tetangga);
          // Buat jalur baru dengan menduplikat jalur yang sekarang
          vector<string> new_path = path;
          // Tambahkan kota tetangga tersebut ke akhir jalur baru
          new_path.push_back(tetangga);
          // Masukkan jalur baru tersebut ke dalam antrean untuk dicek nanti
          q.push(new_path);
        }
      }
    }
    // Jika antrean kosong tapi target tidak ditemukan, kembalikan pesan gagal
    return "Rute tidak ditemukan.";
  }

  // Fungsi pencarian menggunakan algoritma Depth-First Search (DFS)
  string DFS(string start, string target) {
    // Jika graf tidak memiliki kota awal atau kota target, langsung kembalikan
    // pesan error
    if (adjList.find(start) == adjList.end() ||
        adjList.find(target) == adjList.end()) {
      // Mengembalikan pesan error kota tidak ada
      return "Rute tidak ditemukan: Kota tidak ada dalam graf";
    }

    // Tumpukan (stack) direpresentasikan dengan vector untuk melacak jalur yang
    // dieksplorasi (Last In First Out)
    vector<vector<string>> s;
    // Himpunan (set) untuk melacak kota-kota yang sudah pernah dikunjungi agar
    // tidak terjadi perulangan
    unordered_set<string> visited;

    // Memasukkan kota awal sebagai jalur pertama ke dalam tumpukan
    s.push_back({start});

    // Selama tumpukan masih memiliki jalur yang bisa dicek
    while (!s.empty()) {
      // Mengambil jalur yang berada di paling atas tumpukan
      vector<string> path = s.back();
      // Menghapus jalur tersebut dari tumpukan
      s.pop_back();

      // Mendapatkan kota terakhir dari jalur yang sedang dicek
      string current = path.back();

      // Jika kota target ditemukan
      if (current == target) {
        // Kita gabungkan jalur tersebut menjadi satu string teks
        string result = "Rute DFS ditemukan: ";
        // Melakukan iterasi pada setiap kota di jalur
        for (int i = 0; i < path.size(); i++) {
          // Menambahkan nama kota ke hasil
          result += path[i];
          // Menambahkan tanda panah jika bukan kota terakhir
          if (i != path.size() - 1)
            result += " -> ";
        }
        // Mengembalikan teks hasil pencarian
        return result;
      }

      // Jika kota saat ini belum dikunjungi
      if (visited.find(current) == visited.end()) {
        // Tandai kota saat ini sebagai sudah dikunjungi
        visited.insert(current);

        // Mengecek semua kota tetangga dari kota saat ini
        for (string tetangga : adjList[current]) {
          // Jika kota tetangga ini belum pernah dikunjungi
          if (visited.find(tetangga) == visited.end()) {
            // Buat jalur baru dengan menduplikat jalur yang sekarang
            vector<string> new_path = path;
            // Tambahkan kota tetangga tersebut ke akhir jalur baru
            new_path.push_back(tetangga);
            // Masukkan jalur baru tersebut ke dalam tumpukan untuk dicek nanti
            s.push_back(new_path);
          }
        }
      }
    }
    // Jika tumpukan kosong tapi target tidak ditemukan, kembalikan pesan gagal
    return "Rute tidak ditemukan.";
  }

  // Fungsi untuk mengambil semua kota unik yang terdaftar di graf
  vector<string> getUniqueCities() {
    // Membuat vector untuk menampung nama-nama kota
    vector<string> cities;
    // Melakukan iterasi pada setiap key (nama kota) di adjList
    for (auto const& [kota, tetangga] : adjList) {
      // Memasukkan nama kota ke dalam vector
      cities.push_back(kota);
    }
    // Mengembalikan daftar kota unik
    return cities;
  }
};

// Fungsi Selection Sort kustom untuk mengurutkan daftar kota secara alfabetis (A-Z)
// Algoritma ini membandingkan elemen untuk mencari nilai terkecil lalu menukarnya ke posisi depan
void selectionSort(vector<string>& arr) {
  int n = arr.size(); // Mendapatkan jumlah elemen dalam vector
  for (int i = 0; i < n - 1; i++) { // Iterasi untuk setiap posisi elemen terdepan yang belum terurut
    int min_idx = i; // Menandai indeks elemen terkecil sementara
    for (int j = i + 1; j < n; j++) { // Mencari elemen terkecil di sisa array
      if (arr[j] < arr[min_idx]) { // Jika menemukan string yang lebih kecil secara alfabetis
        min_idx = j; // Update indeks elemen terkecil
      }
    }
    if (min_idx != i) { // Jika indeks terkecil berubah, lakukan penukaran posisi (swap)
      swap(arr[i], arr[min_idx]);
    }
  }
}

// Variabel buffer global untuk menyimpan teks hasil yang akan dikirim ke Python
// Ini digunakan karena mengembalikan pointer ke memori lokal akan menyebabkan
// error di C++
static char result_buffer[4096];

// Membuat block extern "C" agar C++ compiler tidak melakukan name-mangling
// Hal ini sangat penting agar library ctypes dari Python bisa menemukan nama
// fungsi secara spesifik
extern "C" {
// Fungsi wrapper yang diekspor ke dalam DLL (Dynamic Link Library)
// __declspec(dllexport) adalah perintah khusus Windows untuk membuat fungsi ini
// bisa diakses dari luar
// Sekarang menerima parameter tambahan 'algo' untuk memilih BFS atau DFS
__declspec(dllexport) const char *cari_rute_wrapper(const char *asal,
                                                    const char *tujuan,
                                                    const char *algo) {
  // Mengubah parameter tipe C-string (const char*) menjadi C++ std::string agar
  // mudah diproses
  string str_asal(asal);
  string str_tujuan(tujuan);
  string str_algo(algo);

  // Membuat objek dari class Graph yang sudah kita definisikan sebelumnya
  Graph g;

  // Menambahkan rute penerbangan antar kota (menginisialisasi data dasar)
  g.addEdge("Jakarta", "Singapura"); // Menambahkan rute Jakarta - Singapura
  g.addEdge("Singapura", "Dubai");   // Menambahkan rute Singapura - Dubai
  g.addEdge("Dubai", "Paris");       // Menambahkan rute Dubai - Paris
  g.addEdge("Jakarta", "Tokyo");     // Menambahkan rute Jakarta - Tokyo
  g.addEdge("Tokyo", "Paris");       // Menambahkan rute Tokyo - Paris

  string hasil;
  // Menentukan algoritma mana yang akan digunakan berdasarkan parameter 'algo'
  if (str_algo == "DFS") {
    // Memanggil fungsi DFS jika parameter algo bernilai "DFS"
    hasil = g.DFS(str_asal, str_tujuan);
  } else {
    // Secara default, memanggil fungsi BFS
    hasil = g.BFS(str_asal, str_tujuan);
  }

  // Menyalin string hasil C++ ke dalam buffer statis C agar aman dikirim dan
  // dibaca oleh Python memset memastikan sisa memori di buffer menjadi kosong
  // (null terminator)
  memset(result_buffer, 0, sizeof(result_buffer));
#ifdef _WIN32
  // Menggunakan strncpy_s jika dicompile di Windows agar lebih aman
  strncpy_s(result_buffer, sizeof(result_buffer), hasil.c_str(), _TRUNCATE);
#else
  // Menggunakan strncpy biasa (meskipun kita utamanya di Windows)
  strncpy(result_buffer, hasil.c_str(), sizeof(result_buffer) - 1);
#endif

  // Mengembalikan pointer memori statis tersebut ke pemanggil (Python)
  return result_buffer;
}

// Fungsi wrapper baru untuk mengambil seluruh daftar kota unik yang terurut secara alfabetis
__declspec(dllexport) const char *get_all_cities_sorted_wrapper() {
  // Membuat objek dari class Graph
  Graph g;

  // Menambahkan rute penerbangan antar kota (inisialisasi data yang sama agar konsisten)
  g.addEdge("Jakarta", "Singapura");
  g.addEdge("Singapura", "Dubai");
  g.addEdge("Dubai", "Paris");
  g.addEdge("Jakarta", "Tokyo");
  g.addEdge("Tokyo", "Paris");

  // Mendapatkan daftar kota unik dari graf
  vector<string> cities = g.getUniqueCities();

  // Mengurutkan kota secara alfabetis menggunakan Selection Sort kustom
  selectionSort(cities);

  // Menggabungkan nama-nama kota dengan pembatas titik koma (;) agar mudah di-parse di Python/Dart
  string hasil = "";
  for (size_t i = 0; i < cities.size(); i++) {
    hasil += cities[i];
    if (i != cities.size() - 1) {
      hasil += ";";
    }
  }

  // Menyalin string hasil ke buffer statis
  memset(result_buffer, 0, sizeof(result_buffer));
#ifdef _WIN32
  strncpy_s(result_buffer, sizeof(result_buffer), hasil.c_str(), _TRUNCATE);
#else
  strncpy(result_buffer, hasil.c_str(), sizeof(result_buffer) - 1);
#endif

  // Mengembalikan data kota terurut
  return result_buffer;
}
}
