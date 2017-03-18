project (ofgui)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.2.0")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

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
    

KI18N_WRAP_UI(ofgui_ui_h 
	ui/main.ui
	ui/monitor.ui
)

qt5_add_resources(ofgui_res_h icons.qrc)

set(ofgui_SRCS   
	src/main.cpp 
	src/mainwindow.cpp
	src/os.cpp
	src/data.cpp
	src/monitordialog.cpp
	src/processiterator.cpp
	src/filelogitem.cpp
	src/openfileiterator.cpp
	src/utils.cpp
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

