#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

class Song {
    private:
    string artist;
    string title;
    vector<string> lyricsWords;

    //Función auxiliar para limpiar y dividir la letra
    void processLyrics(const string& lyrics){
        string word;
        for (char ch: lyrics){
            if(isalpha(ch) || ch == ' '){
                word += tolower(ch);
            } else if (ispunct(ch) || isdigit(ch)) {
                continue; //ignora puntuacion y numeros
            } else {
                word += ' ';
            }
        }


        istringstream iss(word);
        string token;
        while (iss >> token){
            lyricsWords.push_back(token);
        }
    }

    public:
    Song(string artist, string title, string lyrics)
    : artist(artist), title(title){
        processLyrics(lyrics);
    }

    string getArtist() const{
        return artist;
    }

    string getTitle() const{
        return title;
    }

    vector<string> getLyricsWords() const{
        return lyricsWords;
    }

};

int main() {
    Song s("Coldplay", "Yellow", "Look at the stars, look how they shine for you.");
    cout << "Artist: " << s.getArtist() << endl;
    cout << "Title: " << s.getTitle() << endl;
    cout << "Lyrics words: ";
    for (const string& word : s.getLyricsWords()) {
        cout << word << " ";
    }
    cout << endl;
    return 0;
}
