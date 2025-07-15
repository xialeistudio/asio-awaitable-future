# Examples CMakeLists.txt

# Function to create example executable
function(add_example name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE asio-future::asio-future)
    
    # Set compile options
    target_compile_options(${name} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -pedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )
    
    # Set output directory
    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
    )
endfunction()

# Add examples
add_example(basic_usage)
add_example(error_handling)
add_example(database_example)