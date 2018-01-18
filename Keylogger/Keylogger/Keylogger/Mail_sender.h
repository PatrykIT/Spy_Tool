#pragma once

// This file is not included in Github etc. You need to make it yourself with your own credentials.
#include "Credentials\Credentials.h" 

#include <string>

class EMail_Sender
{
private:
	std::string mail_from = credentials::mail_from;
	std::string mail_to;
	std::string password = credentials::password;
	std::string authentication = credentials::authentication;
	std::string smtp_server = credentials::smtp_server;
	std::string command_to_send_mail;

	int email_send_timer; // In minutes
	EMail_Sender(int timer);

public:
	EMail_Sender(std::string &&_mail_to, int timer);
	EMail_Sender(const std::string &_mail_to, int timer);

	void send_email(const std::string &attachment_path);
};
