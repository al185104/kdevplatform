
add_definitions( -DKDE_DEFAULT_DEBUG_AREA=9515 )

########### next target ###############

set(kdevexecute_PART_UIS
    nativeappconfig.ui
)

set(kdevexecute_PART_SRCS

    projecttargetscombobox.cpp
    executeplugin.cpp
    nativeappconfig.cpp
    nativeappjob.cpp
)

qt5_wrap_ui( kdevexecute_PART_SRCS ${kdevexecute_PART_UIS} )
add_library(kdevexecute MODULE ${kdevexecute_PART_SRCS})
target_link_libraries(kdevexecute 
    KF5::KCMUtils
    KDev::Interfaces
    KDev::Util
    KDev::Project
    KDev::OutputView)

install(TARGETS kdevexecute DESTINATION ${PLUGIN_INSTALL_DIR}/kdevplatform/${KDEV_PLUGIN_VERSION})


########### install files ###############
install(FILES iexecuteplugin.h DESTINATION ${INCLUDE_INSTALL_DIR}/kdevplatform/execute COMPONENT Devel)
configure_file(kdevexecute.desktop.cmake ${CMAKE_CURRENT_BINARY_DIR}/kdevexecute.desktop)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kdevexecute.desktop DESTINATION ${SERVICES_INSTALL_DIR})
