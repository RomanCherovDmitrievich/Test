add_executable (ChronoExample
  ChronoExample.cpp
  )
set_property(TARGET ChronoExample PROPERTY COMPILE_WARNING_AS_ERROR ON)
if(MSVC)
  target_compile_options(ChronoExample PRIVATE /W4 /WX)
else()
  target_compile_options(ChronoExample PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference)
endif()
