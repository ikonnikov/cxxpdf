#pragma once

#include <string>
#include <filesystem>
#include <memory>

#include "PDFDoc.h"

class PDFReader {
public:
	PDFReader() = delete;
	PDFReader(const std::string& filename);
	~PDFReader() noexcept;

	const std::shared_ptr<PDFDoc> getPDFDocument();

private:
	void load_from_file(const std::filesystem::path& filePath);

private:
	std::string m_password;
	std::shared_ptr<PDFDoc> m_doc;
	std::filesystem::path m_filePath;
};