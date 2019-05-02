cmake_minimum_required(VERSION 3.5.2)

#
# Deploy qt for current target for debug. Adds post build step which calls windeployqt.exe. This
# should be used when you want to copy the Qt dlls to your build directory. For example, calling
# DeployQt(myExeTarget) will copy the Qt dlls and plugins myExeTarget.exe needs to the build 
# directory so the tests can be run. This does NOT add Qt dlls to the install directory. See the
# InstallQt macro.
#
# Note this should be called after use_qt5 macro (see UseQt5.cmake), or at the very least the
# variable Qt5_DIR must be set to the lib/cmake directory of Qt.
#
# Note this is windows specific
#
# Note if multiple projects in your build chain call this function, they will have dependencies
# placed on each other to ensure that only one windeployqt is running at a time when the parallel
# compile option is turned on.
#
# param target: target which needs Qt dlls copied to its directory
#
macro(DeployQt target)
    set(dir ${Qt5_DIR}/../..)
    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${dir} PATH_SUFFIXES bin)
    if (NOT WINDEPLOYQT_EXECUTABLE)
        message(WARNING "windeployqt.exe not found. Check that Qt5_DIR is set correctly and/or use \
                         use_qt5 macro in UseQt5.cmake. Currently looking with HINT ${dir}. \
                         Otherwise, may have issues finding Qt dependencies when trying to use \
                         ${target}")
    endif()
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${target}> --no-compiler-runtime)

    # If parallel compile options are used, there may be an issue if windployqt runs at the same
    # time, and tries to update the same files at the same time. Seems like it should be rare, but
    # we've seen it happen on the build servers. Therefore, we store the previous target that
    # deployed Qt in order to depend on it the next time. This will ensure that windeployqt never
    # runs at the same time. We accomplish this using a global property, as opposed to a cached
    # variable, because it needs to be cleared out everytime the project is re-generated.
    get_property(_previous_target GLOBAL PROPERTY winqtdeploy_previous_target)
    if(_previous_target)
        add_dependencies(${target} ${_previous_target})
    endif()
    set_property(GLOBAL PROPERTY winqtdeploy_previous_target ${target})
endmacro()

#
# Install qt for current target. Adds post build step which calls windeployqt.exe. Installs qt dlls 
# to ${CMAKE_INSTALL_PREFIX}/bin and plugins to ${CMAKE_INSTALL_PREFIX}/bin/plugins. This macro
# should be called for each configuration.
#
# Note target_install_path cannot contain a generator expression (i.e., $<...>). Those are evaluated
# at build time and only work for certain cmake functions (execute_process is not currently one of 
# them). A solution to get around this may be to generate a file with the target file name and then 
# use that file, but that seemed particularly hackish (even for cmake).
#
# Note this should be called after use_qt5 macro (see UseQt5.cmake), or at the very least the
# variable Qt5_DIR must be set to the lib/cmake directory of Qt.
#
# Note this is windows specific
#
# Example uses:
#    InstallQt(${CMAKE_INSTALL_PREFIX}/bin/myExe.exe Release)
#    InstallQt(${CMAKE_INSTALL_PREFIX}/bin/myExe.exe Release ${THIRDPARTY_DIR}/path/to/windeployqtexe)
#
# param target_install_path: path to installed target (must be dll or exe), e.g. 
#                            ${CMAKE_INSTALL_PREFIX}/bin/myExe.exe
# param configuration: Debug, Release, etc.
#
macro(InstallQt target_install_path configuration)
    set(dir ${Qt5_DIR}/../..)
    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${dir} PATH_SUFFIXES bin)
    if (NOT WINDEPLOYQT_EXECUTABLE)
        message(WARNING "windeployqt.exe not found. Check that Qt5_DIR is set correctly and/or use \
                         use_qt5 macro in UseQt5.cmake. Currently looking with HINT ${dir}. \
                         Otherwise, may have issues finding Qt dependencies when trying to use \
                         ${target}")
    endif()
    install(CODE "
        execute_process(COMMAND ${WINDEPLOYQT_EXECUTABLE} ${target_install_path} 
            --dir ${CMAKE_INSTALL_PREFIX}/bin 
            --plugindir ${CMAKE_INSTALL_PREFIX}/bin/plugins 
            --no-compiler-runtime)
        "
        CONFIGURATIONS ${configuration})
endmacro()

set(dir D:/projects/vapo/ThirdParty/qt/built-5.12.0/5.12.0/msvc2017_64/lib/cmake)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(Qt5_DIR ${dir})
set(Qt5Core_DIR ${Qt5_DIR}/Qt5Core)
set(Qt5Gui_DIR ${Qt5_DIR}/Qt5Gui)
set(Qt5Help_DIR ${Qt5_DIR}/Qt5Help)
set(Qt5Widgets_DIR ${Qt5_DIR}/Qt5Widgets)
set(Qt5Network_DIR ${Qt5_DIR}/Qt5Network)
set(Qt5Test_DIR ${Qt5_DIR}/Qt5Test)
set(Qt5WebEngineWidgets_DIR ${Qt5_DIR}/Qt5WebEngineWidgets)
set(Qt5Concurrent_DIR ${Qt5_DIR}/Qt5Concurrent)
