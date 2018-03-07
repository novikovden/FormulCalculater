#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception {
public:
    Exception();
    virtual ~Exception();

    virtual std::string what() const = 0;
};

class ExplanatoryException {
public:
    ExplanatoryException(std::string const &what, std::string const &explanatory, int pos);
    ExplanatoryException(std::string const &what, std::string const &explanatory, std::string::const_iterator it);
    virtual ~ExplanatoryException();

    virtual std::string what() const;
    std::string explanatory() const;
    int position() const;
    std::string whole() const;
private:
    std::string _what;
    std::string _explanatory;
    int _position;
};

class ExceptionOneMessage: public Exception {
public:
    ExceptionOneMessage(std::string const &message);
    virtual ~ExceptionOneMessage();

    virtual std::string what() const override;
private:
    std::string _message;
};

#endif // EXCEPTION_H
