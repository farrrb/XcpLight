macro (add_xcp_test NAME TIMEOUT)
    add_executable(${NAME} ${NAME}.c)
    target_link_libraries(${NAME} xcplight unity)
    add_test(${NAME} ${NAME})
    set_tests_properties(${NAME} PROPERTIES TIMEOUT ${TIMEOUT})
endmacro (add_xcp_test)

# Add tests
add_xcp_test(test_BasicCommands     5)
add_xcp_test(test_LoginProcedure    5)
add_xcp_test(test_Timestamp         5)
add_xcp_test(test_MemoryAccess      5)
