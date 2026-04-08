# Multi-Node-Distributed-File-Scanning-System-C-MPI-

## Project Overview

This project uses parallel computing system to achieve **multi-node distributed file scanning/searching system**.  

It uses **C++ multithreading** for parallelism and **MPI (Message Passing Interface)** for communication. The system scans and analyzes files **across multiple machines simultaneously**, identifying:

-  **Duplicate files**   
-  **Large files**   
-  **Untouched files**  

The project demonstrates both **shared-memory parallelism (threads)** and **distributed-memory parallelism (nodes)** — key concepts in high-performance and parallel computing.

---

## Project Goals

- Implement a **multi-threaded file scanner** on each node.  
- Use **MPI** to coordinate tasks between multiple nodes.  
- Aggregate and merge results (e.g., duplicate files, large files) on a central coordinator.  
- Demonstrate scalability and performance improvements.  
- Analyze execution time and parallel efficiency across nodes and threads.

---

## Use Case / User Story

**User Story:**  
As a system administrator or researcher managing data across multiple servers,  
I want to quickly identify duplicate, large, or old files across all nodes so that I can optimize storage, improve performance.

**Key Users:**
- System administrators managing multi-node storage clusters  
- Researchers analyzing distributed datasets  
- Students learning distributed and parallel computing concepts  

**Why Use This Project:**  
It provides a practical demonstration of hybrid parallelism — combining multi-threading and distributed message passing — to efficiently process large-scale file systems across multiple machines.

---

## Key Concepts and Technologies

| Concept | Description | Implementation |
|----------|--------------|----------------|
| **Data Parallelism** | Each node scans a portion of the total filesystem | File partitions assigned by coordinator |
| **Task Parallelism** | Nodes perform scanning, hashing, and reporting simultaneously | Independent threads and MPI ranks |
| **Multithreading** | Each node uses threads for local parallelism | `std::thread`, `std::async` |
| **Distributed Processing** | Tasks split across nodes via message passing | **MPI (OpenMPI / MPICH)** |
| **Synchronization** | Results merged and deduplicated safely | MPI reduction and thread-safe containers |
| **Hashing** | Detect duplicates | MD5 or SHA256 file hashes |
| **Performance Metrics** | Execution time, node scalability, efficiency | `<chrono>` and MPI timers |

---

## System Architecture

                    ┌──────────────────────────┐
                    │      Coordinator Node     │
                    │ (task distribution &      │
                    │  result aggregation)      │
                    └────────────┬──────────────┘
                                 │
          ┌──────────────────────┼───────────────────────┐
          │                      │                       │
          ▼                      ▼                       ▼
  ┌──────────────┐        ┌──────────────┐        ┌──────────────┐
  │ Worker Node 1│        │ Worker Node 2│  ...   │ Worker Node N│
  │ (local files │        │ (local files │        │ (local files │
  │  scanning)   │        │  scanning)   │        │  scanning)   │
  └──────────────┘        └──────────────┘        └──────────────┘
                                 │
                                 ▼
                    ┌──────────────────────────┐
                    │ Global Result Aggregator  │
                    │ (duplicates, big files,  │
                    │  untouched files)        │
                    └──────────────────────────┘

# Our project doesn’t rely on any external datasets or paid services. It operates entirely on local file systems using free and standard C++ and MPI libraries. Each node scans its own local directories, so the data is naturally available. All computation, communication, and analysis happen offline, ensuring full control and reproducibility.
