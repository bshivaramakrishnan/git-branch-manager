cmake_minimum_required(VERSION 3.10)
project(git-branch-manager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Try find_package first, fallback to find_library
find_package(SQLite3 QUIET)
if (SQLite3_FOUND)
  message(STATUS "Found SQLite3 via find_package")
  set(SQLITE_LIB ${SQLite3_LIBRARIES})
else()
  find_library(SQLITE3_LIB sqlite3)
  if (NOT SQLITE3_LIB)
    message(FATAL_ERROR "sqlite3 library not found. Install via Homebrew: brew install sqlite")
  endif()
  set(SQLITE_LIB ${SQLITE3_LIB})
endif()

add_executable(gbm
  src/main.cpp
  src/db.cpp
  src/branch_manager.cpp
)

target_link_libraries(gbm PRIVATE ${SQLITE_LIB})
