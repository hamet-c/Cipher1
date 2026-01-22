#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <cctype>
using namespace std;
string decipher(string& text, string key) {
    int i, j;
    cout << key << endl;
    for (i = 0; i < text.size(); i++ ) {
        j = key.at(i % key.size()) - 65;
        text.at(i) = (text.at(i) + 'A' - j) % 26 + 'A';
    }
    return text;
}
int main() {
    // decipher method adapted from Taylor Gibson https://www.youtube.com/watch?v=zmKCaKYkBMU And Dr. C.-K. Shene  https://pages.mtu.edu/~shene/NSF-4/Tutorial/index.html

    int i;
    int j;
    int k;
    int l;

    int coincidenceCounter = 0;
    double IC_TOTAL = 0;

    //string text = "NUIVRNFLXDGNFRRBMAYUENRBJEIOIAMVSIWJBNXZZRLGSJVVAVRVXRUACOLVHTMOGNHQSRLNNRZZVJYXRJAUIJXJSEXRVDXGICIMJBLZMOGNHSSGPBQNRVPLMVWWYGCGLVWAICSRIEISEIXVWVTVXVHTEICNHNPTXVWNPMIYUGMJRFIEXMYGBFMOWCLBZDRPYVWOSNMFMNXHMVRHEXCAKVZNCYEWPRQUEOARUEIVPEYNHTEPKHEDRGYQADXUNUIVRNFLXDGNFRRBMAYJIVZRMNPBIOLNMXTNNGIMRFDHWOEFNUIEEPKHEMHYIBQRINPRWAPBQRVNEAXYIVZRM";
    vector<int> letterFrequency(26);
    vector<char> alphabet;
    vector<long double> indexCoincidence(26);
    vector<double> traditionalFrequency =
    { 0.082, 0.014, 0.028, 0.038, 0.131, 0.029, 0.020, 0.053, 0.064, 0.001,
    0.004, 0.034, 0.025, 0.071, 0.080, 0.020, 0.001, 0.068, 0.061, 0.105,
    0.025, 0.009, 0.015, 0.002, 0.020, 0.001 };
    vector<double>shiftedFrequency(26, 0);
    double CHI_SQUARED = 0;
    vector<int> shiftStorage;


        for (i = 0; i < 26; i++) {
            alphabet.push_back('A' + i);
        }
        cout << endl << endl;
    string text, line;
    cout << "Enter the text you want to decrypt (Press Enter twice to end input):" << endl << endl;

    while (getline(cin, line)) {
        if (line.empty()) break;
        text += line;
    }
        for (i = 0; i < text.size(); i++) {
            if (!isalpha(text.at(i))) {
                text.replace(i, 1, "");
                i = i - 1;
            }
        }
    while (true) {
        shiftStorage.resize(0);
        int shift;
        int length;
        double lowest;
        char lowestLetter;
        double average = 0;
        cout << "What length key would you like to test" << endl;
        cin >> length;
        vector<string> coset(length);

        for (i = 0; i < text.size(); i++) {
            coset.at(i % length) = coset.at(i % length) + text.at(i);                             // cosets are just sets of the length, example  coset of length 3 means were taking 3 groups with every multiple of 3,2, and 1
        }
        for (i = 0; i < length; i++) {
            cout << "Coset " << i + 1 << ": " << coset.at(i) << endl << endl;
        }


        for (j = 0; j < length; j++) {
            indexCoincidence.resize(0);
            indexCoincidence.resize(26);
            IC_TOTAL = 0;
            letterFrequency.resize(0);
            letterFrequency.resize(26);
            for (l = 0; l < coset.at(j).size(); l++) {
                for (k = 0; k < alphabet.size(); k++) {
                    if (toupper(coset.at(j).at(l)) == alphabet.at(k)) {          // Grabs letter frequencies for each coset before calculating the index
                        letterFrequency.at(k)++;
                    }
                }
            }
            for (i = 0; i < alphabet.size(); i++) {
                double N = static_cast<double>(coset.at(j).size());
                indexCoincidence.at(i) = (letterFrequency.at(i) * (letterFrequency.at(i) - 1)) / (N * (N - 1));        // IC is the letter frequency at each letter divided by the size
                IC_TOTAL += indexCoincidence.at(i);
            }
            average = IC_TOTAL + average;
        }
        average = average / length;
        cout << "Index Of Coincidence is : " << average << endl << endl;
        if (average > 0.065) {
            cout << "Your key length is likely " << length << " or a multiple of it." << endl;
        }
        else if (average < 0.062) {
            cout << length << " is most likely not your key length." << endl;
            continue;
        }
        // This giant multi nested loop goes through each coset and shifts it over and over by the alphabet integer values and calculates the chi squared between the frequency of the shift for that coset and the traditional english frequencies and saves the lowest chi square and prints out the letter for that point
        for (j = 0; j < length; j++) {
            letterFrequency.resize(0);
            letterFrequency.resize(26);
            lowest = 1000;
            lowestLetter = 'a';
            for (k = 0; k < coset.at(j).size(); k++) {
                for (l = 0; l < alphabet.size(); l++) {
                    if (toupper(coset.at(j).at(k)) == alphabet.at(l)) {
                        letterFrequency.at(l)++;
                    }
                }
            }
            cout << "Coset " << j + 1 << ":" << endl;
            for (shift = 0; shift < 26; shift++) {																															//This loop with the shift written with chatgpt help 96 - 98
                shiftedFrequency.resize(0);
                shiftedFrequency.resize(26);
                for (i = 0; i < 26; i++) {
                    shiftedFrequency.at(i) = letterFrequency.at(fabs((i + shift) % 26));                                 // week 7 code for shift;
                }

                for (i = 0; i < 26; i++) {
                    double expected = traditionalFrequency.at(i);																										 // This line written from gpt assistance
                    double squared = (shiftedFrequency.at(i)) / coset.at(j).size() - expected;
                    CHI_SQUARED += (squared * squared) / expected;

                }
                if (CHI_SQUARED < lowest) {
                    lowest = CHI_SQUARED;
                    lowestLetter = alphabet.at(shift);
                }
                //	cout <<"Shift " << alphabet.at(shift) << " " << CHI_SQUARED << endl;            Removing the comment on this line you will get the chi squared value for each shift instead of just the lowest one
                CHI_SQUARED = 0;
            }
            cout << "best shift for coset " << j + 1 << " " << lowestLetter << " : " << lowest << endl;
            shiftStorage.push_back(lowestLetter);
        }
        cout << "Word is probably: ";
        string word;
        for (i = 0; i < shiftStorage.size(); i++) {
            cout << (char)shiftStorage.at(i);
            word += (char)shiftStorage.at(i);
        }
        cout << endl << endl;
        cout << "The text deciphed is" << endl;
        cout << decipher(text, word) << endl << endl;
        return 0;
    }
    };
