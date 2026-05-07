# fsmlib - 학습자가 점진적으로 키워가는 라이브러리.
#
# include/ 안의 헤더와 src/ 안의 소스를 자동으로 수집한다.
# 새 헤더/소스를 추가하면 다음 빌드에서 CONFIGURE_DEPENDS 가 자동으로 잡아준다.

file(GLOB_RECURSE FSMLIB_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)
file(GLOB_RECURSE FSMLIB_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(fsmlib STATIC ${FSMLIB_SOURCES} ${FSMLIB_HEADERS})

target_include_directories(fsmlib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
