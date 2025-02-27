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
        INFO("No changes");
        return;
    }

    if (wasDeleted) {
        INFO(FRED(std::string("File deleted")));
        return;
    }

    for (const auto& change : changes) {
        if (limit && change.lineNumber > limit) {
            INFO("... (truncated)");
            break;
        }
        INFO(change.lineNumber << ": " << change.toString());
    }
}