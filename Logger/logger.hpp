#ifndef LOGGER
#define LOGGER

#include <string>
#include <fstream>
#include <iostream>
#include <execinfo.h>
#include <thread>
#include <mutex>
#include <cstdlib>

#define BUFF_SIZE 1024
#define CALL_STACK_SIZE 256
#define DEFAULT_LOGG_FILE "logg_file.txt"

#define LOGGER_INIT(log_lvl, file_name) Logger::logger_init(log_lvl, file_name)
#define LOGGER_WRITE(msg) Logger::logger_write_message(msg)
#define LOGGER_DEINIT() Logger::logger_deinit()
 
using namespace std;

typedef enum Condition {
	INFO,
	WARN,
	ERROR,
	FATAL, 
	NUMBER_OF_OPTIONS
} log_option;

typedef struct FIFO {
	unsigned int tail;
	unsigned int head;
	string buffer[BUFF_SIZE];
} circular_buff;

typedef struct Message {
	string error_description;
	log_option option;
} log_message;

const string log_option_message[NUMBER_OF_OPTIONS] =
										{"INFO: ", "WARNING: ", "ERROR: ", "FATAL: "};
//Default log message
log_message default_log_message = {"", INFO};

class Logger {
	//Circular buffer 
	static circular_buff cir_buff;
	//File stream
	static ofstream file_out_stream;
	//Defaul logging level
	static log_option default_log_level;
	//File for write name 
	static string file_for_write;
public:
	//Initialization func
	static bool logger_init(log_option default_log_level, string file_name = DEFAULT_LOGG_FILE);
	//This function write the message to the buffer, when the buffer becomes full
	//it is written to the file
	static bool logger_write_message(log_message message = default_log_message);
	//Check for completeness
	static bool is_buff_non_empty();
	//Deinitialization func
	static bool logger_deinit();
};

//static variables init
ofstream Logger::file_out_stream;
circular_buff Logger::cir_buff;
void *backtrace_buffer[CALL_STACK_SIZE];
log_option Logger::default_log_level = INFO;
string Logger::file_for_write = DEFAULT_LOGG_FILE;

//mutex init
mutex buff_locker;

#ifdef DEBUG
bool Logger::logger_init(log_option log_lvl, string file_name) {
	try {
		file_for_write = file_name;
		default_log_level = log_lvl;
		//initialize file stream and buffer
		file_out_stream.open(file_for_write, ios::app);

		for(int i = 0; i < BUFF_SIZE; i++) {
			cir_buff.buffer[i] = "";
		}
		cir_buff.head = 0;
		cir_buff.tail = BUFF_SIZE - 1;
		return true;
	} catch(...) {
		cerr << "Error in initialization of logger " << endl;
		return false;
	}
}

bool Logger::logger_deinit() {
	try {
		//close file stream
		file_out_stream.close();
		return true;
	} catch(...) {
		cerr << "Error in deinitialization of logger " << endl;
		return false;
	}
}

bool Logger::is_buff_non_empty() {
	return (cir_buff.tail == cir_buff.head);
}

bool Logger::logger_write_message(log_message message) {
	try {
		if(!is_buff_non_empty() && message.option != FATAL &&  message.option >= default_log_level) {
			buff_locker.lock();
			//write message to buffer
			cir_buff.buffer[cir_buff.head] = log_option_message[message.option] + message.error_description;
			cir_buff.head += 1;
			buff_locker.unlock();
			return true;
		} else if(!is_buff_non_empty() && message.option == FATAL) {
			//receive the call stack of program
			char **strings;
			int nptrs = backtrace(backtrace_buffer, CALL_STACK_SIZE);
			strings = backtrace_symbols(backtrace_buffer, nptrs);

			buff_locker.lock();
			//Before the fall, write all info from the buffer to the file.
			for (int i = 0; i < cir_buff.head; i++) {
				file_out_stream << cir_buff.buffer[i] << endl;
			}

			//write the call stack to file
			file_out_stream << log_option_message[message.option] + message.error_description << endl;
			for (int j = 0; j < nptrs; j++) {
				file_out_stream << strings[j] << endl;
			}
            buff_locker.unlock();

            cerr << "There is fatal error! Program termination." << endl;

            //It is fatal error, so we exit with failure
            exit(EXIT_FAILURE);
		} else if (message.option >= default_log_level){
			buff_locker.lock();
			//if the circular buffer is full, write all information to file
			//and clear the buffer
			for(int i = 0; i < BUFF_SIZE; i++ ) {
				file_out_stream << cir_buff.buffer[i] << endl;
				cir_buff.buffer[i] = "";
			}
			cir_buff.head = 0;
			buff_locker.unlock();
			//when buffer clear, write message
			logger_write_message(message);
			return true;
		} else {
			return true;
		}
	} catch(...) {
		cerr << "Error in logger write message" << endl;
		return false;
	}
}
#endif

#ifndef DEBUG 
//for performance don't do anything
bool Logger::logger_init() {
	return true;
}

bool Logger::logger_deinit() {
	return true;
}

bool Logger::is_buff_empty() {
	return true;
}

bool Logger::logger_write_message(log_message message) {
	return true;
}
#endif

#endif