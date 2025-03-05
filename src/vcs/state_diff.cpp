#include "state_diff.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>
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
    uint32_t lineCounter = 0;
    for (const auto& change : changes) {
        if (limit && lineCounter > limit) {
            INFO("... (truncated)");
            break;
        }
        INFO(change.lineNumber << ": " << change.toString());
        lineCounter++;
    }
}

FileConflict FileDiff::intersection(const FileDiff &fileChanges_a, const FileDiff &fileChanges_b) {
    std::vector<LineConflict> conflicts;

    for (const LineDiff& lineChanges: fileChanges_a.changes) {
        auto it = std::find(fileChanges_b.changes.begin(), fileChanges_b.changes.end(), lineChanges);
        if (it != fileChanges_b.changes.end()) {
            conflicts.push_back({fileChanges_a.filename, lineChanges, *it});
        }
    }

    return {conflicts, fileChanges_a.branchName, fileChanges_b.branchName};
}


bool LineDiff::operator==(const LineDiff& diff) const {
    return this->lineNumber == diff.lineNumber && this->content != diff.content;
}