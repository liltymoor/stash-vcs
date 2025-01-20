#include <iostream>
#include <cstring>

struct RepoSettings {
    std::string str_repoName;
    std::string str_startBranchName;
};


class Repo {
public:
    explicit Repo(const RepoSettings& settings);

    bool fnIsEmpty();
};