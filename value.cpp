#include "value.h"

Value::Value() {}
Value::~Value() {}
Value* Value::inversion() {
    return new InversionValue(this);
}



InversionValue::InversionValue(): _value(nullptr) {}
InversionValue::InversionValue(Value *value): _value(value) {}
InversionValue::~InversionValue() {
    delete _value;
}

double InversionValue::calculate() const {
    return -_value->calculate();
}
std::ostream& InversionValue::print(std::ostream &os, std::string const &tab) const {
    return _value->print(os << "-", tab);
}
InversionValue* InversionValue::clone() const {
    return new InversionValue(_value->clone());
}
Value* InversionValue::inversion() {
    return _value;
}




// END
