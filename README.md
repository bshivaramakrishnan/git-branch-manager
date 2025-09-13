# Git Branch Manager (GBM)

**Git Branch Manager** is a lightweight command-line tool to track, manage, and update metadata for Git branches locally using SQLite. It allows developers to **add branches, list branches, and update branch status** efficiently without relying on GitHub or Git commands directly for branch tracking.

---

## Features

- Add branch metadata including branch name, commit hash, issue ID, and description.
- List all branches along with their status (`open`, `merged`, etc.).
- Update the `merge_status` of a branch.
- Uses a local SQLite database (`data/gitbranch.db`) to store branch metadata.
- Cross-platform support (tested on macOS).

---

## Installation

1. Clone the repository:

```bash
git clone https://github.com/YOUR_USERNAME/git-branch-manager.git
cd git-branch-manager

---

## Build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -- -j $(sysctl -n hw.physicalcpu)

---

## Functionality

List Branches : ./build/gbm
Add Branch : ./build/gbm add <branch-name> <commit-hash> [issue-id] [description]
Update Branch Status : ./build/gbm update <branch-name> <new-status>

## Project Structure

git-branch-manager/
├── build/                  # CMake build folder
├── data/                   # SQLite database folder
├── include/                # Header files
│   ├── branch_manager.hpp
│   └── db.hpp
├── src/                    # Source files
│   ├── main.cpp
│   ├── branch_manager.cpp
│   └── db.cpp
├── CMakeLists.txt
└── README.md

## Dependencies

C++17 or higher
CMake
SQLite3
