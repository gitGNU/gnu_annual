include(${QT_USE_FILE})
include(CMakeListsFiles.txt)

QT4_ADD_RESOURCES(annual_RES_CPP ${annual_RES_SRC})
QT4_WRAP_CPP(annual_MOC_CPP ${annual_MOC_H})


include(${QT_USE_FILE})

add_executable(annual ${annual_SRCS} ${annual_MOC_CPP} ${annual_RES_CPP})
target_link_libraries(annual ${QT_LIBRARIES})

install(TARGETS annual DESTINATION bin)
configure_file("config.h.in" "config.h")
