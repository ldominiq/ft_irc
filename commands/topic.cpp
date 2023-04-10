//
// Created by Lucas Dominique on 4/6/23.
//

#include "../CommandHandler.hpp"

void	topic(TcpListener &SERV, Client &client, std::vector<std::string> params) {
	const std::string user_id = user_id(client.get_nick(), client.get_username());
	if (params.size() == 0) {
		MessageHandler::HandleMessage(client.get_fd(), ERR_NEEDMOREPARAMS(user_id, "TOPIC"));
		return;
	}

	Channel *channel = SERV._is_channel(params[0]);
	if (!channel) {
		MessageHandler::HandleMessage(client.get_fd(), ERR_NOSUCHCHANNEL(user_id, params[0]));
		return;
	}

	if (!channel->is_user_in_channel(client.get_fd())) {
		MessageHandler::HandleMessage(client.get_fd(), ERR_NOTONCHANNEL(user_id, params[0]));
		return;
	}

	if (params.size() == 1) {
		std::string topic = channel->get_topic();
		if (topic.empty()) {
			MessageHandler::HandleMessage(client.get_fd(), RPL_NOTOPIC(user_id, params[0]));
			return;
		}
		MessageHandler::HandleMessage(client.get_fd(), RPL_TOPIC(user_id, params[0], topic));
		MessageHandler::HandleMessage(client.get_fd(), RPL_TOPICWHOTIME(user_id, params[0], channel->get_topic_user(), channel->get_topic_time()));
	}

	std::stringstream new_topic_ss;
	for (size_t i = 1; i < params.size(); i++) {
		new_topic_ss << params[i] << " ";
	}
	std::string new_topic = new_topic_ss.str();
	new_topic.erase(new_topic.size() - 1); // Remove the extra space at the end

	char delimiter = ':';

	size_t pos = new_topic.find(delimiter);
	std::string result = new_topic.substr(pos + 1);

	channel->set_topic(result);
	channel->set_topic_user(client.get_nick());

	std::time_t timestamp = std::time(nullptr);
	std::string timestamp_str = std::to_string(timestamp);
	channel->set_topic_time(timestamp_str);

	channel->send_to_users(client.get_nick(), ":" + user_id + " TOPIC " + channel->get_name() + " :" + result);
	MessageHandler::HandleMessage(client.get_fd(), ":" + user_id + " TOPIC " + channel->get_name() + " :" + result + "\r\n");
}