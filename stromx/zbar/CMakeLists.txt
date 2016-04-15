configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h
)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${zbar_DIR}
)

set (SOURCES 
    Scan.cpp
    Zbar.cpp
)

add_library (stromx_zbar SHARED ${SOURCES})

set(VERSION_STRING "${STROMX_ZBAR_VERSION_MAJOR}.${STROMX_ZBAR_VERSION_MINOR}.${STROMX_ZBAR_VERSION_PATCH}")

set_target_properties (stromx_zbar PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)

target_link_libraries (stromx_zbar
    ${zbar_LIB}
    ${stromx_RUNTIME_LIB}
)

if(WIN32)
    install (TARGETS stromx_zbar
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else(WIN32)
    install (TARGETS stromx_zbar
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif(WIN32)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/stromx/zbar
    DESTINATION include/stromx
    FILES_MATCHING PATTERN "*.h"
    PATTERN "test" EXCLUDE
)

install(FILES ${CMAKE_BINARY_DIR}/stromx/zbar/Config.h
    DESTINATION include/stromx/zbar
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)
