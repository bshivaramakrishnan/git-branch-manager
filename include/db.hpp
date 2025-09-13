#pragma once
#include<string>
#include<iostream>
#include<vector>
#include<cstdint>

using namespace std;
struct BranchInfo
{
    int id = 0;
    string repo_root;
    string branch_name;
    string commit_hash;
    int64_t created_at =0;
    int64_t updated_at =0;
    string issue_id;
    string fix_description;
    string merge_status;
};

class DB{
    private:
    void* dbHandle_;
    public:
    bool updateBranchStatus(const std::string& branchName, const std::string& newStatus);
    explicit DB(const string& path);
    ~DB();
    bool init();
    bool addBranch(const BranchInfo& b);
    vector<BranchInfo> listBranches() const;
    
};