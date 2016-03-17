add_test(NAME stromx_zbar_test COMMAND stromx_zbar_test)
set_tests_properties(stromx_zbar_test PROPERTIES ENVIRONMENT "${TEST_ENVIRONMENT}")

if(MSVC)
    add_definitions(/DSTROMX_ZBAR_STATIC)
endif(MSVC)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/barcode.png ${CMAKE_CURRENT_BINARY_DIR}/barcode.png COPYONLY)

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIR}
    ${zbar_DIR}
)

set (SOURCES 
    ../Scan.cpp
    ../Zbar.cpp
    main.cpp
    ScanTest.cpp
)

add_executable(stromx_zbar_test ${SOURCES})

set_target_properties(stromx_zbar_test PROPERTIES FOLDER "test")

target_link_libraries (stromx_zbar_test
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${zbar_LIB}
    ${stromx_RUNTIME_LIB}
    ${stromx_CVSUPPORT_LIB}
)

