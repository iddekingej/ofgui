enable_testing()

set(test_SRCS   test/lib/testclass.cpp
		test/lib/runner.cpp
		test/lib/testconfig.cpp
		test/base/test01.cpp
		test/base/test_os.cpp
		test/base/run_base.cpp
)
add_executable(ofgui_test test/test.cpp ${test_SRCS} ${ofgui_SRCS})
target_link_libraries(ofgui_test
    Qt5::Widgets
    KF5::CoreAddons
    KF5::ConfigCore
    KF5::I18n
)


install(TARGETS ofgui_test  ${INSTALL_TARGETS_DEFAULT_ARGS})
add_subdirectory(test)

