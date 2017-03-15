//
// Created by Chenfan Sun on 2/5/17.
//

#include "./test_suite.h"
#include "Test_CopyRecentDetails.h"

#include "../include/CopyRecentDetails.h"

#include "gtest/gtest.h"

using std::cout;
using std::endl;

TEST_F (Test_CopyRecentDetails, Test_CopyRecentDetailsConstructorBasic) {
    CopyRecentDetails::CopyRecentDetails crds;
    ASSERT_EQ(crds.getYear(), 0);
    ASSERT_EQ(crds.getDay(), 0);
    ASSERT_EQ(crds.getMonth(), 0);
    ASSERT_EQ(crds.getPhotoFilename(), "");
    ASSERT_EQ(crds.getDeviceID(), "");
    ASSERT_EQ(crds.getPhotoID(), "");

}

TEST_F (Test_CopyRecentDetails, Test_CopyRecentDetailsConstructorSetArgsException) {
    CopyRecentDetails::CopyRecentDetails crds;
    ASSERT_(crds.setArguments(""));
}

TEST_F (Test_CopyRecentDetails, Test_CopyRecentDetailsConstructorCopy) {
    CopyRecentDetails::CopyRecentDetails crds;
    ASSERT_EQ(crds.getYear(), 0);
    ASSERT_EQ(crds.getDay(), 0);
    ASSERT_EQ(crds.getMonth(), 0);
    ASSERT_EQ(crds.getPhotoFilename(), "");
    ASSERT_EQ(crds.getDeviceID(), "");
    ASSERT_EQ(crds.getPhotoID(), "");

    CopyRecentDetails::CopyRecentDetails crds2(crds);
    ASSERT_EQ(crds2.getYear(), 0);
    ASSERT_EQ(crds2.getDay(), 0);
    ASSERT_EQ(crds2.getMonth(), 0);
    ASSERT_EQ(crds2.getPhotoFilename(), "");
    ASSERT_EQ(crds2.getDeviceID(), "");
    ASSERT_EQ(crds2.getPhotoID(), "");

    ASSERT_EQ(crds2.getYear(), crds.getYear());
    ASSERT_EQ(crds2.getDay(), crds.getDay());
    ASSERT_EQ(crds2.getMonth(), crds.getMonth());
    ASSERT_EQ(crds2.getPhotoFilename(), crds.getPhotoFilename());
    ASSERT_EQ(crds2.getDeviceID(), crds.getDeviceID());
    ASSERT_EQ(crds2.getPhotoID(), crds.getPhotoID());
}



