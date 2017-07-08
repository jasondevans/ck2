#ifndef CK2_UTIL_EXCEPTION_H
#define CK2_UTIL_EXCEPTION_H

#include "ck_common_includes.h"

namespace CipherKick
{
	class UtilException : public std::exception
	{

	private:

		std::string message;

	public:

		UtilException();

		UtilException(const std::string& message);

		UtilException(const std::string& message, const std::exception& cause);

	};
}

#endif // CK2_UTIL_EXCEPTION_H
