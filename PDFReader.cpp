#include "PDFReader.h"

//#include <iostream>  //
//#include <memory>

PDFReader::PDFReader(const std::string& filename) : m_password(""), m_doc(nullptr), m_filePath(filename) {
	std::cout << u8"this is a ctor::CPDFReader" << std::endl;  // todo: cout delete it

	load_from_file(m_filePath);
}

PDFReader::~PDFReader() noexcept {
	std::cout << u8"this is a dtor::CPDFReader" << std::endl;  // todo: cout delete it
}

void PDFReader::load_from_file(const std::filesystem::path& filePath) {
	m_doc = std::make_shared<PDFDoc>(filePath);
}

const std::shared_ptr<PDFDoc> PDFReader::getPDFDocument() {
	return m_doc;
}