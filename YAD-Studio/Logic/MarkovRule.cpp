#include "MarkovRule.h"

MarkovRule::MarkovRule(const QString& left_patr,
                       const QString& right_part,
                       bool is_final,
                       int line_number):
    _pattern(left_patr),
    _substitution(right_part),
    _is_final_rule(is_final),
    _line_number(line_number)
{
}
bool MarkovRule::isFinalRule()
{
    return _is_final_rule;
}

const QString& MarkovRule::getLeftPart()
{
    return _pattern;
}

const QString& MarkovRule::getRightPart()
{
    return _substitution;
}

int MarkovRule::getLineNumber()
{
    return _line_number;
}

void MarkovRule::setFinalRule(bool is_final)
{
    _is_final_rule = is_final;
}

void MarkovRule::setLeftPart(const QString& str)
{
    _pattern = str;
}

void MarkovRule::setRightPart(const QString& str)
{
    _substitution = str;
}

void MarkovRule::setLineNumber(int line_number)
{
    _line_number = line_number;
}
