#include <stdio.h>
#include "logger.hpp"
#include <chrono>

void test_func (log_message msg, int sec) {
	this_thread::sleep_for(chrono::seconds(sec));
	LOGGER_WRITE(msg);
}

int main() {

	//Logging level - error, Logging file - logg_file.txt
	LOGGER_INIT(ERROR, "logg_file.txt");

	//Log messages and threads creation
	log_message msg_info = {"very informative message", INFO};
	thread t1(test_func, msg_info, 1);

	log_message msg_warn = {"some debug info", WARN};
	thread t2(test_func, msg_warn, 2);

	log_message msg_error = {"information about error", ERROR};
	thread t3(test_func, msg_error, 3);

	log_message msg_fatal = {"fatality", FATAL};
	thread t4(test_func, msg_fatal, 4);
	
	t1.join();
	t2.join();
	t4.join();
	t4.join();

	//Stop work of logger
	LOGGER_DEINIT();

	return 0;
}

