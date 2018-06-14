#include "disk_info.h"

#include <cassert>
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

// FOR DEBUG
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/property_tree/ini_parser.hpp>


#include "lib/utils/utils.h"
#include "lib/definition/constants.h"

namespace photobackup {
namespace {

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

bool validate_disk(const fs::path &dir) {
  return (fs::exists(dir) && fs::is_directory(dir));
}

std::string get_abs_path(const std::string &directory) {
  fs::path dir(directory);
  // TODO: CHECK disk is valid
  assert(validate_disk(dir));
  return fs::canonical(dir).string();
}

}

DiskInfo::DiskInfo(std::string directory) {
  _directory = get_abs_path(directory);
}

void DiskInfo::read_all_disk_property() {
#ifdef __linux__
  std::stringstream block_info;
  block_info << cmd_execute(CMD_IO_DISK_INFO_ALL);
  pt::read_json(block_info, _all_disk_property);
#endif
}

void DiskInfo::search_target_disk_property() {

  // DEBUG_PRINT(_all_disk_property.get_value());
  // BOOST_FOREACH(pt::ptree::value_type &v, _all_disk_property) {
    // DEBUG_PRINT(v.first, v.second.data());
  // }
  // DEBUG_PRINT(_all_disk_property.find(_directory)->second.data());
  std::ostringstream oss;
  pt::ini_parser::write_ini(oss, _all_disk_property);
  std::string text = oss.str();
  DEBUG_PRINT(text);
}

std::string DiskInfo::get_serial_id() {
  if (_all_disk_property.empty()) {
    read_all_disk_property();
  }
  if (_target_disk_property.empty()) {
    search_target_disk_property();
  }
  return "";
}

}
