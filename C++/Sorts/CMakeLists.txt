
add_executable (Sorts
  Sorts.cpp
  )
set_property(TARGET Sorts PROPERTY COMPILE_WARNING_AS_ERROR ON)
if(MSVC)
  target_compile_options(Sorts PRIVATE /W4 /WX)
else()
  target_compile_options(Sorts PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference)
endif()
