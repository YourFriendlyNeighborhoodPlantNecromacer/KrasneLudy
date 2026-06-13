#ifndef HUFFMAN_COMPRESSION_H
#define HUFFMAN_COMPRESSION_H

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <array>

struct node {
    long long frequency;
    unsigned char character;
    node* left;
    node* right;

    node(): frequency(0), character(0), left(nullptr), right(nullptr) {};
    node(long long freq, unsigned char cha, node* l = nullptr, node* r = nullptr)
        : frequency(freq), character(cha), left(l), right(r) {};

    // Destruktor jest określony iteracyjnie poniżej
    ~node() {}
};

// Iteracyjnie usuwa węzły, aby uniknąć np. podwójnego zwolnienia w rekursji
void deleteTree(node* root) {
    if (!root) return;
    std::vector<node*> nodes = { root };
    while (!nodes.empty()) {
        node* curr = nodes.back();
        nodes.pop_back();
        if (curr->left) nodes.push_back(curr->left);
        if (curr->right) nodes.push_back(curr->right);
        delete curr;
    }
}

struct Compare {
    bool operator()(node* l, node* r) {
        if (l->frequency == r->frequency) {
            // Deterministyczne porównanie
            return l->character > r->character;
        }
        return l->frequency > r->frequency;
    }
};

void generateCodes(node* root, std::string code, std::array<std::string, 256>& huffmanCodes) {
    if (!root) return;

    if (!root->left && !root->right) {
        // Przypadek dla plików tylko z jednym charakterem
        huffmanCodes[root->character] = code.empty() ? "0" : code;
        return;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

void huffman_compression(const std::string& inputPath, const std::string& outputPath) {
    std::cout << "oh so thight kawaii" << std::endl;
    std::ifstream tekst(inputPath, std::ios::binary);
    if (!tekst.is_open()) {
        std::cerr << "Error: Could not open input file " << inputPath << std::endl;
        return;
    }

    std::array<long long, 256> ilosci;
    ilosci.fill(0);

    // Obliczenie częstotliwości dzięki tablicy
    unsigned char c;
    while (tekst.read(reinterpret_cast<char*>(&c), 1)) {
        ilosci[c]++;
    }

    // Kolejka priorytetów do budowania drzewa
    std::priority_queue<node*, std::vector<node*>, Compare> kolejka;

    for (int i = 0; i < 256; i++) {
        if (ilosci[i] > 0) {
            kolejka.push(new node(ilosci[i], static_cast<unsigned char>(i)));
        }
    }

    if (kolejka.empty()) return;

    // Specjalny przypadek gdy kod jest jednym znakiem, np. AAAA
    if (kolejka.size() == 1) {
        node* single = kolejka.top();
        kolejka.pop();
        kolejka.push(new node(single->frequency, single->character, single, nullptr));
    }

    while (kolejka.size() > 1) {
        node* left = kolejka.top();
        kolejka.pop();
        node* right = kolejka.top();
        kolejka.pop();

        // Użyc najmniejszego char'a dzieciaka by mieć deterministyczną konwencję nazwania węzłów
        unsigned char internalChar = std::min(left->character, right->character);
        kolejka.push(new node(left->frequency + right->frequency, internalChar, left, right));
    }

    node* huffmanRoot = kolejka.top();
    long long totalChars = huffmanRoot->frequency;

    std::array<std::string, 256> huffmanCodes;
    generateCodes(huffmanRoot, "", huffmanCodes);

    tekst.clear();
    tekst.seekg(0, std::ios::beg);

    std::ofstream binarny(outputPath, std::ios::binary);
    if (!binarny.is_open()) {
        std::cerr << "Error: Could not open output file " << outputPath << std::endl;
        deleteTree(huffmanRoot);
        return;
    }

    // Napisz tablicę częstotliwości do nagłówka pliku aby można potem zrekonstruować
    for (int i = 0; i < 256; i++) {
        binarny.write(reinterpret_cast<const char*>(&ilosci[i]), sizeof(long long));
    }
    // Wypisać ilość
    binarny.write(reinterpret_cast<const char*>(&totalChars), sizeof(long long));

    unsigned char buffer = 0;
    int bitCount = 0;

    // Streamuj z pliku na bity by unikać wysokiego użycia pamięci
    while (tekst.read(reinterpret_cast<char*>(&c), 1)) {
        const std::string& code = huffmanCodes[c];
        for (char bit : code) {
            buffer <<= 1;
            if (bit == '1') buffer |= 1;
            bitCount++;

            if (bitCount == 8) {
                binarny.put(static_cast<char>(buffer));
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        binarny.put(static_cast<char>(buffer));
    }

    binarny.close();
    tekst.close();

    deleteTree(huffmanRoot);
}

void huffman_decompression(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream binarny(inputPath, std::ios::binary);
    if (!binarny.is_open()) {
        std::cerr << "Error: Could not open input file " << inputPath << std::endl;
        return;
    }

    // Czytanie
    std::array<long long, 256> ilosci;
    for (int i = 0; i < 256; i++) {
        binarny.read(reinterpret_cast<char*>(&ilosci[i]), sizeof(long long));
    }

    long long totalChars;
    binarny.read(reinterpret_cast<char*>(&totalChars), sizeof(long long));

    if (totalChars <= 0) {
        binarny.close();
        return;
    }

    // Odbudowanie
    std::priority_queue<node*, std::vector<node*>, Compare> kolejka;
    for (int i = 0; i < 256; i++) {
        if (ilosci[i] > 0) {
            kolejka.push(new node(ilosci[i], static_cast<unsigned char>(i)));
        }
    }

    while (kolejka.size() > 1) {
        node* left = kolejka.top();
        kolejka.pop();
        node* right = kolejka.top();
        kolejka.pop();
        unsigned char internalChar = std::min(left->character, right->character);
        kolejka.push(new node(left->frequency + right->frequency, internalChar, left, right));
    }

    node* root = kolejka.top();
    node* current = root;

    std::ofstream wyjscie(outputPath, std::ios::binary);
    if (!wyjscie.is_open()) {
        std::cerr << "Error: Could not open output file " << outputPath << std::endl;
        deleteTree(root);
        return;
    }

    unsigned char byte;
    long long charsDecoded = 0;

    // Czytanie bitów by pokryło się z logiką kompresji
    while (charsDecoded < totalChars && binarny.read(reinterpret_cast<char*>(&byte), 1)) {
        for (int i = 7; i >= 0; --i) {
            bool bit = (byte >> i) & 1;

            if (current->left || current->right) {
                if (bit) current = current->right;
                else current = current->left;
            }

            // Kiedy napotkamy liść
            if (!current->left && !current->right) {
                wyjscie.put(static_cast<char>(current->character));
                charsDecoded++;
                current = root;

                if (charsDecoded >= totalChars) break;
            }
        }
    }

    wyjscie.close();
    binarny.close();
    deleteTree(root);
}

#endif // HUFFMAN_COMPRESSION_H
