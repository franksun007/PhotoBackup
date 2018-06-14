#include "gtest/gtest.h"
#include "disk_info.h"

namespace photobackup {

TEST(DiskInfoTest, TestConstructor) {
  DiskInfo s1;
  ASSERT_EQ(s1.get_directory().empty(), true);

  std::string dir = "/";
  DiskInfo s2(dir);
  ASSERT_EQ(s2.get_directory(), dir);
}

TEST(DiskInfoTest, TestReadAllDiskProperty) {
  DiskInfo s1("/");
  s1.read_all_disk_property();
  ASSERT_FALSE(s1.get_all_disk_property().empty());
}

TEST(DiskInfoTest, TestSearchTargetDiskProperty) {
  DiskInfo s1("/");
  s1.read_all_disk_property();
  s1.search_target_disk_property();
  ASSERT_TRUE(0);
}

}
