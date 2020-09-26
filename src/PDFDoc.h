#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <memory>
#include <mutex>
#include <iostream>  // todo: delete it

#include "XRef.h"
#include "Tokeniser.h"

class PDFDoc {
private:
	struct PDFVersion {
		std::string pdfVersion;
		int major;
		int minor;
	};

public:
	PDFDoc() = delete;
	PDFDoc(const std::filesystem::path& filePath);
	~PDFDoc() noexcept;

	bool isOk() const;
	std::uintmax_t getFileSize() const;

	const std::string getPDFVersion(int* major, int* minor) const;
	int getNumberOfPages() const;

private:
	void tokenize_document(std::ifstream& pdfFileStream);

private:
	bool m_isOK;
	std::uintmax_t m_fileSize;

	PDFVersion m_pdfVersion;
	std::unique_ptr<XRef> m_xref;

	mutable std::recursive_mutex m_mutex;
};