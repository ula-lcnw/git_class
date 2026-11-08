#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "grades.h"

// Flags CLI
static void printHelp(const char* prog) {
    std::cout << "Usage: " << prog << " [OPTIONS] <grades_file>\n\n";
    std::cout << "Reads a file of students and their grades and prints a report.\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help    Show this help message and exit\n";
    std::cout << "  -v, --version Show version information and exit\n";
    std::cout << "\nInput format:\n";
    std::cout << "  One student per line: the name followed by their grades.\n";
}

static void printVersion() {
    std::cout << "grades report 1.0\n";
}

int main(int argc, char* argv[]) {
    std::string gradesFile;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            printVersion();
            return 0;
        } else if (arg.size() > 1 && arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << "\n";
            std::cerr << "Try '" << argv[0] << " --help' for more information.\n";
            return 1;
        } else if (gradesFile.empty()) {
            gradesFile = arg;
        } else {
            std::cerr << "Unexpected argument: " << arg << "\n";
            std::cerr << "Try '" << argv[0] << " --help' for more information.\n";
            return 1;
        }
    }

    if (gradesFile.empty()) {
        std::cerr << "Usage: " << argv[0] << " <grades_file>\n";
        std::cerr << "Try '" << argv[0] << " --help' for more information.\n";
        return 1;
    }

    std::vector<Student> students = readStudents(gradesFile);
    if (students.empty()) {
        std::cerr << "No students found in " << argv[1] << "\n";
        return 1;
    }

    std::sort(students.begin(), students.end(),
              [](const Student& a, const Student& b) {
                  return average(a.grades) > average(b.grades);
              });

    std::cout << "Grades report\n\n";
    std::cout << std::left << std::setw(10) << "Name"
              << std::right << std::setw(9) << "Average"
              << std::setw(7) << "Max"
              << std::setw(7) << "Min"
              << "  Status\n";
    std::cout << std::fixed << std::setprecision(2);
    std::vector<double> averages;
    for (const Student& s : students) {
        averages.push_back(average(s.grades));
        std::cout << std::left << std::setw(10) << s.name
                  << std::right << std::setw(9) << average(s.grades)
                  << std::setw(7) << highest(s.grades)
                  << std::setw(7) << lowest(s.grades)
                  << "  " << (passed(s.grades) ? "PASS" : "FAIL") << "\n";
    }
    std::cout << "\nClass average: " << average(averages) << "\n";
    return 0;
}
