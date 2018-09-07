CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE_SOURCES)
  
list(APPEND EXTERNAL_SOURCES
	"../worldserver/CommandLine/CliRunnable.cpp"
	"../worldserver/CommandLine/CliRunnable.h"
	"../worldserver/RemoteAccess/RASession.cpp"
	"../worldserver/RemoteAccess/RASession.h"
	"../worldserver/TCSoap/TCSoap.cpp"
	"../worldserver/TCSoap/TCSoap.h")
list(APPEND PRIVATE_SOURCES ${EXTERNAL_SOURCES})

if( WIN32 )
  list(APPEND PRIVATE_SOURCES ${sources_windows})
endif()

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER PrecompiledHeaders/worldPCH.h)
endif()
GroupSources(${CMAKE_CURRENT_SOURCE_DIR}
	${EXTERNAL_SOURCES})
file(GLOB worldserverTest_SRC
	"*.h"
	"*.cpp"
)
add_gmock_test(worldserverTest ${worldserverTest_SRC}
	${PRIVATE_SOURCES})
target_link_libraries(worldserverTest
    trinity-core-interface
    scripts
    game
    gsoap
    readline)