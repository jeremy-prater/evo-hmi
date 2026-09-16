# Optional libosmscout (offline OSM render + routing).
# Expected components: osmscout, osmscoutmap / osmscout_map, osmscoutmapqt / osmscout_map_qt.

find_path(LibOsmScout_INCLUDE_DIR
    NAMES osmscout/db/Database.h osmscout/Database.h
    PATH_SUFFIXES include
)

find_library(LibOsmScout_LIBRARY NAMES osmscout)
find_library(LibOsmScout_MAP_LIBRARY NAMES osmscoutmap osmscout_map)
find_library(LibOsmScout_MAPQT_LIBRARY NAMES osmscoutmapqt osmscout_map_qt osmscoutmap-qt)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibOsmScout
    REQUIRED_VARS LibOsmScout_INCLUDE_DIR LibOsmScout_LIBRARY LibOsmScout_MAP_LIBRARY
)

if(LibOsmScout_FOUND)
    set(LibOsmScout_INCLUDE_DIRS ${LibOsmScout_INCLUDE_DIR})
    set(LibOsmScout_LIBRARIES ${LibOsmScout_LIBRARY} ${LibOsmScout_MAP_LIBRARY})
    if(LibOsmScout_MAPQT_LIBRARY)
        list(APPEND LibOsmScout_LIBRARIES ${LibOsmScout_MAPQT_LIBRARY})
    endif()
endif()

mark_as_advanced(
    LibOsmScout_INCLUDE_DIR
    LibOsmScout_LIBRARY
    LibOsmScout_MAP_LIBRARY
    LibOsmScout_MAPQT_LIBRARY
)
