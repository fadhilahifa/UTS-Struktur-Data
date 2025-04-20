#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Paper {
    string judul;
    int tahun;
    string penulis;
};

// Fungsi pembanding untuk sort manual
bool bandingTahun(const Paper& a, const Paper& b) {
    return a.tahun < b.tahun;
}

bool bandingJudul(const Paper& a, const Paper& b) {
    return a.judul < b.judul;
}

bool bandingPenulis(const Paper& a, const Paper& b) {
    return a.penulis < b.penulis;
}

// Fungsi untuk mengubah int ke string (tanpa to_string)
string intToString(int num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

// Fungsi untuk parsing CSV baris dengan koma di dalam kutipan
vector<string> parseCSV(const string& line) {
    vector<string> result;
    string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];

        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            result.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    result.push_back(field); // Tambahkan field terakhir
    return result;
}

// Fungsi untuk membaca CSV
vector<Paper> loadCSV(const string& filename) {
    vector<Paper> data;
    ifstream file(filename.c_str());
    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        vector<string> fields = parseCSV(line);
        if (fields.size() >= 8) {
            Paper p;
            p.judul = fields[5];
            stringstream ss(fields[6]);
            ss >> p.tahun;
            p.penulis = fields[7];
            data.push_back(p);
        }
    }

    return data;
}

// Fungsi menampilkan hasil
void tampilkanHasil(const Paper& p) {
    cout << "----------------------------------------" << endl;
    cout << "Judul  : " << p.judul << endl;
    cout << "Tahun  : " << p.tahun << endl;
    cout << "Penulis: " << p.penulis << endl;
}

// Linear Search
void linearSearch(const vector<Paper>& data, const string& field, const string& keyword) {
    bool found = false;
    for (size_t i = 0; i < data.size(); ++i) {
        if ((field == "judul" && data[i].judul.find(keyword) != string::npos) ||
            (field == "penulis" && data[i].penulis.find(keyword) != string::npos) ||
            (field == "tahun" && intToString(data[i].tahun).find(keyword) != string::npos)) {
            tampilkanHasil(data[i]);
            found = true;
        }
    }

    if (!found) {
        cout << "Data tidak ditemukan." << endl;
    }
}

// Binary Search
void binarySearch(vector<Paper> data, const string& field, const string& keyword) {
    if (field == "tahun") {
        sort(data.begin(), data.end(), bandingTahun);
    } else if (field == "judul") {
        sort(data.begin(), data.end(), bandingJudul);
    } else if (field == "penulis") {
        sort(data.begin(), data.end(), bandingPenulis);
    }

    int kiri = 0, kanan = data.size() - 1;
    int index = -1;

    // Cari satu data yang cocok
    while (kiri <= kanan) {
        int tengah = (kiri + kanan) / 2;
        string value;

        if (field == "judul") value = data[tengah].judul;
        else if (field == "penulis") value = data[tengah].penulis;
        else value = intToString(data[tengah].tahun);

        if (value.find(keyword) != string::npos) {
            index = tengah;
            break;
        } else if (value < keyword) {
            kiri = tengah + 1;
        } else {
            kanan = tengah - 1;
        }
    }

    if (index == -1) {
        cout << "Data tidak ditemukan." << endl;
        return;
    }

    // Telusuri ke kiri dan kanan dari posisi ditemukan
    int i = index;
    while (i >= 0) {
        string value;
        if (field == "judul") value = data[i].judul;
        else if (field == "penulis") value = data[i].penulis;
        else value = intToString(data[i].tahun);

        if (value.find(keyword) != string::npos) {
            tampilkanHasil(data[i]);
            i--;
        } else break;
    }

    i = index + 1;
    while (i < data.size()) {
        string value;
        if (field == "judul") value = data[i].judul;
        else if (field == "penulis") value = data[i].penulis;
        else value = intToString(data[i].tahun);

        if (value.find(keyword) != string::npos) {
            tampilkanHasil(data[i]);
            i++;
        } else break;
    }
}

// Main
int main() {
    vector<Paper> data = loadCSV("Struktur_Data_Dataset_Kelas_A_B_C_Sheet1.csv");
    int pilihan;

    do {
        cout << "\n========== MENU ==========" << endl;
        cout << "1. Pencarian Linear" << endl;
        cout << "2. Pencarian Binary" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1 || pilihan == 2) {
            string field, keyword;
            cout << "Cari berdasarkan (judul / penulis / tahun): ";
            getline(cin, field);
            cout << "Masukkan kata kunci: ";
            getline(cin, keyword);

            cout << "\n========== HASIL PENCARIAN ==========" << endl;
            if (pilihan == 1) {
                linearSearch(data, field, keyword);
            } else {
                binarySearch(data, field, keyword);
            }
            cout << "=====================================" << endl;
        } else if (pilihan == 3) {
            cout << "Keluar dari program." << endl;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    } while (pilihan != 3);

    return 0;
}
