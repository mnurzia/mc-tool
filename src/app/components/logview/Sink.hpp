#ifndef APP_COMPONENTS_LOGVIEW_LOGVIEWSINK_HPP
#define APP_COMPONENTS_LOGVIEW_LOGVIEWSINK_HPP

#include <mutex>
#include <tuple>
#include <vector>

#include <fmt/format.h>
#include <spdlog/sinks/base_sink.h>

namespace App::Components::LogView {

	struct Line {
		std::string content;
		std::string logger_name;
		std::string time_fmt;
		spdlog::level::level_enum level{ spdlog::level::off };
		spdlog::log_clock::time_point time;
		size_t thread_id{ 0 };
	};

	class Sink : public spdlog::sinks::base_sink<std::mutex> {

		size_t messages_size = 0;
		size_t messages_ptr = 0;
		size_t messages_max = 16384;
		std::vector<Line> messages;

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override;

		void flush_() override;

	public:
		std::vector<Line>& getMessages();
	};

}

#endif