#include "db.hpp"
#include "branch_manager.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
    std::filesystem::create_directories("data");
    DB db("data/gitbranch.db");
    if (!db.init()) return 1;
    BranchManager bm(db);

    if (argc == 1) {
        // list branches
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
    std::cerr << "unknown command\n";
    return 1;
}
