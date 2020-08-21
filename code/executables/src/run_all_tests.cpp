/*
 * run_all_tests.cpp
 *
 *  Created on: Aug 19, 2020
 *      Author: cady
 */


#include <gtest/gtest.h>
#include <iostream>
#include "google/protobuf/stubs/common.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    const int ret = RUN_ALL_TESTS();
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}

