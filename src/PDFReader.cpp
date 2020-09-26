#include "PDFReader.h"

PDFReader::PDFReader(const std::string& filename) : m_password(""), m_doc(nullptr), m_filePath(filename) {
	load_from_file(m_filePath);
}

PDFReader::~PDFReader() noexcept {
	// dtor
}

void PDFReader::load_from_file(const std::filesystem::path& filePath) {
	m_doc = std::make_shared<PDFDoc>(filePath);
}

const std::shared_ptr<PDFDoc> PDFReader::getPDFDocument() {
	return m_doc;
}