#include "db.hpp"
#include <sqlite3.h>
#include <iostream>
#include <cstring>
#include <vector>

struct sqlite3_tag; // forward
struct sqlite3_tag* to_sqlite(void* p) { return reinterpret_cast<sqlite3_tag*>(p); }

DB::DB(const std::string& path) : dbHandle_(nullptr) {
    sqlite3* db = nullptr;
    if (sqlite3_open(path.c_str(), &db) == SQLITE_OK) {
        dbHandle_ = db;
    } else {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        dbHandle_ = nullptr;
    }
}

DB::~DB() {
    if (dbHandle_) sqlite3_close(reinterpret_cast<sqlite3*>(dbHandle_));
}

bool DB::init() {
    if (!dbHandle_) return false;
    const char* sql = R"sql(
CREATE TABLE IF NOT EXISTS branches (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  repo_root TEXT NOT NULL,
  branch_name TEXT NOT NULL,
  commit_hash TEXT NOT NULL,
  created_at INTEGER NOT NULL,
  updated_at INTEGER NOT NULL,
  issue_id TEXT,
  fix_description TEXT,
  merge_status TEXT,
  UNIQUE(repo_root, branch_name)
);
CREATE INDEX IF NOT EXISTS idx_repo_branch ON branches(repo_root, branch_name);
CREATE INDEX IF NOT EXISTS idx_commit_hash ON branches(commit_hash);
)sql";
    char* err = nullptr;
    int rc = sqlite3_exec(reinterpret_cast<sqlite3*>(dbHandle_), sql, nullptr, nullptr, &err);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << (err ? err : "unknown") << "\n";
        if (err) sqlite3_free(err);
        return false;
    }
    return true;
}

bool DB::addBranch(const BranchInfo& b) {
    if (!dbHandle_) return false;
    const char* sql = "INSERT OR REPLACE INTO branches (repo_root, branch_name, commit_hash, created_at, updated_at, issue_id, fix_description, merge_status) VALUES (?,?,?,?,?,?,?,?);";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(reinterpret_cast<sqlite3*>(dbHandle_), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "prepare failed: " << sqlite3_errmsg(reinterpret_cast<sqlite3*>(dbHandle_)) << "\n";
        return false;
    }
    sqlite3_bind_text(stmt, 1, b.repo_root.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, b.branch_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, b.commit_hash.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 4, b.created_at);
    sqlite3_bind_int64(stmt, 5, b.updated_at);
    sqlite3_bind_text(stmt, 6, b.issue_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, b.fix_description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, b.merge_status.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(reinterpret_cast<sqlite3*>(dbHandle_)) << "\n";
        return false;
    }
    return true;
}

std::vector<BranchInfo> DB::listBranches() const {
    std::vector<BranchInfo> out;
    if (!dbHandle_) return out;
    const char* sql = "SELECT id, repo_root, branch_name, commit_hash, created_at, updated_at, issue_id, fix_description, merge_status FROM branches ORDER BY created_at DESC;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(reinterpret_cast<sqlite3*>(dbHandle_), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return out;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        BranchInfo b;
        b.id = sqlite3_column_int(stmt, 0);
        b.repo_root = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        b.branch_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        b.commit_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        b.created_at = sqlite3_column_int64(stmt, 4);
        b.updated_at = sqlite3_column_int64(stmt, 5);
        const unsigned char* t;
        t = sqlite3_column_text(stmt, 6); b.issue_id = t ? reinterpret_cast<const char*>(t) : "";
        t = sqlite3_column_text(stmt, 7); b.fix_description = t ? reinterpret_cast<const char*>(t) : "";
        t = sqlite3_column_text(stmt, 8); b.merge_status = t ? reinterpret_cast<const char*>(t) : "";
        out.push_back(b);
    }
    sqlite3_finalize(stmt);
    return out;
}
