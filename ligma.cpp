#include <iostream>
#include <vector>
#include <string>
#include <regex>


#include "sleepy_discord/sleepy_discord.h"
#define TOKEN "Your token here"

class RenoirClient : public SleepyDiscord::DiscordClient {
	public:
		using SleepyDiscord::DiscordClient::DiscordClient;

		void onMessage(SleepyDiscord::Message message) override {
			SleepyDiscord::Embed embed_help{};
			embed_help.color = 120;
			embed_help.title = "Helping a newbie!";
			embed_help.description = 
				"**Commands**\n\n"
				"**"+prefix+" setPrefix:** Set new prefix\n"
				"**"+prefix+" help:** Plot this\n"
				"**"+prefix+" mention:** Mention user\n"
				"**"+prefix+" sex:** Sexy quotes\n"
				"**"+prefix+" star:** Send message to Server's Starboard\n"
				"**"+prefix+" setStar:** Set currently channel as Starboard\n";

			if ((message.startsWith(prefix + " help"))) {
				sendMessage(message.channelID, "<@" + message.author.ID + ">", embed_help);
			}

			if (message.content == prefix + " setPrefix") {
				embed_help.description = "yo setPrefix *prefix*";
				sendMessage(message.channelID, "<@" + message.author.ID + ">", embed_help);	
			} 
			else if (message.startsWith(prefix + " setPrefix") && message.content.length() > prefix.length()+10) {
				prefix = std::regex_replace(message.content, std::regex(prefix + " setPrefix "), "");
				sendMessage(message.channelID,
						"**"+prefix+"** is the new prefix\n\n **"+prefix+" help** to show all commands!");
			}
			
			if (message.startsWith(prefix + " mention")) {
				if (message.startsWith(prefix + " mention <@")) {
					std::string userID = std::regex_replace(message.content, std::regex(prefix + " mention <@!"), "");
					userID.pop_back();
					sendMessage(message.channelID, "<@"+userID+">");
				} else 
					sendMessage(message.channelID, "<@" + message.author.ID + ">");
			}

			if (message.startsWith(prefix + " sex")) {
				sendMessage(message.channelID, bad_words[rand() % bad_words.size()]);
			}

			if (message.startsWith(prefix + " setStar")) {
				if (message.startsWith(prefix + " setStar <#")) {
					std::string my_channelID = std::regex_replace(message.content, std::regex(prefix + " setStar <#"), "");
					my_channelID.pop_back();
					starboard.ID = my_channelID;	
					sendMessage(message.channelID, "<#" + my_channelID + "> configured as STARBOARD!");
				} 
				else {
					starboard.ID = message.channelID;
					sendMessage(starboard.ID, "Now this channel is a STARBOARD!");
				}
			} 

			if (message.startsWith(prefix + " star") && !starboard.empty()) {

				if (!message.messageReference.empty()) {
					SleepyDiscord::Message replied_msg;
					SleepyDiscord::Embed emb_star{};

					replied_msg = *message.referencedMessage.get();
					emb_star.timestamp = replied_msg.timestamp;
					emb_star.color = 120;
					emb_star.author.name = replied_msg.author.username;
					emb_star.description = replied_msg.content;
					emb_star.author.iconUrl = 
						"https://cdn.discordapp.com/avatars/" + replied_msg.author.ID +
						"/" + replied_msg.author.avatar + ".png";

					if (!replied_msg.attachments.empty()) {
						SleepyDiscord::EmbedImage image;
						image.url = replied_msg.attachments[0].url;
						emb_star.image = image;
					}

					sendMessage(starboard.ID, "", emb_star);
				} 
				else {
					SleepyDiscord::Embed emb_selfstar{};
					std::string new_content = std::regex_replace(message.content, std::regex(prefix + " star "), "");

					emb_selfstar.timestamp = message.timestamp;
					emb_selfstar.color = 120;
					emb_selfstar.author.name = message.author.username;
					emb_selfstar.description = new_content;
					emb_selfstar.author.iconUrl =
						"https://cdn.discordapp.com/avatars/" + message.author.ID +
						"/" + message.author.avatar + ".png";

					if (!message.attachments.empty()) {
						SleepyDiscord::EmbedImage image;
						image.url = message.attachments[0].url;
						emb_selfstar.image = image;
					}

					sendMessage(starboard.ID, "", emb_selfstar);
				}
			} 
		}

	private:
		std::vector<std::string> bad_words 
		{
			"No sex.",
			"SEEEXXX",
			"Why?",
			"Are you sure?",
			"No, respect me.",
			"Ok my son",
			"Only after marrige.",
			"Nyo, i'm virgin oni-chan >w<",
			"Again? you pussy",
			"Ok, but first lemme grab deez nuts",
			":(",
			"Yes, please",
			"Please, no :("
		};

		SleepyDiscord::Channel starboard;	
		std::string prefix = "yo";
};

int main() {
	srand(time(0));
	RenoirClient client(TOKEN, SleepyDiscord::USER_CONTROLED_THREADS);
	auto intentList = {
		SleepyDiscord::Intent::SERVER_MESSAGES
	};

	client.setIntents(intentList);
	client.run();
}
