#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>

class Arg {
    private:
        std::string str_name;
        std::string str_description;
        std::string str_short_name;

        bool b_required;

        std::vector<std::string> definitions;
    public:
        Arg(const char* name, const char* desc);
        Arg(const char* name, const char* desc, std::vector<std::string> definitions);

        Arg(const char* name, const char* desc, bool required);
        Arg(const char* name, const char* desc, std::vector<std::string> definitions, bool required);

        bool isRequired() const;

        const char* getName() const;
        const char* getDescription() const;

        bool parse(const char* str) const;

        bool operator==(const Arg& other) const;
};

struct ParsedArgs {
    std::unordered_set<std::string> parsed_args;
    bool hasArg(const char* str) const;
};


class CommnandArgs {
    private:
        std::vector<Arg*> expected_args;
    public:
        CommnandArgs();
        CommnandArgs(std::vector<Arg*> args);

        // TODO: Реализовать
        virtual ParsedArgs parseArgs(const std::vector<std::string> args) const;

        virtual ~CommnandArgs();
};
