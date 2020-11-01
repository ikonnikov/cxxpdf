// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFDoc.h"

PDFDoc::PDFDoc(const std::filesystem::path& filePath) : m_fileSize(0), m_pdfVersion{"", 0, 0 }, m_xref(nullptr) {
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

	TokeniserPtr documentTokeniser = std::make_shared<Tokeniser>(pdfFileStream, 0);

	m_pdfVersion.pdfVersion = documentTokeniser->getDocumentHeader(&m_pdfVersion.major, &m_pdfVersion.minor);
	m_xref = std::make_unique<XRef>(documentTokeniser);
}

const std::string PDFDoc::getPDFVersion(int* major, int* minor) const {
	*major = m_pdfVersion.major;
	*minor = m_pdfVersion.minor;
	
	return m_pdfVersion.pdfVersion;
}

std::uintmax_t PDFDoc::getFileSize() const {
	return m_fileSize;
}

int PDFDoc::getNumberOfPages() const {
	return 0;  // todo: impl it
}
