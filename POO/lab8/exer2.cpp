#include <fstream>
#include <stdexcept>
#include <sstream>
#include "Song.h" // Asumiendo que la clase Song está en otro archivo .h/.cpp

class SongCollection {
private:
    vector<Song> songs;

public:
    SongCollection(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw invalid_argument("No se pudo abrir el archivo: " + filename);
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string artist, title, lyrics;

            if (getline(ss, artist, '|') && getline(ss, title, '|') && getline(ss, lyrics)) {
                songs.emplace_back(artist, title, lyrics);
            }
        }

        file.close();
    }

    const vector<Song>& getSongs() const {
        return songs;
    }
};

int main() {
    try {
        SongCollection mySongs("songs.txt"); // Asegúrate de tener este archivo
        cout << "Canciones cargadas exitosamente." << endl;
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}