package(default_visibility = ["//visibility:public"])

cc_library(
    name = "disk_info",
    hdrs = ["disk_info.h", ],
    srcs = ["disk_info.cpp", ],
    deps = [
      "//lib/definition:constants",
      "//lib/utils:utils",
      "@googletest//:gtest_main",
    ],
    linkopts = [
      "-lboost_system",
      "-lboost_filesystem",
    ]
)

cc_binary(
    name = "disk_info_test",
    srcs = ["disk_info_test.cpp", ],
    deps = [
      ":disk_info"
    ],
)
