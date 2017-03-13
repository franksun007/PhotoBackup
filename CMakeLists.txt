cmake_minimum_required(VERSION 3.6)
project(PhotoBackup)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/PhotoBackup.cpp src/CommandLineEssential.cpp include/CommandLineEssential.h include/Definition.h include/CopyRecentDetails.h src/CopyRecentDetails.cpp test/Test_CopyRecentDetails.cpp include/PhotoBackup.h include/Shared.h src/Shared.cpp test/Test_CopyRecentDetails.h test/test_suite.cpp test/test_suite.h)

set(BOOST_VERSION 1.63.0)
link_directories(/usr/local/Cellar/boost/${BOOST_VERSION}/include)
link_libraries(boost_system boost_filesystem boost_program_options)

add_executable(PhotoBackup ${SOURCE_FILES})

add_custom_target(COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})