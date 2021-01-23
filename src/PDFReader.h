// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <string>
#include <filesystem>
#include <memory>

#include "PDFDoc.h"

class PDFReader {
 public:
     PDFReader() = delete;
     explicit PDFReader(const std::string& filename);
     virtual ~PDFReader() noexcept;

     const std::shared_ptr<PDFDoc> getPDFDocument();

 private:
     void load_from_file();

 private:
     std::string m_password;
     std::shared_ptr<PDFDoc> m_doc;
     std::filesystem::path m_filePath;
};
