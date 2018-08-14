SET(JOURNALSOURCE Journal.cpp Page.cpp mmap_helper.cpp)

ADD_LIBRARY(journal SHARED ${JOURNALSOURCE})