#define CATCH_CONFIG_MAIN
#include "https://raw.githubusercontent.com/catchorg/Catch2/v3.4.0/single_include/catch2/catch_amalgamated.hpp"
#include <vector>
#include <string>

// Raw Singleton with vector of paths
class PathManager {
public:
    static PathManager& instance() {
        static PathManager inst;
        return inst;
    }

    void addPath(const std::string& path) {
        m_paths.push_back(path);
    }

    const std::vector<std::string>& getPaths() const {
        return m_paths;
    }

private:
    PathManager() = default;

private:
    std::vector<std::string> m_paths;
};

TEST_CASE("Add paths to singleton") {
    // Add some paths to the global singleton
    PathManager::instance().addPath("/home/user");
    PathManager::instance().addPath("/tmp");
    
    auto paths = PathManager::instance().getPaths();
    REQUIRE(paths.size() == 2);
    REQUIRE(paths[0] == "/home/user");
    REQUIRE(paths[1] == "/tmp");
}

TEST_CASE("Singleton shared state - paths persist between tests") {
    // This test sees the paths added by the previous test!
    auto paths = PathManager::instance().getPaths();
    REQUIRE(paths.size() == 2);  // Still has the paths from previous test
    REQUIRE(paths[0] == "/home/user");
    REQUIRE(paths[1] == "/tmp");
    
    // Add more paths
    PathManager::instance().addPath("/var/log");
}

TEST_CASE("More shared state demonstration") {
    // Now has 3 paths: 2 from first test + 1 from second test
    auto paths = PathManager::instance().getPaths();
    REQUIRE(paths.size() == 3);
    REQUIRE(paths[2] == "/var/log");
}