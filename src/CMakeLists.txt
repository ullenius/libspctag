SET(libspctag_src spctag.c)

ADD_LIBRARY(spctag SHARED ${libspctag_src})

SET_TARGET_PROPERTIES(
  spctag
    PROPERTIES 
      SOVERSION 1
      VERSION 1.0.2
)

INSTALL(FILES spctag.h DESTINATION "include")
INSTALL(TARGETS spctag DESTINATION "lib")
