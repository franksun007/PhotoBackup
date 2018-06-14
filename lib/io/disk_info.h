#ifndef _PHOTOBACKUP_IO_DISKINFO_H_
#define _PHOTOBACKUP_IO_DISKINFO_H_

#include <string>
#include <boost/property_tree/ptree.hpp>

namespace photobackup {
namespace {

namespace pt = boost::property_tree;

}

  class DiskInfo {
    public:
      DiskInfo() {};
      DiskInfo(std::string directory);

      std::string get_serial_id();
      std::string get_directory() { return _directory; };

      pt::ptree get_all_disk_property() { return _all_disk_property; };
      pt::ptree get_target_disk_property() { return _target_disk_property; };

      void read_all_disk_property();
      void search_target_disk_property();

    private:
      std::string   _directory;
      pt::ptree     _all_disk_property;
      pt::ptree     _target_disk_property;
  };
}

#endif // _PHOTOBACKUP_IO_DISKINFO_H_
