#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cctype>
using namespace std;

const int MAKS = 20;      // kapasitas maksimum film
const int BATAS_EDGE = 3; // skor minimal agar dua film dianggap terhubung

// data untuk satu film
struct Film {
    string judul;
    string genre;
    string artis;
    double rating;
    int tahun;
};

// graph: node = film (indeks array), edge = tingkat kemiripan (bobot)
// disimpan dengan adjacency matrix berbobot (0 = tidak ada edge)
struct Graph {
    int n;
    int bobot[MAKS][MAKS];
};


// membersihkan layar (Windows / Linux / Mac)
void bersihLayar() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// fungsi untuk mengubah semua huruf menjadi huruf kecil
string kecil(string teks) {
    for (int i = 0; i < (int)teks.length(); i++) {
        teks[i] = tolower(teks[i]);
    }
    return teks;
}

// cetak detail satu film
void cetakFilm(const Film &f) {
    cout << "   Judul  : " << f.judul << endl;
    cout << "   Genre  : " << f.genre << endl;
    cout << "   Artis  : " << f.artis << endl;
    cout << "   Rating : " << f.rating << endl;
    cout << "   Tahun  : " << f.tahun << endl;
}



// fungsi untuk menampilkan semua film
void tampilkan(Film data[], int n) {
    for (int i = 0; i < n; i++) {
        cout << i + 1 << ". " << data[i].judul << endl;
        cout << "   Genre  : " << data[i].genre << endl;
        cout << "   Artis  : " << data[i].artis << endl;
        cout << "   Rating : " << data[i].rating << endl;
        cout << "   Tahun  : " << data[i].tahun << endl;
        cout << endl;
    }
}



// bubble sort berdasarkan rating (dari tertinggi ke terendah)
void sortRating(Film data[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (data[j].rating < data[j + 1].rating) {
                swap(data[j], data[j + 1]);
            }
        }
    }
}

// insertion sort berdasarkan tahun rilis (dari terbaru ke terlama)
void sortTahun(Film data[], int n) {
    for (int i = 1; i < n; i++) {
        Film key = data[i];
        int j = i - 1;

        while (j >= 0 && data[j].tahun < key.tahun) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}



// linear search berdasarkan judul
void cariJudul(Film data[], int n, string target) {
    bool ketemu = false;

    for (int i = 0; i < n; i++) {
        if (kecil(data[i].judul).find(kecil(target)) != string::npos) {
            cout << "Film ditemukan pada indeks : " << i << endl;
            cetakFilm(data[i]);
            cout << endl;
            ketemu = true;
        }
    }

    if (!ketemu) {
        cout << "Film tidak ditemukan" << endl;
    }
}

// linear search berdasarkan genre
void cariGenre(Film data[], int n, string target) {
    bool ketemu = false;

    for (int i = 0; i < n; i++) {
        if (kecil(data[i].genre) == kecil(target)) {
            cout << "- " << data[i].judul << " (Rating: " << data[i].rating
                 << ", Tahun: " << data[i].tahun << ")" << endl;
            ketemu = true;
        }
    }

    if (!ketemu) {
        cout << "Genre tidak ditemukan" << endl;
    }
}

// linear search berdasarkan artis
void cariArtis(Film data[], int n, string target) {
    bool ketemu = false;

    for (int i = 0; i < n; i++) {
        if (kecil(data[i].artis).find(kecil(target)) != string::npos) {
            cout << "- " << data[i].judul << " (" << data[i].artis
                 << ", Rating: " << data[i].rating
                 << ", Tahun: " << data[i].tahun << ")" << endl;
            ketemu = true;
        }
    }

    if (!ketemu) {
        cout << "Artis tidak ditemukan" << endl;
    }
}

// mengembalikan indeks film pertama yang judulnya cocok, atau -1
int cariIndeks(Film data[], int n, string target) {
    for (int i = 0; i < n; i++) {
        if (kecil(data[i].judul).find(kecil(target)) != string::npos) {
            return i;
        }
    }
    return -1;
}



// menghitung skor kemiripan dua film
// genre sama = +3, artis sama = +3, selisih rating <= 0.5 = +1, selisih tahun <= 3 = +1
int hitungKemiripan(Film a, Film b) {
    int skor = 0;
    if (kecil(a.genre) == kecil(b.genre)) skor += 3;
    if (kecil(a.artis) == kecil(b.artis)) skor += 3;

    double selisihRating = a.rating - b.rating;
    if (selisihRating < 0) selisihRating = -selisihRating;
    if (selisihRating <= 0.5) skor += 1;

    int selisihTahun = a.tahun - b.tahun;
    if (selisihTahun < 0) selisihTahun = -selisihTahun;
    if (selisihTahun <= 3) skor += 1;

    return skor;
}

// membangun graph dari data film saat ini
// (dipanggil ulang setiap kali dipakai, karena urutan array bisa berubah akibat sorting)
void bangunGraph(Graph &g, Film data[], int n) {
    g.n = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g.bobot[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int skor = hitungKemiripan(data[i], data[j]);
            if (skor >= BATAS_EDGE) {
                g.bobot[i][j] = skor;
                g.bobot[j][i] = skor; // graph tidak berarah
            }
        }
    }
}

// menampilkan graph dalam bentuk adjacency list
void tampilkanGraph(Graph &g, Film data[]) {
    cout << "Node = film, Edge = tingkat kemiripan (bobot)" << endl;
    cout << "Edge dibuat jika skor kemiripan >= " << BATAS_EDGE << endl;
    cout << "(genre sama +3, artis sama +3, selisih rating <= 0.5 +1, selisih tahun <= 3 +1)" << endl << endl;

    // cari judul terpanjang supaya tanda "->" sejajar
    int lebar = 0;
    for (int i = 0; i < g.n; i++) {
        if ((int)data[i].judul.length() > lebar) {
            lebar = data[i].judul.length();
        }
    }

    int totalEdge = 0;
    for (int i = 0; i < g.n; i++) {
        cout << left << setw(lebar) << data[i].judul << " -> ";
        bool adaTetangga = false;
        for (int j = 0; j < g.n; j++) {
            if (g.bobot[i][j] > 0) {
                if (adaTetangga) cout << ", ";
                cout << data[j].judul << " [" << g.bobot[i][j] << "]";
                adaTetangga = true;
                if (i < j) totalEdge++;
            }
        }
        if (!adaTetangga) cout << "(tidak ada)";
        cout << endl;
    }
    cout << endl << "Jumlah node : " << g.n << endl;
    cout << "Jumlah edge : " << totalEdge << endl;
}

// menampilkan alasan kenapa dua film mirip
void cetakAlasan(Film a, Film b) {
    bool pertama = true;
    if (kecil(a.genre) == kecil(b.genre)) {
        cout << "genre sama (" << a.genre << ")";
        pertama = false;
    }
    if (kecil(a.artis) == kecil(b.artis)) {
        if (!pertama) cout << ", ";
        cout << "artis sama (" << a.artis << ")";
        pertama = false;
    }
    double s = a.rating - b.rating;
    if (s < 0) s = -s;
    if (s <= 0.5) {
        if (!pertama) cout << ", ";
        cout << "rating mirip";
        pertama = false;
    }
    int t = a.tahun - b.tahun;
    if (t < 0) t = -t;
    if (t <= 3) {
        if (!pertama) cout << ", ";
        cout << "tahun rilis berdekatan";
    }
}

// rekomendasi film mirip: tetangga dari node film yang dipilih,
// diurutkan dari bobot terbesar (bubble sort), jika sama urut rating tertinggi
void rekomendasiGraph(Graph &g, Film data[], int idx) {
    int tetangga[MAKS];
    int jumlah = 0;

    for (int j = 0; j < g.n; j++) {
        if (g.bobot[idx][j] > 0) {
            tetangga[jumlah++] = j;
        }
    }

    if (jumlah == 0) {
        cout << "Tidak ada film yang mirip dengan " << data[idx].judul << endl;
        return;
    }

    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - 1 - i; j++) {
            int a = tetangga[j];
            int b = tetangga[j + 1];
            bool tukar = false;
            if (g.bobot[idx][a] < g.bobot[idx][b]) tukar = true;
            else if (g.bobot[idx][a] == g.bobot[idx][b] && data[a].rating < data[b].rating) tukar = true;
            if (tukar) swap(tetangga[j], tetangga[j + 1]);
        }
    }

    cout << "Karena kamu suka \"" << data[idx].judul << "\", kamu mungkin suka:" << endl << endl;
    for (int i = 0; i < jumlah; i++) {
        int j = tetangga[i];
        cout << i + 1 << ". " << data[j].judul << " (Rating: " << data[j].rating
             << ", Tahun: " << data[j].tahun << ")" << endl;
        cout << "   Kemiripan : " << g.bobot[idx][j] << " -> ";
        cetakAlasan(data[idx], data[j]);
        cout << endl << endl;
    }
}

// penelusuran BFS dari film tertentu (menunjukkan film yang "terjangkau")
void bfs(Graph &g, Film data[], int mulai) {
    bool dikunjungi[MAKS] = {false};
    int level[MAKS] = {0};
    int antrian[MAKS];
    int depan = 0, belakang = 0;

    antrian[belakang++] = mulai;
    dikunjungi[mulai] = true;

    while (depan < belakang) {
        int u = antrian[depan++];
        cout << "  Level " << level[u] << " : " << data[u].judul << endl;

        for (int v = 0; v < g.n; v++) {
            if (g.bobot[u][v] > 0 && !dikunjungi[v]) {
                dikunjungi[v] = true;
                level[v] = level[u] + 1;
                antrian[belakang++] = v;
            }
        }
    }
}



int main() {
    Film data[MAKS] = {
        {"Inception",           "Sci-Fi",  "Leonardo DiCaprio",   8.8, 2010},
        {"Interstellar",        "Sci-Fi",  "Matthew McConaughey", 8.7, 2014},
        {"The Dark Knight",     "Action",  "Christian Bale",      9.0, 2008},
        {"Avengers Endgame",    "Action",  "Robert Downey Jr.",   8.4, 2019},
        {"Titanic",             "Romance", "Kate Winslet",        7.9, 1997},
        {"La La Land",          "Romance", "Emma Stone",          8.0, 2016},
        {"Joker",               "Drama",   "Joaquin Phoenix",     8.4, 2019},
        {"Parasite",            "Drama",   "Song Kang-ho",        8.5, 2019},
        {"The Prestige",        "Sci-Fi",  "Christian Bale",      8.5, 2006},
        {"The Revenant",        "Drama",   "Leonardo DiCaprio",   8.0, 2015},
        {"Iron Man",            "Action",  "Robert Downey Jr.",   7.9, 2008},
        {"Dune",                "Sci-Fi",  "Timothee Chalamet",   8.0, 2021}
    };
    int n = 12;
    int pilihan;
    Graph graph;

    do {
        bersihLayar();
        cout << "=== SISTEM REKOMENDASI FILM ===" << endl;
        cout << "1. Tampilkan semua film" << endl;
        cout << "2. Urutkan berdasarkan rating (tertinggi)" << endl;
        cout << "3. Urutkan berdasarkan tahun rilis (terbaru)" << endl;
        cout << "4. Cari film berdasarkan judul" << endl;
        cout << "5. Cari film berdasarkan genre" << endl;
        cout << "6. Cari film berdasarkan artis" << endl;
        cout << "7. Tampilkan graph kemiripan film" << endl;
        cout << "8. Rekomendasi film mirip (berdasarkan graph)" << endl;
        cout << "9. Telusuri graph dari satu film (BFS)" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilihan : ";

        if (!(cin >> pilihan)) {
            cin.clear();
            pilihan = -1;
        }
        cin.ignore(10000, '\n');

        bersihLayar();

        if (pilihan == 1) {
            cout << "=== DAFTAR FILM ===" << endl << endl;
            tampilkan(data, n);
        } else if (pilihan == 2) {
            sortRating(data, n);
            cout << "=== FILM BERDASARKAN RATING (Bubble Sort) ===" << endl << endl;
            tampilkan(data, n);
        } else if (pilihan == 3) {
            sortTahun(data, n);
            cout << "=== FILM BERDASARKAN TAHUN (Insertion Sort) ===" << endl << endl;
            tampilkan(data, n);
        } else if (pilihan == 4) {
            string judul;
            cout << "Masukkan judul film : ";
            getline(cin, judul);
            cout << endl;
            cariJudul(data, n, judul);
        } else if (pilihan == 5) {
            string genre;
            cout << "Genre yang tersedia: Sci-Fi, Action, Romance, Drama" << endl;
            cout << "Masukkan genre : ";
            getline(cin, genre);
            cout << endl;
            cout << "Rekomendasi film genre " << genre << " :" << endl;
            cariGenre(data, n, genre);
        } else if (pilihan == 6) {
            string artis;
            cout << "Masukkan nama artis : ";
            getline(cin, artis);
            cout << endl;
            cout << "Film dengan artis " << artis << " :" << endl;
            cariArtis(data, n, artis);
        } else if (pilihan == 7) {
            cout << "=== GRAPH KEMIRIPAN FILM ===" << endl << endl;
            bangunGraph(graph, data, n);
            tampilkanGraph(graph, data);
        } else if (pilihan == 8) {
            string judul;
            cout << "=== REKOMENDASI FILM MIRIP ===" << endl;
            cout << "Masukkan judul film yang kamu suka : ";
            getline(cin, judul);
            cout << endl;
            int idx = cariIndeks(data, n, judul);
            if (idx == -1) {
                cout << "Film tidak ditemukan" << endl;
            } else {
                bangunGraph(graph, data, n);
                rekomendasiGraph(graph, data, idx);
            }
        } else if (pilihan == 9) {
            string judul;
            cout << "=== PENELUSURAN GRAPH (BFS) ===" << endl;
            cout << "Masukkan judul film awal : ";
            getline(cin, judul);
            cout << endl;
            int idx = cariIndeks(data, n, judul);
            if (idx == -1) {
                cout << "Film tidak ditemukan" << endl;
            } else {
                bangunGraph(graph, data, n);
                cout << "Film yang terjangkau dari \"" << data[idx].judul << "\":" << endl;
                bfs(graph, data, idx);
            }
        } else if (pilihan == 0) {
            cout << "Terima kasih!" << endl;
        } else {
            cout << "Pilihan tidak valid" << endl;
        }

        if (pilihan != 0) {
            cout << "\nTekan Enter untuk kembali ke menu...";
            cin.get();
        }

    } while (pilihan != 0);

    return 0;
}