

#include <exception>



class TestException: public std::exception
{
private:
	const char* _message;
public:
	
	/**
	* Constructor
	*/
	TestException(const char* message);

	virtual const char* what() const throw();
};