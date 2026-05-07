add_subdirectory(fsmlib)

# 진도에 맞춰 step 을 하나씩 활성화하거나 현재 step만 빌드한다.
# 라이브러리 확장이 끝나기 전에 활성화하면 빌드가 깨진다.
add_subdirectory(01_build_core)
add_subdirectory(02_use_gripper)
add_subdirectory(03_extend_hooks)
add_subdirectory(04_use_safety)
