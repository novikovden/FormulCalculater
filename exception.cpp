#include "exception.h"
#include <string>

Exception::Exception() {}
Exception::~Exception() {}

ExplanatoryException::ExplanatoryException(std::string const &what, std::string const &explanatory, int pos):
    _what(what), _explanatory(explanatory), _position(pos) {}
ExplanatoryException::ExplanatoryException(std::string const &what, std::string const &explanatory, std::string::const_iterator it):
    _what(what), _explanatory(explanatory), _position(0) {
    for(auto b = explanatory.begin(), e = explanatory.end(); b != e; ++b) {
        if(b == it)
            break;
        ++_position;
    }
}
ExplanatoryException::~ExplanatoryException() {}

std::string ExplanatoryException::what() const {
    return _what;
}
std::string ExplanatoryException::explanatory() const {
    return _explanatory;
}
int ExplanatoryException::position() const {
    return _position;
}
std::string ExplanatoryException::whole() const {
    std::string result = _what + "\n" + _explanatory + "\n";
    for(int i = 0; i < _position; ++i) {
        result += " ";
    }
    result += "^";
    return result;
}


ExceptionOneMessage::ExceptionOneMessage(std::string const &message): _message(message) {}
ExceptionOneMessage::~ExceptionOneMessage() {}

std::string ExceptionOneMessage::what() const {
    return _message;
}

// END
