#ifndef __BASE_EXCEPTION_H__
#define __BASE_EXCEPTION_H__

#include <string>
#include <exception>
using namespace std;

class Exception
{
public:
	explicit Exception(const char* what);
	explicit Exception(const string& what);
	virtual ~Exception() throw();
	virtual const char* what() const throw();
	const char* stackTrace() const throw();
	
private:
	void fillStackTrace();
	
	string message_;
	string stack_;
};

#endif	// __BASE_EXCEPTION_H__
