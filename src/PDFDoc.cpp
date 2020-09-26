#include "PDFDoc.h"

PDFDoc::PDFDoc(const std::filesystem::path& filePath) : m_isOK(false), m_fileSize(0), m_pdfVersion{"", 0, 0 }, m_xref(nullptr) {
	std::ifstream pdfFileStream(filePath, std::ios::binary | std::ios::in);

	if (!pdfFileStream.is_open()) {
		throw std::exception("Failed to open the file");
	}

	m_fileSize = std::filesystem::file_size(filePath);

	tokenize_document(pdfFileStream);
}

PDFDoc::~PDFDoc() noexcept {
	// dtor
}

void PDFDoc::tokenize_document(std::ifstream& pdfFileStream) {
	std::unique_lock<std::recursive_mutex> localLocker(m_mutex);

	std::unique_ptr<Tokeniser> documentTokeniser = std::make_unique<Tokeniser>(pdfFileStream, 0);

	m_pdfVersion.pdfVersion = documentTokeniser->tokenizeDocumentHeader(&m_pdfVersion.major, &m_pdfVersion.minor);
	m_xref = std::make_unique<XRef>(documentTokeniser);
}

const std::string PDFDoc::getPDFVersion(int* major, int* minor) const {
	*major = m_pdfVersion.major;
	*minor = m_pdfVersion.minor;
	
	return m_pdfVersion.pdfVersion;
}

bool PDFDoc::isOk() const {
	return m_isOK;
}

std::uintmax_t PDFDoc::getFileSize() const {
	return m_fileSize;
}

int PDFDoc::getNumberOfPages() const {
	return 0;
}