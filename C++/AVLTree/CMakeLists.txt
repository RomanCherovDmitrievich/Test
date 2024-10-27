add_executable(AVLTree
  AVLTree.cpp
  )
set_property(TARGET AVLTree PROPERTY COMPILE_WARNING_AS_ERROR ON)
if(MSVC)
  target_compile_options(AVLTree PRIVATE /W4 /WX)
else()
  target_compile_options(AVLTree PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference)
endif()
