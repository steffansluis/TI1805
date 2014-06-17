
#include "Testing.h"


//////////// TestException ///////////////////

TestException::TestException(const char* message)
{
	this->_message = message;
}


const char* TestException::what() const throw()
{
	return this->_message;
}







