#include <cstring>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cin;
using std::cout;
using std::getline;
using std::pair;
using std::size_t;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// ============================================================================
// KELAS GRAPH - Implementasi Struktur Data Graph untuk Travel Planner AI
// ============================================================================
// Konsep: Graph adalah struktur data yang terdiri dari vertex (kota) dan 
// edge (jalur) yang menghubungkan kota-kota tersebut. Kami menggunakan 
// Adjacency List untuk representasi yang efisien dalam hal ruang memori.
// ============================================================================
class Graph {
private:
  // Adjacency List: Map kota -> vektor kota tetangga
  // Kompleksitas ruang: O(V + E) dimana V = jumlah vertex, E = jumlah edge
  unordered_map<string, vector<string>> adjacency_list;
  
  // Edge List: Menyimpan semua jalur untuk kemudahan visualisasi
  vector<pair<string, string>> edge_list;

  // ========================================================================
  // FUNGSI HELPER: Menggabungkan path kota menjadi format "A -> B -> C"
  // Kompleksitas: O(n) dimana n adalah jumlah kota dalam path
  // ========================================================================
  static string joinPath(const vector<string> &path) {
    string result;
    for (size_t i = 0; i < path.size(); ++i) {
      result += path[i];
      if (i + 1 < path.size()) {
        result += " -> ";
      }
    }
    return result;
  }

public:
  // ========================================================================
  // MENAMBAH EDGE (Jalur dua arah tidak terarah / Undirected Graph)
  // Kompleksitas: O(1) amortized
  // ========================================================================
  void addEdge(const string &from, const string &to) {
    adjacency_list[from].push_back(to);
    adjacency_list[to].push_back(from);
    edge_list.push_back({from, to});
  }

  // ========================================================================
  // VALIDASI: Mengecek apakah kota ada di dalam graph
  // Kompleksitas: O(1) rata-rata untuk unordered_map
  // ========================================================================
  bool containsCity(const string &city) const {
    return adjacency_list.find(city) != adjacency_list.end();
  }

  // ========================================================================
  // GET UNIQUE CITIES: Mengambil semua kota unik dari graph
  // Kompleksitas: O(V) dimana V adalah jumlah vertex
  // ========================================================================
  vector<string> getUniqueCities() const {
    vector<string> cities;
    for (const auto &[city, neighbors] : adjacency_list) {
      (void)neighbors;
      cities.push_back(city);
    }
    return cities;
  }

  // ========================================================================
  // GET SORTED CITIES: Mengurutkan kota secara alfabetis
  // Menggunakan Selection Sort dengan kompleksitas O(V^2)
  // ========================================================================
  vector<string> getSortedCities() const {
    vector<string> cities = getUniqueCities();
    selectionSort(cities);
    return cities;
  }

  // ========================================================================
  // BIG O NOTATION SUMMARY: Memberikan analisis kompleksitas waktu
  // untuk semua operasi yang tersedia dalam aplikasi
  // ========================================================================
  string getBigONotationSummary() const {
    return "Big O Notasi:\n"
           "- Simpan graph: O(V + E)\n"
           "- BFS: O(V + E)\n"
           "- DFS: O(V + E)\n"
           "- Linear Search kota: O(V)\n"
           "- Selection Sort: O(V^2)";
  }

  // ========================================================================
  // SELECTION SORT: Algoritma Sorting untuk mengurutkan nama kota
  // Cara kerja: 
  //   1. Temukan elemen terkecil dalam bagian yang belum terurut
  //   2. Tukar dengan elemen pertama dari bagian yang belum terurut
  //   3. Ulangi sampai seluruh array terurut
  // Kompleksitas: O(V^2) - Kuadratik, cocok untuk dataset kecil
  // Space: O(1) - In-place sorting, tidak perlu ruang ekstra
  // ========================================================================
  static void selectionSort(vector<string> &arr) {
    const size_t n = arr.size();
    // Loop eksternal: iterasi untuk setiap posisi
    for (size_t i = 0; i + 1 < n; ++i) {
      size_t min_index = i;
      // Loop internal: cari elemen minimum dari posisi i hingga akhir
      for (size_t j = i + 1; j < n; ++j) {
        if (arr[j] < arr[min_index]) {
          min_index = j;
        }
      }
      // Tukar elemen minimum dengan elemen di posisi i
      if (min_index != i) {
        std::swap(arr[i], arr[min_index]);
      }
    }
  }

  // ========================================================================
  // SEARCHING: Linear Search untuk menemukan kota berdasarkan keyword
  // Cara kerja: Cek setiap kota sampai ditemukan atau habis
  // Kompleksitas: O(V) - Linear search, memeriksa setiap vertex
  // ========================================================================
  string searchCity(const string &keyword) const {
    vector<string> cities = getUniqueCities();
    // Iterasi setiap kota untuk pencarian
    for (const string &city : cities) {
      if (city == keyword) {
        return "Searching ditemukan: " + city;
      }
    }
    return "Searching tidak menemukan kota: " + keyword;
  }

  // ========================================================================
  // BREADTH FIRST SEARCH (BFS) - Algoritma Pencarian Grafik
  // ========================================================================
  // Cara kerja BFS:
  //   1. Mulai dari vertex awal (start)
  //   2. Gunakan QUEUE untuk menyimpan path yang akan dieksplorasi
  //   3. Eksplorasi tetangga level demi level (breadth-wise)
  //   4. Tandai vertex yang sudah dikunjungi untuk menghindari loop
  //   5. Jika target ditemukan, return path
  // 
  // Karakteristik:
  //   - Menemukan shortest path (jalur terpendek)
  //   - Optimal untuk graph yang tidak weighted
  //   - Kompleksitas: O(V + E)
  //   - Space: O(V) untuk visited set dan queue
  // ========================================================================
  string BFS(const string &start, const string &target) const {
    // Validasi input: pastikan kota asal dan tujuan ada
    if (!containsCity(start) || !containsCity(target)) {
      return "Rute tidak ditemukan: kota asal atau tujuan tidak ada dalam graph";
    }

    // Queue untuk BFS: menyimpan path (urutan kota) yang sedang dieksplorasi
    std::queue<vector<string>> q;
    // Set untuk tracking vertex yang sudah dikunjungi
    unordered_set<string> visited;
    
    // Inisialisasi: masukkan path awal hanya berisi start city
    q.push({start});
    visited.insert(start);

    // Loop utama BFS
    while (!q.empty()) {
      // Ambil path paling awal dari queue (FIFO)
      vector<string> path = q.front();
      q.pop();

      // Ambil kota terakhir dari path (kota current yang sedang dieksplorasi)
      const string current = path.back();
      
      // Jika current adalah target, berarti sudah menemukan rute
      if (current == target) {
        return "Rute BFS ditemukan: " + joinPath(path);
      }

      // Cari tetangga dari current city
      const auto it = adjacency_list.find(current);
      if (it == adjacency_list.end()) {
        continue;
      }

      // Eksplorasi semua tetangga yang belum dikunjungi
      for (const string &neighbor : it->second) {
        if (visited.find(neighbor) == visited.end()) {
          visited.insert(neighbor);
          // Buat path baru dengan menambahkan neighbor
          vector<string> new_path = path;
          new_path.push_back(neighbor);
          // Masukkan path baru ke queue untuk dieksplorasi
          q.push(new_path);
        }
      }
    }
    return "Rute tidak ditemukan.";
  }

  // ========================================================================
  // DEPTH FIRST SEARCH (DFS) - Algoritma Pencarian Grafik
  // ========================================================================
  // Cara kerja DFS:
  //   1. Mulai dari vertex awal (start)
  //   2. Gunakan STACK untuk menyimpan path yang akan dieksplorasi
  //   3. Eksplorasi tetangga secara mendalam (depth-wise)
  //   4. Tandai vertex yang sudah dikunjungi untuk menghindari loop
  //   5. Jika target ditemukan, return path
  // 
  // Karakteristik:
  //   - Eksplorasi lebih dalam sebelum mundur (backtrack)
  //   - Berguna untuk topological sorting dan cycle detection
  //   - Kompleksitas: O(V + E)
  //   - Space: O(V) untuk visited set dan stack
  // ========================================================================
  string DFS(const string &start, const string &target) const {
    // Validasi input: pastikan kota asal dan tujuan ada
    if (!containsCity(start) || !containsCity(target)) {
      return "Rute tidak ditemukan: kota asal atau tujuan tidak ada dalam graph";
    }

    // Stack untuk DFS: menyimpan path (urutan kota) yang sedang dieksplorasi
    vector<vector<string>> stack_paths;
    // Set untuk tracking vertex yang sudah dikunjungi
    unordered_set<string> visited;
    
    // Inisialisasi: masukkan path awal hanya berisi start city
    stack_paths.push_back({start});

    // Loop utama DFS
    while (!stack_paths.empty()) {
      // Ambil path paling atas dari stack (LIFO)
      vector<string> path = stack_paths.back();
      stack_paths.pop_back();

      // Ambil kota terakhir dari path (kota current yang sedang dieksplorasi)
      const string current = path.back();
      
      // Jika current adalah target, berarti sudah menemukan rute
      if (current == target) {
        return "Rute DFS ditemukan: " + joinPath(path);
      }

      // Cek apakah kota sudah dikunjungi
      if (visited.find(current) != visited.end()) {
        continue;
      }
      visited.insert(current);

      // Cari tetangga dari current city
      const auto it = adjacency_list.find(current);
      if (it == adjacency_list.end()) {
        continue;
      }

      // Eksplorasi semua tetangga secara mendalam (reverse order untuk LIFO)
      for (auto rit = it->second.rbegin(); rit != it->second.rend(); ++rit) {
        if (visited.find(*rit) == visited.end()) {
          // Buat path baru dengan menambahkan tetangga
          vector<string> new_path = path;
          new_path.push_back(*rit);
          // Masukkan path baru ke stack untuk dieksplorasi
          stack_paths.push_back(new_path);
        }
      }
    }
    return "Rute tidak ditemukan.";
  }

  // ========================================================================
  // GET GRAPH SUMMARY: Menampilkan semua edge dalam graph
  // Digunakan untuk visualisasi struktur graph
  // ========================================================================
  string getGraphSummary() const {
    string result = "Data graph AI Travel Planner:\n";
    for (const auto &[from, to] : edge_list) {
      result += "- " + from + " <-> " + to + "\n";
    }
    return result;
  }
};

// ============================================================================
// BUILD SAMPLE GRAPH: Membuat graph dengan data kota dan rute travel
// ============================================================================
// Graph diinisialisasi dengan kota-kota destinasi wisata terkenal dan
// jalur penerbangan yang menghubungkan mereka. Ini adalah contoh graph
// tak berarah (undirected graph) karena jalur bisa dilalui dua arah.
// ============================================================================
static Graph buildSampleGraph() {
  Graph g;
  // Menambahkan edge (jalur) antar kota
  g.addEdge("Jakarta", "Singapura");
  g.addEdge("Singapura", "Dubai");
  g.addEdge("Dubai", "Paris");
  g.addEdge("Jakarta", "Tokyo");
  g.addEdge("Tokyo", "Paris");
  g.addEdge("Singapura", "Sydney");
  g.addEdge("Sydney", "Tokyo");
  return g;
}

// ============================================================================
// THREAD-LOCAL STORAGE: Menyimpan hasil terakhir untuk dikembalikan ke Python
// Menggunakan thread_local untuk thread-safety dalam lingkungan multi-threaded
// ============================================================================
static thread_local std::string last_result;

// ============================================================================
// C WRAPPER FUNCTIONS: Interface untuk Python ctypes
// Semua fungsi di-export dengan __declspec(dllexport) untuk DLL
// ============================================================================

// ========================================================================
// CARI_RUTE_WRAPPER: Mencari rute antar kota menggunakan BFS atau DFS
// Parameter:
//   - asal: nama kota keberangkatan
//   - tujuan: nama kota tujuan
//   - algo: algoritma pencarian ("BFS" atau "DFS")
// Return: pointer ke string hasil path atau pesan error
// ========================================================================
extern "C" {
__declspec(dllexport) const char *cari_rute_wrapper(const char *asal,
                                                    const char *tujuan,
                                                    const char *algo) {
  Graph g = buildSampleGraph();
  const string str_asal = asal ? asal : "";
  const string str_tujuan = tujuan ? tujuan : "";
  const string str_algo = algo ? algo : "BFS";

  // Pilih algoritma berdasarkan parameter algo
  if (str_algo == "DFS") {
    last_result = g.DFS(str_asal, str_tujuan);
  } else {
    last_result = g.BFS(str_asal, str_tujuan);
  }
  return last_result.c_str();
}

// ========================================================================
// GET_ALL_CITIES_SORTED_WRAPPER: Mendapatkan daftar kota terurut A-Z
// Return: pointer ke string berisi daftar kota dipisahkan semicolon (;)
// ========================================================================
__declspec(dllexport) const char *get_all_cities_sorted_wrapper() {
  Graph g = buildSampleGraph();
  vector<string> cities = g.getSortedCities();
  last_result.clear();
  // Gabungkan kota dengan semicolon sebagai delimiter
  for (size_t i = 0; i < cities.size(); ++i) {
    last_result += cities[i];
    if (i + 1 < cities.size()) {
      last_result += ";";
    }
  }
  return last_result.c_str();
}

// ========================================================================
// SEARCH_CITY_WRAPPER: Mencari kota berdasarkan keyword menggunakan Linear Search
// Parameter:
//   - keyword: nama kota yang dicari
// Return: pointer ke string hasil pencarian
// ========================================================================
__declspec(dllexport) const char *search_city_wrapper(const char *keyword) {
  Graph g = buildSampleGraph();
  last_result = g.searchCity(keyword ? keyword : "");
  return last_result.c_str();
}

// ========================================================================
// BIG_O_WRAPPER: Mendapatkan penjelasan Big O Notation
// Return: pointer ke string berisi analisis kompleksitas waktu
// ========================================================================
__declspec(dllexport) const char *big_o_wrapper() {
  Graph g = buildSampleGraph();
  last_result = g.getBigONotationSummary();
  return last_result.c_str();
}
}

// ============================================================================
// MENU HELPER: Menampilkan menu interaktif untuk demo lokal
// ============================================================================
static void printMenu() {
  cout << "\n=== BackpackAI - Local C++ Demo ===\n";
  cout << "1. Tampilkan graph\n";
  cout << "2. BFS cari rute\n";
  cout << "3. DFS cari rute\n";
  cout << "4. Searching kota\n";
  cout << "5. Sorting kota A-Z\n";
  cout << "6. Big O notasi\n";
  cout << "0. Keluar\n";
  cout << "Pilih: ";
}

// ============================================================================
// MAIN FUNCTION: Program interaktif untuk testing algoritma secara lokal
// ============================================================================
// Fungsi main menyediakan interface command-line untuk menguji semua fitur:
// - Visualisasi graph
// - Pencarian rute dengan BFS dan DFS
// - Searching kota
// - Sorting kota
// - Penjelasan Big O Notation
// ============================================================================
int main() {
  Graph g = buildSampleGraph();
  int choice = -1;

  do {
    printMenu();
    cin >> choice;
    // Clear input buffer untuk menghindari bug input
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 1) {
      // Opsi 1: Tampilkan struktur graph
      cout << g.getGraphSummary();
    } else if (choice == 2 || choice == 3) {
      // Opsi 2: BFS, Opsi 3: DFS
      string asal, tujuan;
      cout << "Masukkan kota asal: ";
      getline(cin, asal);
      cout << "Masukkan kota tujuan: ";
      getline(cin, tujuan);
      // Panggil BFS atau DFS berdasarkan choice
      cout << (choice == 2 ? g.BFS(asal, tujuan) : g.DFS(asal, tujuan)) << '\n';
    } else if (choice == 4) {
      // Opsi 4: Searching kota
      string keyword;
      cout << "Masukkan nama kota: ";
      getline(cin, keyword);
      cout << g.searchCity(keyword) << '\n';
    } else if (choice == 5) {
      // Opsi 5: Sorting kota (Selection Sort)
      vector<string> cities = g.getSortedCities();
      cout << "Kota terurut: ";
      for (size_t i = 0; i < cities.size(); ++i) {
        cout << cities[i];
        if (i + 1 < cities.size()) {
          cout << ", ";
        }
      }
      cout << '\n';
    } else if (choice == 6) {
      // Opsi 6: Tampilkan Big O Notation
      cout << g.getBigONotationSummary() << '\n';
    }
  } while (choice != 0);

  return 0;
}
