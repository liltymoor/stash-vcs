#include "state_diff.hpp"
#include <iostream>
std::string LineDiff::toString() const {
    switch (type) {
        case ADDED:
            return "+ | " + FGRN(content);
        case REMOVED:
            return "- | " + FRED(content);
        default:
            return "* | " + FYEL(content);
    }
}

void FileDiff::print(const int limit) const {
    if (changes.size() == 0) {
        std::cout << "No changes" << std::endl;
        return;
    }

    std::cout << "Changes:" << std::endl;
    for (const auto& change : changes) {
        if (limit && change.lineNumber > limit) {
            std::cout << "... (truncated)" << std::endl;
            break;
        }
        std::cout << change.lineNumber << ": " << change.toString() << std::endl;
    }
}