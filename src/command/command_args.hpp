#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>

class Arg
{
private:
    std::string str_name;
    std::string str_description;
    std::string str_short_name;

    bool b_required;

    std::vector<std::string> definitions;

public:
    Arg(const char *name, const char *desc);
    Arg(const char *name, const char *desc, std::vector<std::string> definitions);

    Arg(const char *name, const char *desc, bool required);
    Arg(const char *name, const char *desc, std::vector<std::string> definitions, bool required);

    bool isRequired() const;

    const char *getName() const;
    const char *getDescription() const;
    std::vector<std::string> getDefinitions() const;

    bool parse(const char *str) const;

    bool operator==(const Arg &other) const;
};

struct ParsedArgs
{
    std::unordered_set<std::string> parsed_args;
    bool hasArg(const char *str) const;
};


class CommandArgs
{
private:
    std::vector<Arg *> expected_args;

public:
    CommandArgs();
    CommandArgs(std::vector<Arg *> args);

    virtual ParsedArgs parseArgs(const std::vector<std::string> args) const;

    virtual ~CommandArgs();
};
