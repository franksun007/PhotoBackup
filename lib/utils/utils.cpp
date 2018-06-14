
#include "utils.h"

#include <memory>
#include <iostream>


namespace photobackup {
namespace {

#define BUFFER_SIZE 256

}

std::string pipe_all(const std::vector<std::string> &cmds) {
  std::string ret;
  for (unsigned int i = 0; i < cmds.size() - 1; i++) {
    const std::string &cmd = cmds[i];
    ret += cmd + " | ";
  }
  ret += cmds[cmds.size() - 1];
  return ret;
}

std::string construct_grep_cond(const std::string &cond) {
  return std::string("grep \"" + cond + "\"");
}

std::string cmd_execute(const std::string &cmd) {
  return cmd_execute(cmd.c_str());
}

// Exec a command line command and return the value to the parent function
std::string cmd_execute(const char* cmd) {
  // TODO: FACTOR VERBOSE OUT
  std::cout << "****************************************" << std::endl;
  std::cout << "\tExecuting command: " << std::endl;
  std::cout << "\t\t" << cmd << std::endl;
  std::cout << "****************************************" << std::endl;

  char buffer[BUFFER_SIZE];
  std::string result = "";
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  // TODO: FACTOR THOSE OUT
  if (!pipe) throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get())) {
      if (fgets(buffer, BUFFER_SIZE, pipe.get()) != NULL)
          result += buffer;
  }
  return result;
}

}
