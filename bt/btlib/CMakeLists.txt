# btlib - 학습자가 점진적으로 키워가는 BT 라이브러리.
#
# include/ 의 헤더와 src/ 의 소스를 자동으로 수집한다.
# 새 헤더/소스(예: Decorator.h, Blackboard.h, ...) 를 추가하면 다음 빌드에서
# CONFIGURE_DEPENDS 가 자동으로 잡아준다.

file(GLOB_RECURSE BTLIB_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)
file(GLOB_RECURSE BTLIB_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(btlib STATIC ${BTLIB_SOURCES} ${BTLIB_HEADERS})

target_include_directories(btlib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
