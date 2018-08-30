file(GLOB gameTest_SRC
	"*.h"
	"*.cpp"
)

add_gmock_test(gameTest ${gameTest_SRC})
target_link_libraries(gameTest 
	game 
    scripts)