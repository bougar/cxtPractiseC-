set(LLVM_LINK_COMPONENTS
  Analysis
  Core
  IRReader
  Passes
  )

include_directories(include)

add_subdirectory(src)

add_llvm_tool(Cxt001
  Cxt001.cpp
  )

target_link_libraries(Cxt001
  Cxt001Pass
  )
