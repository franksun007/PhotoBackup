cc_library(
    name = "definition",
    hdrs = ["Definition.h", ],
    visibility = ["//visibility:public", ],
)

cc_library(
    name = "shared",
    hdrs = ["Shared.h", ],
    srcs = ["Shared.cpp", ],
    visibility = ["//visibility:public", ],
)

cc_library(
    name = "cmd",
    hdrs = ["CommandLineEssential.h", ],
    srcs = ["CommandLineEssential.cpp", ],
    visibility = ["//visibility:public", ],
    deps = [
      ":definition",
      ":shared",
    ],
    linkopts = ["-lboost_program_options"],
)

cc_library(
    name = "photo_holder",
    hdrs = ["PhotoHolder.h", ],
    srcs = ["PhotoHolder.cpp", ],
    visibility = ["//visibility:public", ],
    deps = [
      ":shared",
    ],
)


cc_binary(
    name = "photo_backup",
    srcs = [
      "PhotoBackup.cpp",
      "PhotoBackup.h",
    ],
    visibility = ["//visibility:public", ],
    deps = [
      ":shared",
      ":photo_holder",
      ":cmd",
      ":definition",
    ],
    linkopts = [
      "-lboost_system",
      "-lboost_filesystem",
    ],
    data = [
      "//scripts:linux_diskutil"
    ]
)
