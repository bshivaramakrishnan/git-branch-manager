#pragma once
#include "db.hpp"
#include <string>
using namespace std;

class BranchManager {
public:
    explicit BranchManager(DB& db);
    bool addFromGit(const std::string& branchName, const std::string& commitHash, const std::string& issue="", const std::string& desc="");
    bool updateBranchStatus(const std::string& branchName, const std::string& newStatus);
private:
    DB& db_;
};
