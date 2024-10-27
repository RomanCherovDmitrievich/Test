add_executable (Test
  Main.cpp
  )
set_property(TARGET Test PROPERTY COMPILE_WARNING_AS_ERROR ON)
if(MSVC)
  target_compile_options(Test PRIVATE /W4 /WX)
else()
  target_compile_options(Test PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference)
endif()
