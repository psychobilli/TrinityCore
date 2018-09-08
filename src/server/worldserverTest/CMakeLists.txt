CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE_SOURCES)

if( WIN32 )
  list(APPEND PRIVATE_SOURCES ${sources_windows})
endif()

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER PrecompiledHeaders/worldPCH.h)
endif()
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB worldserverTest_SRC
	"*.h"
	"*.cpp"
)

add_gmock_test(worldserverTest ${worldserverTest_SRC}
	${PRIVATE_SOURCES})
target_link_libraries(worldserverTest
    trinity-core-interface
	worldserverMoq
    scripts
    game
    gsoap
    readline)