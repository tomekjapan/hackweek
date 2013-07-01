#ifndef COMMON_ERROR_REPORTING_H
#define COMMON_ERROR_REPORTING_H

#include <stdio.h>
#include <system_error>

/**
 * Report a non-critical error about an unexpected condition.
 */
#define NCR_UNEXPECTED(msg) do { \
	::fprintf( stderr, "Non-critical error at %s (%d): %s\n", \
			   __FILE__, __LINE__, msg ); \
	} while ( false )

/**
 * Throw a system_error exception
 */
#define THROW_SYSTEM_ERROR(msg) \
	throw std::system_error( \
		errno, \
		std::generic_category(), \
		__FILE__ " (" + std::to_string(__LINE__) + "): " + msg \
	)

#endif
