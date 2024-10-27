add_executable (SentenceSeparator
  SentenceSeparator.cpp
  )
set_property(TARGET SentenceSeparator PROPERTY COMPILE_WARNING_AS_ERROR ON)
if(MSVC)
  target_compile_options(SentenceSeparator PRIVATE /W4 /WX)
else()
  target_compile_options(SentenceSeparator PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference)
endif()
