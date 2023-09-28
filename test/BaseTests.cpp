// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#define BOOST_TEST_MODULE PDF unit-tests
#include <boost/test/included/unit_test.hpp>

#include <memory>
#include "..\src\PDFReader.h"

BOOST_AUTO_TEST_SUITE(BaseSuite, *boost::unit_test::enabled())

BOOST_AUTO_TEST_CASE(MinimalPage_details, *boost::unit_test::disabled()) {
	std::string filename(".//test//resources//minimal.pdf");

	PDFReader pdfReader = PDFReader(filename);
	std::shared_ptr<PDFDoc> pdfDocument = pdfReader.getPDFDocument();

	int major = 0, minor = 0;
	std::string pdfVersion = pdfDocument->getPDFVersion(&major, &minor);

	BOOST_TEST(pdfVersion == "1.1", "wrong pdf-version");
	BOOST_TEST(major == 1);
	BOOST_TEST(minor == 1);
	BOOST_TEST(1003 == pdfDocument->getFileSize());
	BOOST_TEST(1 == pdfDocument->getNumberOfPages(), "file contains one page only");
}

BOOST_AUTO_TEST_CASE(EmptyPage_details, *boost::unit_test::enabled()) {
	std::string filename(".//test//resources//empty_page.pdf");

	PDFReader pdfReader = PDFReader(filename);
	std::shared_ptr<PDFDoc> pdfDocument = pdfReader.getPDFDocument();

	int major = 0, minor = 0;
	std::string pdfVersion = pdfDocument->getPDFVersion(&major, &minor);

	BOOST_TEST(pdfVersion == "1.6", "wrong pdf-version");
	BOOST_TEST(major == 1);
	BOOST_TEST(minor == 6);
	BOOST_TEST(4880 == pdfDocument->getFileSize());
	BOOST_TEST(1 == pdfDocument->getNumberOfPages(), "file contains one page only");
}

BOOST_AUTO_TEST_SUITE_END()