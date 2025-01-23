//
// Created by Timmie on 21.01.2025.
//

#ifndef PREPARED_COMMANDS_HPP
#define PREPARED_COMMANDS_HPP
#include "command.hpp"

#define DEFINE_CMND(x) class x##Command : public Command { public: x##Command(); void* action(ParsedArgs) const override; }; class x##Args : public CommandArgs { public: x##Args(); }

DEFINE_CMND(Init);
DEFINE_CMND(Add);
DEFINE_CMND(Commit);
DEFINE_CMND(Checkout);
DEFINE_CMND(Merge);
DEFINE_CMND(RevertTo);

#endif //PREPARED_COMMANDS_HPP
