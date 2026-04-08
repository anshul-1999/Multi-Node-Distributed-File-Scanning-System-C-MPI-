#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <map>
#include <chrono>
#include <fstream>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    std::vector<std::string> target_dirs = {"./data1", "./data2", "./data3", "./data4"};
    std::map<uintmax_t, std::vector<std::string>> size_map;

    // Optional: write CSV if user passes: --csv out.csv
    bool write_csv = false;
    std::string csv_path;
    if (argc == 3 && std::string(argv[1]) == "--csv") {
        write_csv = true;
        csv_path = argv[2];
    }

    std::cout << "--- Sequential File Scanner (baseline) ---\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& dir : target_dirs) {
        if (!fs::exists(dir)) continue;

        for (const auto& entry : fs::recursive_directory_iterator(dir)) {
            if (!fs::is_regular_file(entry)) continue;

            uintmax_t size = entry.file_size();
            size_map[size].push_back(entry.path().string());
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    if (write_csv) {
        std::ofstream out(csv_path);
        if (!out) {
            std::cerr << "[!] Could not write CSV to: " << csv_path << "\n";
        } else {
            out << "size,path\n";
            for (const auto& [size, paths] : size_map) {
                for (const auto& p : paths) out << size << "," << p << "\n";
            }
            std::cout << "[+] Wrote CSV metadata to: " << csv_path << "\n";
        }
    }

    std::cout << "\n--- Duplicate Report (size-based candidates) ---\n";
    for (const auto& [size, paths] : size_map) {
        if (size > 0 && paths.size() > 1) {
            std::cout << "Size: " << size << " bytes\n";
            for (const auto& p : paths) std::cout << "  -> " << p << "\n";
        }
    }

    std::cout << "\nTotal Sequential Time: " << elapsed.count() << " seconds\n";
    return 0;
}