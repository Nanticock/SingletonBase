#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h>

#include <vector>
#include <string>
#include <filesystem>

class PathManager {
public:
    static PathManager& instance() {
        static PathManager inst;
        return inst;
    }

    std::filesystem::path localSearchPath() const {
        return std::filesystem::current_path();
    }

    void addPath(const std::string& path) {
        m_paths.push_back(path);
    }

    const std::vector<std::string>& getPaths() const {
        return m_paths;
    }

    std::vector<std::string> getAllSearchPaths() const {
        std::vector<std::string> result;
        result.push_back(localSearchPath().string());
        result.insert(result.end(), m_paths.begin(), m_paths.end());
        return result;
    }

    std::vector<std::filesystem::path> getAllFiles() const {
        std::vector<std::filesystem::path> allFiles;

        for (const auto& dir : getAllSearchPaths()) {
            std::filesystem::path p(dir);

            if (!std::filesystem::exists(p) || !std::filesystem::is_directory(p))
                continue;

            for (const auto& entry : std::filesystem::directory_iterator(p)) {
                if (entry.is_regular_file()) {
                    allFiles.push_back(entry.path());
                }
            }
        }

        return allFiles;
    }

private:
    PathManager() = default;
    std::vector<std::string> m_paths;
};

TEST_CASE("Add one path to singleton") {
    PathManager::instance().addPath("/home/user");

    auto files = PathManager::instance().getAllFiles();
    CHECK(files.size() == 4);  // 2 from localSearchPath + 2 from added path
    CHECK(files[0] == "/usr/local/bin/file1.txt");
    CHECK(files[1] == "/usr/local/bin/file2.txt");
    CHECK(files[2] == "/home/user/file1.txt");
    CHECK(files[3] == "/home/user/file2.txt");
}

TEST_CASE("Add three more paths - shared state demonstration") {
    // This test sees the path added by the previous test!
    auto files = PathManager::instance().getAllFiles();
    REQUIRE(files.size() == 4);  // Still has the files from previous test
    
    // Add three more paths
    PathManager::instance().addPath("/tmp");
    PathManager::instance().addPath("/var/log");
    PathManager::instance().addPath("/etc");

    files = PathManager::instance().getAllFiles();
    REQUIRE(files.size() == 10);  // local + 4 paths = 5 * 2 = 10 files
    REQUIRE(files[8] == "/var/log/file1.txt");
    REQUIRE(files[9] == "/var/log/file2.txt");
}