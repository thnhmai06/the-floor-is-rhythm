#include "logging.h" // Header
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/sinks/ConsoleSink.h>

quill::Logger* LogSystem::init(const std::string& name, const bool debug)
{
	quill::Backend::start();

	// Tạo sink
	auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("Console");
	auto file_sink = quill::Frontend::create_or_get_sink<quill::FileSink>("logs\\" + name,
		[] {
			quill::FileSinkConfig cfg;
			cfg.set_open_mode('w');
			cfg.set_filename_append_option(
				quill::FilenameAppendOption::StartCustomTimestampFormat,
				" %Y-%m-%d %H-%M-%S.log"
			);
			return cfg;
		}(), quill::FileEventNotifier{});

	// Logger của chúng ta đây :3
	logger = quill::Frontend::create_or_get_logger(name, { console_sink, file_sink });
	if (debug)
		logger->set_log_level(quill::LogLevel::Debug);
	else logger->set_log_level(quill::LogLevel::Info);

	return logger;
}