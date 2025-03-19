#pragma once
#include <quill/Logger.h>
#include <quill/sinks/FileSink.h>

namespace LogSystem {
	inline quill::Logger* logger = nullptr;
	quill::Logger* init(const std::string& name = "root", const bool debug = true);
};