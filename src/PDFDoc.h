// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
#include "XRef.hpp"
#include "Tokeniser.hpp"

class PDFDoc {
 private:
     struct PDFVersion {
         std::string pdfVersion;
         int major;
         int minor;
     };

 public:
     PDFDoc() = delete;
     explicit PDFDoc(const std::filesystem::path& filePath);
     virtual ~PDFDoc() noexcept;

     std::uintmax_t getFileSize() const;

     const std::string getPDFVersion(int* major, int* minor) const;
     int getNumberOfPages() const;

 private:
     void tokenize_document(PDFFileStream& pdfFileStream);
     void tokenize_document(PDFArrayStream& pdfArrayStream);

 protected:
     mutable std::recursive_mutex m_mutex;

     std::uintmax_t m_fileSize;

     PDFVersion m_pdfVersion;
     std::unique_ptr<XRef> m_xref;
};
