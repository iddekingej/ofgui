project (ofgui)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.2.0")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Open file gui")
SET(CPACK_PACKAGE_VENDOR "iddekingej@lycos.com")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Readme.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_RPM_PACKAGE_NAME  "ofgui")
SET(CPACK_RPM_PACKAGE_SUMMARY "Open file gui")
SET(CPACK_RPM_PACKAGE_LICENSE "GPLv2.0")
SET(CPACK_RPM_PACKAGE_GROUP,"Admin")
SET(CPACK_RPM_PACKAGE_VENDOR,"idekingej@lycos.com")
SET(CPACK_RPM_PACKAGE_DESCRIPTION,"Open file gui")
set(CPACK_RPM_PACKAGE_RELEASE "1")

include(CPack)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS 
    Core    
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons      
    I18n            
    WidgetsAddons   
    Config
)



feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
    

KI18N_WRAP_UI(ofgui_ui_h ui/main.ui)

qt5_add_resources(ofgui_res_h icons.qrc)

set(ofgui_SRCS   
	src/main.cpp 
	src/mainwindow.cpp
	src/os.cpp
	src/data.cpp
)

set(ofgui_SRCS ${ofgui_res_h} ${ofgui_SRCS} ${ofgui_ui_h})

add_executable(ofgui ${ofgui_SRCS})


target_link_libraries(ofgui
    Qt5::Widgets
    KF5::CoreAddons
    KF5::ConfigCore
    KF5::I18n
    ${LIB_BLKID}    
)


install(TARGETS ofgui  ${INSTALL_TARGETS_DEFAULT_ARGS})
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

add_subdirectory(po)

