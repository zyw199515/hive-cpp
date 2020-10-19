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
        "@gtest//:gtest_main",
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

cc_library(
    name="state",
    srcs=["state.cc"],
    hdrs=["state.h"],
    deps=[
        ":enums",
        ":pos",
        "//third_party/mcts:mcts",
        "@boost//:optional",
        "@glog"
    ]
)

cc_library(
    name="moves",
    srcs=["moves.cc"],
    hdrs=["moves.h"],
    deps=[
        ":state",
        "@glog",
        "@boost//:functional"
    ]
)

cc_test(
    name="moves_test",
    srcs=["moves_test.cc"],
    deps=[
        ":moves",
        ":test_deps"
    ]
)