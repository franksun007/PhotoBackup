#ifndef _PHOTOBACKUP_UTILS_UTILS_H_
#define _PHOTOBACKUP_UTILS_UTILS_H_

#include <string>
#include <vector>
#include <cstdlib>

namespace photobackup {

// debug print
#ifdef DEBUG

#define DEBUG_PRINT(...) \
  std::cerr << "INFO: " << __FILE__ << ":" \
                        << __func__ << ":" \
                        << __LINE__ << ": " ; \
  do { std::cerr << __VA_ARGS__; } while (0); \
  std::cerr << std::endl

#else

#define DEBUG_PRINT(...)

#endif

// Execute command line
std::string cmd_execute(const char* cmd);
std::string cmd_execute(const std::string &cmd);

std::string pipe_all(const std::vector<std::string> &cmds);
std::string construct_grep_cond(const std::string &cond);

}

#endif // _PHOTOBACKUP_UTILS_UTILS_H_
