#pragma once

#include <string>
#include <vector>

struct ExerciseEntry {
    std::string slug;
    std::string title;
    std::string file;
};

struct Chapter {
    std::string id;
    std::string title;
    int order;
    std::vector<ExerciseEntry> exercises;
};
