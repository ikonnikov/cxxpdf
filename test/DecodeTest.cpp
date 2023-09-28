// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include <boost/test/unit_test.hpp>

//#include <memory>
#include "..\src\StreamDecoder.h"

BOOST_AUTO_TEST_SUITE(DecodeSuite, *boost::unit_test::disabled())

BOOST_AUTO_TEST_CASE(DecodeCase_FlateDocode_ex1, *boost::unit_test::disabled()) {
	std::vector<char> inData = {104, -34, 98, 98, 100, 16, 96, 98, 96, 82, 98, 98, 96, -16, 97, 98, -8, -9, -115, -119, -127, -47, 26, 32, -64, 0, 17, 48, 2, -68};
	std::vector<char> outData = {1, 0, 16, 1, 2, 50, 1, 2, 126, 1, 0, 116, 1, 1, -81};
	
	std::int64_t predictor = 12;
	std::int64_t colors = 1;
	std::int64_t bitsPerComponent = 8;
	std::int64_t columns = 3;

	char* inBuffer = new char[inData.size()];
	std::copy(inData.begin(), inData.end(), inBuffer);

	std::vector<char> decodedData = StreamDecoder::decodeFilteredStream(inBuffer, inData.size(), {predictor, colors, bitsPerComponent, columns});

	delete[] inBuffer;

	BOOST_REQUIRE(outData.size() == decodedData.size());

	bool isEqual = std::equal(outData.begin(), outData.end(), decodedData.begin());

	BOOST_TEST(isEqual == true);
}

BOOST_AUTO_TEST_CASE(DecodeCase_FlateDocode_ex2, *boost::unit_test::disabled()) {
	std::vector<char> inData = {104, -34, 98, 98, 0, 2, 38, 70, -90, 63, 12, 76, 12, -116, -9, -128, 4, -17, 22, 16, -85, 23, 72, 48, 28, 7, 74, -68, -105, 7, -79, 24, 24, 97, 4, -45, 63, 32, -63, -56, 0, 16, 96, 0, -73, 65, 6, 29};
	std::vector<char> outData = {0, 0, 0, 0, 1, 2, -4, 0, 1, 3, -38, 0, 1, 16, -114, 0, 1, 17, 27, 0, 1, 17, -30, 0, 2, 0, 1, 0, 2, 0, 1, 1, 2, 0, 1, 2, 2, 0, 3, 0, 2, 0, 4, 0};

	std::int64_t predictor = 12;
	std::int64_t colors = 1;
	std::int64_t bitsPerComponent = 8;
	std::int64_t columns = 4;

	char* inBuffer = new char[inData.size()];
	std::copy(inData.begin(), inData.end(), inBuffer);

	std::vector<char> decodedData = StreamDecoder::decodeFilteredStream(inBuffer, inData.size(), {predictor, colors, bitsPerComponent, columns});

	delete[] inBuffer;

	BOOST_REQUIRE(outData.size() == decodedData.size());

	bool isEqual = std::equal(outData.begin(), outData.end(), decodedData.begin());

	BOOST_TEST(isEqual == true);
}

BOOST_AUTO_TEST_SUITE_END()