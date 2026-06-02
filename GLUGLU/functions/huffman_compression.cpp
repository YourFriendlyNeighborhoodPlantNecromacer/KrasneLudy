#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <string>

struct node {
    int frequency;
    char character;
    node* left;
    node* right;

    node(): frequency(0), character(' '), left(nullptr), right(nullptr) {};
    node(int freq, char cha) : frequency(freq), character(cha), left(nullptr), right(nullptr) {};
    ~node() {
        delete left;
        delete right;
    }
};

struct Compare {
    bool operator()(node* l, node* r) {
        return l->frequency > r->frequency;
    }
};

void generateCodes(node* root, std::string code, std::map<char, std::string>& huffmanCodes) {
    if (!root) return;

    // je¿eli to liœæ, to ma jakiœ charakter
    if (!root->left && !root->right) {
        huffmanCodes[root->character] = code;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

std::string decode(node* root, std::string encoded) {
    std::string decodedString = "";
    node* current = root;

    if (!root) return "";

    for (char bit : encoded) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (!current->left && !current->right) {
            decodedString += current->character;
            current = root;
        }
    }
    return decodedString;
}

void huffman_compression() {
    std::ifstream tekst("text.txt"); //Antony pomyśl co z tym zrobić.

    char c;
    std::map<char, int> ilosci;

    // wziêcie czêstotliwoœci ka¿dego charakteru
    while (tekst.get(c)) {
        ilosci[c]++;
    }

    // tworzenie kojelki, automatycznie porównaj¹ca po czêstotliwoœci
    std::priority_queue<node*, std::vector<node*>, Compare> kolejka;

    for (auto const& [character, count] : ilosci) {
        kolejka.push(new node(count, character));
    }

    while (kolejka.size() > 1) {
        // usuwa 2 najmniej wystêpuj¹ce elementy
        node* left = kolejka.top();
        kolejka.pop();
        node* right = kolejka.top();
        kolejka.pop();

        // i je dodaje jako dzieci
        node* top = new node(left->frequency + right->frequency, '\0');
        top->left = left;
        top->right = right;
        kolejka.push(top);
    }

    // Drzewo wystêpuj¹ce z tego
    node* huffmanRoot = kolejka.empty() ? nullptr : kolejka.top();

    // Generacja Kodów do ka¿dego charakteru
    std::map<char, std::string> huffmanCodes;
    generateCodes(huffmanRoot, "", huffmanCodes);

    tekst.clear();
    tekst.seekg(0, std::ios::beg);

    // Zakodowanie
    std::string zakodowane;
    while (tekst.get(c)) {
        zakodowane += huffmanCodes[c];
    }
    tekst.close();

    std::cout << zakodowane << std::endl;

    std::ofstream binarny("text_endoded.bin", std::ios::binary); //to samo z tym
    unsigned char buffer = 0;
    int bitCount = 0;

    for (char bit : zakodowane) {
        buffer <<= 1;

        if (bit == '1') {
            buffer |= 1;
        }

        bitCount++;

        if (bitCount == 8) {
            binarny.put(static_cast<char>(buffer));
            buffer = 0;
            bitCount = 0;
        }
    }

    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        binarny.put(static_cast<char>(buffer));
    }

    binarny.close();

    // Dekodowanie
    std::string dekodowane = decode(huffmanRoot, zakodowane);
    std::cout << dekodowane << std::endl;

    delete huffmanRoot;
}