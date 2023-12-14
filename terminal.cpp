#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

class FileSystem {
private:
    struct Entry {
        std::string name;
        bool isDirectory;
        std::string content;

        Entry(const std::string& n, bool isDir = false) : name(n), isDirectory(isDir) {}
    };

    std::vector<Entry> currentDirectory;
    std::string currentPath;

public:
    FileSystem() {
        currentPath = "/";
        currentDirectory.emplace_back(".", true);
        currentDirectory.emplace_back("..", true);
    }

    void mkdir(const std::string& dirName) {
        currentDirectory.emplace_back(dirName, true);
    }

    void cd(const std::string& path) {
        if (path == "..") {
            size_t lastSlash = currentPath.find_last_of('/');
            if (lastSlash != std::string::npos) {
                currentPath = currentPath.substr(0, lastSlash);
            }
        } else if (path == "/") {
            currentPath = "/";
        } else {
            currentPath += "/" + path;
        }
    }

    void ls(const std::string& path = "") {
        std::vector<std::string> entries;
        for (const auto& entry : currentDirectory) {
            if (entry.name != "." && entry.name != "..") {
                entries.push_back(entry.name);
            }
        }

        if (!path.empty()) {
            std::cout << "Contents of " << path << ":\n";
        } else {
            std::cout << "Contents of current directory:\n";
        }

        for (const auto& entry : entries) {
            std::cout << entry << " ";
        }
        std::cout << "\n";
    }

    void touch(const std::string& fileName) {
        currentDirectory.emplace_back(fileName);
    }

    void echo(const std::string& content, const std::string& fileName) {
        auto it = std::find_if(currentDirectory.begin(), currentDirectory.end(),
                               [&fileName](const Entry& entry) { return entry.name == fileName; });

        if (it != currentDirectory.end()) {
            it->content = content;
        } else {
            std::cout << "Error: File not found.\n";
        }
    }

    void rm(const std::string& path) {
        auto it = std::find_if(currentDirectory.begin(), currentDirectory.end(),
                               [&path](const Entry& entry) { return entry.name == path; });

        if (it != currentDirectory.end()) {
            currentDirectory.erase(it);
        } else {
            std::cout << "Error: File or directory not found.\n";
        }
    }

    void displayPrompt() {
        std::cout << currentPath << "> ";
    }

    void run() {
        std::string command;
        while (true) {
            displayPrompt();
            std::getline(std::cin, command);

            std::istringstream iss(command);
            std::string operation;
            iss >> operation;

            if (operation == "mkdir") {
                std::string dirName;
                iss >> dirName;
                mkdir(dirName);
            } else if (operation == "cd") {
                std::string path;
                iss >> path;
                cd(path);
            } else if (operation == "ls") {
                ls();
            } else if (operation == "touch") {
                std::string fileName;
                iss >> fileName;
                touch(fileName);
            } else if (operation == "echo") {
                std::string content, fileName;
                iss >> content;
                iss >> fileName;
                echo(content, fileName);
            } else if (operation == "rm") {
                std::string path;
                iss >> path;
                rm(path);
            } else if (operation == "exit") {
                break;
            } else {
                std::cout << "Error: Unknown command.\n";
            }
        }
    }
};

int main() {
    FileSystem fs;
    fs.run();

    return 0;
}
