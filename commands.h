#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include "bank.h"

std::string process_command(Bank* bank, const std::string& input, bool& shutdown_flag);

#endif

