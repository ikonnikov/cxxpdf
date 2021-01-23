// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include <Windows.h>

#include <iostream>
#include <clocale>
#include <memory>
#include <fstream>

#include "PDFReader.h"

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);

    std::string filename(argv[1]);

    std::cout.imbue(std::locale("ru_RU.UTF-8"));
    std::cout << u8"Привет, мир!" << std::endl;

    std::unique_ptr<PDFReader> pdfReader = std::make_unique<PDFReader>(filename);

    std::shared_ptr<PDFDoc> pdfDocument = pdfReader->getPDFDocument();

    int major = 0, minor = 0;
    std::string pdfVersion = pdfDocument->getPDFVersion(&major, &minor);

    return 0;
}
