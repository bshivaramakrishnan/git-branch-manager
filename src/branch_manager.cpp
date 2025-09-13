#include "branch_manager.hpp"
#include <ctime>
#include <filesystem>
using namespace std;

BranchManager::BranchManager(DB& db) : db_(db) {}

bool BranchManager::addFromGit(const std::string& branchName, const std::string& commitHash, const std::string& issue, const std::string& desc) {
    BranchInfo b;
    b.repo_root = std::filesystem::current_path().string();
    b.branch_name = branchName;
    b.commit_hash = commitHash;
    std::time_t t = std::time(nullptr);
    b.created_at = b.updated_at = static_cast<std::int64_t>(t);
    b.issue_id = issue;
    b.fix_description = desc;
    b.merge_status = "open";
    return db_.addBranch(b);
}
bool BranchManager::updateBranchStatus(const std::string& branchName, const std::string& newStatus) {
    return db_.updateBranchStatus(branchName, newStatus);
}


