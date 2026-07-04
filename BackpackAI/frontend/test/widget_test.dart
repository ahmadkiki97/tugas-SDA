import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';

import 'package:frontend/main.dart';

void main() {
  testWidgets('BackpackHomePage UI rendering test', (WidgetTester tester) async {
    // Membangun widget MyApp ke dalam simulator pengujian
    await tester.pumpWidget(const MyApp());

    // Memverifikasi apakah AppBar menampilkan judul yang benar
    expect(find.text('BackpackAI Cyberpunk'), findsOneWidget);

    // Memverifikasi apakah teks petunjuk utama "Cari Rute Destinasi Anda" ada di layar
    expect(find.text('Cari Rute Destinasi Anda'), findsOneWidget);

    // Memverifikasi keberadaan tombol "Cari Rute"
    expect(find.text('Cari Rute'), findsOneWidget);

    // Memverifikasi keberadaan pilihan algoritma BFS dan DFS
    expect(find.text('BFS (Terpendek)'), findsOneWidget);
    expect(find.text('DFS (Mendalam)'), findsOneWidget);

    // Memverifikasi status instruksi awal sebelum pencarian dilakukan
    expect(find.text('Silakan pilih kota dan algoritma, lalu klik tombol Cari Rute.'), findsOneWidget);
  });
}
