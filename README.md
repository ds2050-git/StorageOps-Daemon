# StorageOps-Daemon

A C++17 and Python telemetry tool built to monitor storage fleets, improve operational visibility, and anticipate device issues regarding reliability and security.

## Project Details
Developed to support operational workflows by ingesting raw storage hardware metrics and flagging device degradation before failure. It utilizes a multithreaded producer-consumer model in C++ to process high-throughput logs safely, paired with Python automation scripts for data orchestration and reporting.

## Implementation Summary
- **C++17 Telemetry Core (`daemon_core.cpp`):** Uses a thread-safe queue (`std::mutex`, `std::condition_variable`, `<thread>`) to process device records concurrently and evaluate health parameters like temperature and reallocated sectors.
- **Python Automation Wrapper (`daemon_runner.py`):** Automates dataset generation, compiles the system binary via subprocesses, and aggregates diagnostic outputs into structured JSON logs.

## Setup & Execution
Ensure `g++` (with C++17 and pthread support) and Python 3 are installed, then run:

```bash
python3 daemon_runner.py
