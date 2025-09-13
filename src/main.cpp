#include "db.hpp"
#include "branch_manager.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
    // Create data directory and initialize DB
    std::filesystem::create_directories("data");
    DB db("data/gitbranch.db");
    if (!db.init()) return 1;

    BranchManager bm(db);

    // No arguments → list branches
    if (argc == 1) {
        auto branches = db.listBranches();
        std::cout << "ID | Branch | Commit | Issue | Status | Created\n";
        for (auto &b : branches) {
            std::cout << b.id << " | " << b.branch_name << " | " << b.commit_hash
                      << " | " << b.issue_id << " | " << b.merge_status
                      << " | " << b.created_at << "\n";
        }
        return 0;
    }

    std::string cmd = argv[1];

    // Add a branch
    if (cmd == "add") {
        if (argc < 4) {
            std::cerr << "usage: gbm add <branch-name> <commit-hash> [issue-id] [description]\n";
            return 1;
        }
        std::string branch = argv[2];
        std::string commit = argv[3];
        std::string issue = (argc >= 5) ? argv[4] : "";
        std::string desc = (argc >= 6) ? argv[5] : "";
        if (bm.addFromGit(branch, commit, issue, desc)) {
            std::cout << "Added branch metadata\n";
            return 0;
        } else {
            std::cerr << "Failed to add\n";
            return 1;
        }
    }

    // Update branch status
    if (cmd == "update") {
        if (argc < 4) {
            std::cerr << "usage: gbm update <branch-name> <new-status>\n";
            return 1;
        }
        std::string branch = argv[2];
        std::string status = argv[3];
        if (bm.updateBranchStatus(branch, status)) {
            std::cout << "Branch status updated\n";
            return 0;
        } else {
            std::cerr << "Failed to update branch status\n";
            return 1;
        }
    }

    std::cerr << "unknown command\n";
    return 1;
}
