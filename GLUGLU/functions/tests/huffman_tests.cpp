#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdio>

#include "../huffman_compression.h"

bool file_exists(const std::string& path) {
    std::ifstream f(path.c_str());
    return f.good();
}

bool files_are_equal(const std::string& p1, const std::string& p2) {
    std::ifstream f1(p1, std::ios::binary);
    std::ifstream f2(p2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open()) return false;

    std::istreambuf_iterator<char> it1(f1), end1;
    std::istreambuf_iterator<char> it2(f2), end2;

    return std::equal(it1, end1, it2, end2);
}

void create_test_input(const std::string& path, const std::string& content) {
    std::ofstream f(path, std::ios::binary);
    f << content;
    f.close();
}

void run_huffman_test(const std::string& testName, const std::string& content) {
    std::cout << "Testing [" << testName << "] ... ";

    // Sanitize test name for filenames
    std::string safeName = testName;
    std::replace(safeName.begin(), safeName.end(), ' ', '_');

    std::string inputPath = safeName + "_input.txt";
    std::string compressedPath = safeName + "_compressed.txt";
    std::string outputPath = safeName + "_output.txt";

    create_test_input(inputPath, content);

    try {
        huffman_compression(inputPath, compressedPath);

        if (!content.empty() && !file_exists(compressedPath)) {
            std::cout << "FAILED (Compressed file not created)" << std::endl;
            return;
        }

        huffman_decompression(compressedPath, outputPath);

        if (content.empty() || files_are_equal(inputPath, outputPath)) {
            std::cout << "PASSED" << std::endl;
        } else {
            std::cout << "FAILED (Integrity check failed)" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "FAILED (Exception: " << e.what() << ")" << std::endl;
    }
}

int main() {
    std::cout << "--- HUFFMAN ALGORITHM UNIT TESTS ---\n" << std::endl;

    run_huffman_test("Standard String", "The quick brown fox jumps over the lazy dog.");
    run_huffman_test("Repeating Single Char", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    run_huffman_test("Small Alternating", "ababababababab");

    std::string largeData;
    for(int i = 0; i < 5000; ++i) largeData += "HuffmanTestingData123!";
    run_huffman_test("Large Dataset", largeData);

    std::string allBytes;
    for(int i = 0; i < 256; ++i) allBytes += static_cast<char>(i);
    run_huffman_test("Full 8-bit Range (Binary)", allBytes);

    std::cout << "\nTests completed." << std::endl;
    return 0;
}
