#pragma once

#include <fstream>
#include <any>
#include <utility>
#include <iostream>  // todo: delete it

static const char specialChars[256] = {
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, // 0x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1x
	1, 0, 0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, // 2x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, // 3x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, // 5x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, // 7x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9x
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // ax
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // bx
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // cx
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // dx
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // ex
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // fx
};

class Tokeniser {
public:
	enum class TokenTypes {
		kTK_NA = 0,
		kTK_NUMBER_INT,
		kTK_NUMBER_REAL,
		kTK_STRING,
		kTK_NAME,
		kTK_COMMENT,
		kTK_START_ARRAY,
		kTK_END_ARRAY,
		kTK_START_DIC,
		kTK_END_DIC,
		kTK_REF,
		kTK_OTHER,
		kTK_ENDOFFILE
	};

public:
	Tokeniser() = delete;
	Tokeniser(std::ifstream& pdfFileStream, std::streamoff startPos = 0);
	~Tokeniser() noexcept;

	std::string tokenizeDocumentHeader(int* major, int* minor) const;
	std::streamoff getStartXref(bool andLocate = true) const;
	std::pair<std::any, Tokeniser::TokenTypes> nextToken() const;

	void seek(std::streamoff pos) const;

	const char* getVersionToken() const;

	static bool isWhitespace(int c);

	


private:
	std::ifstream& m_pdfFileStream;

	const char* m_pdfVersionToken;
};