// Mailsender.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Mail_sender.h"

#include <windows.h>

#include <iostream>
#include <chrono>
#include <thread>

EMail_Sender::EMail_Sender(int timer)
{
	email_send_timer = timer;
	printf("Maile beda wysylane co: %d minut.\n", timer);
}
EMail_Sender::EMail_Sender(std::string &&_mail_to, int timer) : EMail_Sender(timer)
{
	mail_to = std::move(_mail_to);
}
EMail_Sender::EMail_Sender(const std::string &_mail_to, int timer) : EMail_Sender(timer)
{
	mail_to = _mail_to;
}

void EMail_Sender::send_email(const std::string &attachment_path)
{	
	std::chrono::minutes sleep_duation(email_send_timer); // Initial sleep, so there is some data in a buffer to send.
	std::this_thread::sleep_for(sleep_duation);

	command_to_send_mail = "curl " + smtp_server + " -v --mail-from " + mail_from + " --mail-rcpt  " +
		mail_to + " --ssl -u " + authentication + " -T " + attachment_path + " -k --anyauth";
	
	while (1)
	{
		//TODO: We need to check if curl is installed before executing this command.
		WinExec(command_to_send_mail.c_str(), SW_HIDE);

		std::this_thread::sleep_for(sleep_duation);
	}
}
