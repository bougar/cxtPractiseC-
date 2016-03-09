set(LLVM_LINK_COMPONENTS
  Analysis
  Core
  IRReader
  Passes
  )

include_directories(include)

add_subdirectory(src)

add_llvm_tool(cxt1110
  Cxt001.cpp
  )

target_link_libraries(cxt1110
  cxt1110Pass
  )
