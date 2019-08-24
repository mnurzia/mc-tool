#include "Sink.hpp"

namespace App::Components::LogView {

	void Sink::sink_it_(const spdlog::details::log_msg& msg) {
		Line out;
		fmt::memory_buffer obuf;
		formatter_->format(msg, obuf);
		out.content = std::string(msg.payload.data(), msg.payload.size());
		out.level = msg.level;
		out.logger_name = std::string(msg.logger_name.data(), msg.logger_name.size());
		out.thread_id = msg.thread_id;
		out.time = msg.time;
		out.time_fmt = std::string(&obuf[0], obuf.size());
		if (messages_size < messages_max) {
			messages.push_back(out);
			messages_size++;
		} else {
			messages[messages_ptr] = out;
		}
		messages_ptr++;
		if (messages_ptr == messages_max) {
			messages_ptr = 0;
		}
	}

	void Sink::flush_() {

	}

	std::vector<Line>& Sink::getMessages() {
		return messages;
	}

}