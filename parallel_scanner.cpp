#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <omp.h>
#include <chrono>
#include <fstream>

namespace fs = std::filesystem;

// Helper: merge local unordered_map into global ordered map (for stable output)
static void merge_into_global(
    std::map<uintmax_t, std::vector<std::string>>& global,
    const std::unordered_map<uintmax_t, std::vector<std::string>>& local
) {
    for (const auto& [size, paths] : local) {
        auto& dst = global[size];
        dst.insert(dst.end(), paths.begin(), paths.end());
    }
}

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

    std::cout << "--- Parallel File Scanner (OpenMP, optimized) ---\n";
    std::cout << "OpenMP threads: " << omp_get_max_threads() << "\n";

    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel
    {
        // Thread-local collector (no locks during scanning)
        std::unordered_map<uintmax_t, std::vector<std::string>> local_map;
        local_map.reserve(1024);

#pragma omp for schedule(dynamic)
        for (int i = 0; i < (int)target_dirs.size(); i++) {
            const std::string& dir = target_dirs[i];
            if (!fs::exists(dir)) continue;

            int tid = omp_get_thread_num();
            printf("Thread %d scanning: %s\n", tid, dir.c_str());

            for (const auto& entry : fs::recursive_directory_iterator(dir)) {
                if (!fs::is_regular_file(entry)) continue;

                uintmax_t fsize = entry.file_size();
                std::string fpath = entry.path().string();
                local_map[fsize].push_back(std::move(fpath));
            }
        }

        // Merge once per thread (small number of critical sections)
#pragma omp critical
        {
            merge_into_global(size_map, local_map);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Optional CSV output for later Spark/distributed phase
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

    std::cout << "\nTotal Parallel Time: " << elapsed.count() << " seconds\n";
    return 0;
}