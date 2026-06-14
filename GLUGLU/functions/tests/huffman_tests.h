#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdio>

#include "../huffman_compression.h"

bool file_exists(const std::string& path);

bool files_are_equal(const std::string& p1, const std::string& p2);

void create_test_input(const std::string& path, const std::string& content);

void run_huffman_test(const std::string& testName, const std::string& content);

int encoder(const std::string file_name);
