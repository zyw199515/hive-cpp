package(default_visibility=["//visibility:public"])

cc_library(
    name="enums",
    hdrs=["enums.h"]
)

cc_library(
    name="pos",
    hdrs=["pos.h"],
    srcs=["pos.cc"],
    deps=["@boost//:functional"],
)

cc_library(
    name="test_deps",
    deps=[
        "@gtest",
        "@glog",
        "@gtest//:gtest_main"
    ]
)

cc_test(
    name="pos_test",
    srcs=["pos_test.cc"],
    deps=[
        ":pos",
        ":test_deps",
    ]
)