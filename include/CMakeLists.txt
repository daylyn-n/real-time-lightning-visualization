add_library(project_headers INTERFACE)

target_include_directories(project_headers INTERFACE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_SOURCE_DIR}/thirdparty/glm-master
)
