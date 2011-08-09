configure_file("${CMAKE_CURRENT_SOURCE_DIR}/2annual.pl.in" "${CMAKE_CURRENT_BINARY_DIR}/2annual" @ONLY)

ADD_TEST(gmx     ${CMAKE_CURRENT_BINARY_DIR}/test.sh 2annual 0)
ADD_TEST(outlook ${CMAKE_CURRENT_BINARY_DIR}/test.sh 2annual 1)
ADD_TEST(vcard   ${CMAKE_CURRENT_BINARY_DIR}/test.sh 2annual 2)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/2annual DESTINATION bin)

#pod2man --date "`stat --format="%y" $< | cut -b-10`" --center "$(PACKAGE_NAME) Documentation"  --release "$(PACKAGE_NAME) v$(PACKAGE_VERSION)" --section 1  $< $@
