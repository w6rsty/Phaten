# Macro to add files from a directory grouped to the solution file
macro (add_engine_directory DIRECTORY)
    file (GLOB DIR_FILES ${DIRECTORY}/*.hpp ${DIRECTORY}/*.cpp)
    get_filename_component (GROUP ${DIRECTORY} NAME)
    source_group ("${GROUP}" FILES ${DIR_FILES})
    list (APPEND SOURCE_FILES ${DIR_FILES})
endmacro ()

macro (add_engine_directory_group DIRECTORY GROUP)
    file (GLOB DIR_FILES ${DIRECTORY}/*.hpp ${DIRECTORY}/*.cpp)
    source_group ("${GROUP}" FILES ${DIR_FILES})
    list (APPEND SOURCE_FILES ${DIR_FILES})
endmacro ()