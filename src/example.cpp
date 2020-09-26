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