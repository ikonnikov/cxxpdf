// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#define BOOST_TEST_MODULE PDF unit-tests
#include <boost/test/included/unit_test.hpp>

#include <memory>
#include "..\src\PDFReader.h"

BOOST_AUTO_TEST_SUITE(BaseSuite)

BOOST_AUTO_TEST_CASE(MinimalPage_details, * boost::unit_test::disabled()) {
	std::string filename(".//test//resources//minimal.pdf");

	std::unique_ptr<PDFReader> pdfReader = std::make_unique<PDFReader>(filename);
	std::shared_ptr<PDFDoc> pdfDocument = pdfReader->getPDFDocument();

	int major = 0, minor = 0;
	std::string pdfVersion = pdfDocument->getPDFVersion(&major, &minor);

	std::uintmax_t pdfFileSize = pdfDocument->getFileSize();
	int count = pdfDocument->getNumberOfPages();

	BOOST_TEST(pdfVersion == "1.1", "wrong pdf-version getted");
	BOOST_TEST(major == 1);
	BOOST_TEST(minor == 1);
	BOOST_TEST(pdfFileSize == 739);
	BOOST_TEST(count == 1, "file contains one page only");
}

BOOST_AUTO_TEST_CASE(EmptyPage_details, * boost::unit_test::enabled()) {
	std::string filename(".//test//resources//empty_page.pdf");

	std::unique_ptr<PDFReader> pdfReader = std::make_unique<PDFReader>(filename);
	std::shared_ptr<PDFDoc> pdfDocument = pdfReader->getPDFDocument();

	int major = 0, minor = 0;
	std::string pdfVersion = pdfDocument->getPDFVersion(&major, &minor);

	std::uintmax_t pdfFileSize = pdfDocument->getFileSize();
	int count = pdfDocument->getNumberOfPages();

	BOOST_TEST(pdfVersion == "1.6", "wrong pdf-version getted");
	BOOST_TEST(major == 1);
	BOOST_TEST(minor == 6);
	BOOST_TEST(pdfFileSize == 4880);
	BOOST_TEST(count == 1);
}

BOOST_AUTO_TEST_SUITE_END()