#pragma once
#include <map>
#include <unordered_set>
#include <vector>
#include <string>

#include "logger.hpp"

#ifndef _COLORS_
#define _COLORS_

/* FOREGROUND */
#define RST   "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) (KRED + (x + RST))
#define FGRN(x) (KGRN + (x + RST))
#define FYEL(x) (KYEL + (x + RST))
#define FBLU(x) (KBLU + (x + RST))
#define FMAG(x) (KMAG + (x + RST))
#define FCYN(x) (KCYN + (x + RST))
#define FWHT(x) (KWHT + (x + RST))

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

#endif  /* _COLORS_ */

struct LineDiff {
    enum ChangeType { ADDED, REMOVED, MODIFIED };
    ChangeType type;
    std::string content;
    int lineNumber;

    bool operator== (const LineDiff& diff) const;
    std::string toString() const;
};

struct FileDiff {
    std::vector<LineDiff> changes;
    bool hasConflicts = false;
    bool wasDeleted = false;

    void print(const int limit = 0) const;
    
    static std::vector<std::pair<LineDiff, LineDiff>> intersection(const FileDiff& fileChanges_a, const FileDiff& fileChanges_b);
};

using DiffResult = std::map<std::string, FileDiff>;