// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include <boost/test/unit_test.hpp>

#include <memory>
#include "..\src\PDFReader.h"

BOOST_AUTO_TEST_SUITE(SpecSuite, * boost::unit_test::disabled())

BOOST_AUTO_TEST_CASE(CryptoProSigned_details) {
	std::string filename(".//test//resources//crypto_pro_signed.pdf");

	std::unique_ptr<PDFReader> pdfReader = std::make_unique<PDFReader>(filename);
	std::shared_ptr<PDFDoc> pdfDocument = pdfReader->getPDFDocument();

	int major = 0, minor = 0;
	std::string pdfVersion = pdfDocument->getPDFVersion(&major, &minor);

	std::uintmax_t pdfFileSize = pdfDocument->getFileSize();
	int count = pdfDocument->getNumberOfPages();

	BOOST_TEST(pdfVersion == "1.6", "wrong pdf-version getted");
	BOOST_TEST(major == 1);
	BOOST_TEST(minor == 6);
	BOOST_TEST(pdfFileSize == 1224838);
	//BOOST_TEST(count == 1);
}

BOOST_AUTO_TEST_CASE(DigitalSignatureTS_details) {
	std::string filename(".//test//resources//digital_signature_timestamp.pdf");

	std::unique_ptr<PDFReader> pdfReader = std::make_unique<PDFReader>(filename);
	std::shared_ptr<PDFDoc> pdfDocument = pdfReader->getPDFDocument();

	int major = 0, minor = 0;
	std::string pdfVersion = pdfDocument->getPDFVersion(&major, &minor);

	std::uintmax_t pdfFileSize = pdfDocument->getFileSize();
	int count = pdfDocument->getNumberOfPages();

	BOOST_TEST(pdfVersion == "1.4", "wrong pdf-version getted");
	BOOST_TEST(major == 1);
	BOOST_TEST(minor == 4);
	BOOST_TEST(pdfFileSize == 46016);
	//BOOST_TEST(count == 1);
}

BOOST_AUTO_TEST_SUITE_END()