#pragma once

#include <fstream>
#include <mutex>
//#include <algorithm>
#include <iostream>  // todo: delete it

#include "Tokeniser.h"

class XRef {
public:
	XRef() = delete;
	XRef(std::unique_ptr<Tokeniser>& documentTokeniser);
	~XRef() noexcept;

private:
	void read_xref(std::unique_ptr<Tokeniser>& documentTokeniser);
	bool read_xref_stream(std::unique_ptr<Tokeniser>& documentTokeniser);

private:
	mutable std::recursive_mutex m_mutex;

	//int64_t m_lastXrefPos;
};