#include <iostream>
#include <string>
using namespace std;

// data untuk satu film
struct Film {
    string judul;
    string genre;
    string artis;
    double rating;
    int tahun;
};

// fungsi untuk mengubah semua huruf menjadi huruf kecil
string kecil(string teks) {
    for (int i = 0; i < teks.length(); i++) {
        teks[i] = tolower(teks[i]);
    }
    return teks;
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
            cout << "   Judul  : " << data[i].judul << endl;
            cout << "   Genre  : " << data[i].genre << endl;
            cout << "   Artis  : " << data[i].artis << endl;
            cout << "   Rating : " << data[i].rating << endl;
            cout << "   Tahun  : " << data[i].tahun << endl;
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

int main() {
    Film data[10] = {
        {"Inception",        "Sci-Fi",  "Leonardo DiCaprio", 8.8, 2010},
        {"Interstellar",     "Sci-Fi",  "Matthew McConaughey", 8.7, 2014},
        {"The Dark Knight",  "Action",  "Christian Bale",    9.0, 2008},
        {"Avengers Endgame", "Action",  "Robert Downey Jr.", 8.4, 2019},
        {"Titanic",          "Romance", "Kate Winslet",      7.9, 1997},
        {"La La Land",       "Romance", "Emma Stone",        8.0, 2016},
        {"Joker",            "Drama",   "Joaquin Phoenix",   8.4, 2019},
        {"Parasite",         "Drama",   "Song Kang-ho",      8.5, 2019}
    };
    int n = 8;
    int pilihan;

    do {
        system("cls");
        cout << "=== SISTEM REKOMENDASI FILM ===" << endl;
        cout << "1. Tampilkan semua film" << endl;
        cout << "2. Urutkan berdasarkan rating (tertinggi)" << endl;
        cout << "3. Urutkan berdasarkan tahun rilis (terbaru)" << endl;
        cout << "4. Cari film berdasarkan judul" << endl;
        cout << "5. Cari film berdasarkan genre" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilihan : ";
        cin >> pilihan;
        cin.ignore();

        system("cls");

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