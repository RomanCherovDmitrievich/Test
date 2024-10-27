
add_executable (StructExample
  StructExample.cpp
  )
set_property(TARGET StructExample PROPERTY COMPILE_WARNING_AS_ERROR ON)
if(MSVC)
  target_compile_options(StructExample PRIVATE /W4 /WX)
else()
  target_compile_options(StructExample PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference)
endif()
