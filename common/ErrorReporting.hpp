#ifndef COMMON_ERROR_REPORTING_H
#define COMMON_ERROR_REPORTING_H

/**
 * Reports a non-critical error about an unexpected condition.
 */
#define NCR_UNEXPECTED(msg)

/**
 * Checks and reports a function contract violation
 */
#define USE_CONTRACT_CHECK(condition)

/**
 * Throws a system_error exception
 */
#define THROW_SYSTEM_ERROR(msg)


#if ! defined(AVR)

#include <stdio.h>
#include <stdlib.h>
#include <system_error>

#undef NCR_UNEXPECTED
#undef USE_CONTRACT_CHECK
#undef THROW_SYSTEM_ERROR

#define NCR_UNEXPECTED(msg) \
	do { \
		::fprintf( stderr, "Non-critical error at %s (%d): %s\n", \
				   __FILE__, __LINE__, std::string( msg ).c_str() ); \
	} while ( false )


#define USE_CONTRACT_CHECK(condition) \
	do { \
		if ( ! (condition) ) { \
			::fprintf( stderr, "Use contract violation at %s (%d): %s\n", \
				   __FILE__, __LINE__, #condition ); \
			::abort(); \
		} \
	} while ( false )


#define THROW_SYSTEM_ERROR(msg) \
	throw std::system_error( \
		errno, \
		std::generic_category(), \
		__FILE__ " (" + std::to_string(__LINE__) + "): " + msg \
	)

#endif // ! defined(AVR)


#endif
