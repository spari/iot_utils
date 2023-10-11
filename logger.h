#ifndef logger_h
#define logger_h

//#define DEBUG_LOG

void log_proxy(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void log_serial(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#ifdef DEBUG_LOG
#define log_debug(fmt, ...) log_proxy("[DEBUG] " fmt, ##__VA_ARGS__)
#else
#define log_debug(fmt, ...)
#endif

/*
 * Logging functions. 
 */
#define log_info(fmt, ...) log_proxy(fmt, ##__VA_ARGS__)
/*
 * Since C++11, the space between the string literal "xxx" and fmt is required 
 * to avoid the error: 
 *      unable to find string literal operator 'operator""fmt'
 * See: https://en.cppreference.com/w/cpp/language/user_literal
 */
#define log_warn(fmt, ...) log_proxy("[WARN] " fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) log_proxy("[ERROR] " fmt, ##__VA_ARGS__)
#define log_serialln(fmt, ...) log_serial(fmt"\r\n", ##__VA_ARGS__)

#endif
