#pragma once
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/Logger.h>
#include <quill/sinks/ConsoleSink.h>
#include <quill/sinks/FileSink.h>
#include "utilities.h"

namespace LogSystem {
	inline quill::Logger* logger = nullptr;
	inline quill::Logger* init(const std::string& name = "root", const bool debug = true)
	{
		quill::Backend::start();

		// Tạo sink
		auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("Console");
		auto file_sink = quill::Frontend::create_or_get_sink<quill::FileSink>("logs\\" + name,
			[] {
				quill::FileSinkConfig cfg;
				cfg.set_open_mode('w');
				cfg.set_filename_append_option(quill::FilenameAppendOption::StartCustomTimestampFormat, " %Y-%m-%d %H-%M-%S.log");
				return cfg;
			}(), quill::FileEventNotifier{});

		// Logger của chúng ta đây :3
		logger = quill::Frontend::create_or_get_logger(name, { console_sink, file_sink });
		if (debug) 
			logger->set_log_level(quill::LogLevel::Debug);
		else logger->set_log_level(quill::LogLevel::Info);

		return logger;
	}
};