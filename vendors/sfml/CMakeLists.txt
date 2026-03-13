message(STATUS "Fetching SFML..")

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NEWORK FALSE)

FetchContent_MakeAvailable(sfml)
