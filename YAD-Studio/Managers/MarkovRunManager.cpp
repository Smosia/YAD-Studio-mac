#include "MarkovRunManager.h"
#include <QCoreApplication>
#include <QTime>

MarkovRunManager* MarkovRunManager::_instance = nullptr;

MarkovRunManager* MarkovRunManager::getInstance( )
{
    if (_instance == nullptr)
        _instance = new MarkovRunManager();
    return _instance;
}

MarkovRunManager::MarkovRunManager():
    _input_word(""),
    _steps_made(0),
    _word_after_last_step(""),
    _is_debug_mode(false),
    _stop_on_next_step(false),
    _terminate_on_next_step(false)
{
}

RunError notInAlphabet(QString letter, MarkovAlphabet alphabet_m)
{
    QString old_alphabet = QObject::tr("%1").arg(letter);
    QSet<QChar> alphabet = alphabet_m.getAlphabet();
    foreach (const QChar value, alphabet)
    {
        old_alphabet += QObject::tr(", %1").arg(value);
    }

    QString title=QObject::tr("Symbol '%1' is not in alphabet").arg(letter);
    QString description=QObject::tr("You can add it to alphabet. Examples: 'T = {%1}'.").arg(old_alphabet);

    return RunError(title, description, 305);
}

int MarkovRunManager::getStepNumberOfValue(QString word)
{
    //    QSet<StepResult>::iterator i;
    //    for (i = _steps_history.begin(); i != _steps_history.end(); ++i)
    //    {
    //        StepResult curr = *i;
    //        if(curr._output == word)
    //            return curr._step_id;
    //    }

    QSet<StepResult>::iterator it = qFind(_steps_history.begin(),
                                          _steps_history.end(),
                                          StepResult(word,0));
    if (it != _steps_history.end())
        return (*it)._step_id;
    return -1;
}
bool MarkovRunManager::choseAndUseRule(QString& word,
                                       MarkovRule& rule)
{
    //get rule
    const MarkovRule*  markov_rule = _algorithm.getRuleFor(word);
    QString res="";

    //use rule
    if(markov_rule != nullptr)
    {
        rule = *markov_rule;

        res = _algorithm.useRule(word,rule);
        word = res;
        return true;
    }
    return false;
}

bool MarkovRunManager::findAndApplyNextRule()
{
    if(_terminate_on_next_step)
    {
        _terminate_on_next_step = false;
        debugStop();
        QCoreApplication::processEvents();
        return false;
    }

    if(_word_after_last_step.size()>2000)
    {
        QString description = tr("Result can not be longer than 2000 symbols. On step #%1 input become %2 symbols long")
                .arg(_steps_made).arg(_word_after_last_step.size());
        RunError error(tr("Too long result"),
                       description,
                       101);

        if(_is_debug_mode)
        {
            emit debugFinishFail(_input_word,error,_steps_made);
            QCoreApplication::processEvents();
        }
        else
        {
            emit runWithoutDebugFinishFail(_input_word,error,_steps_made);
            QCoreApplication::processEvents();
        }
        return false;
    }

    //Go through MarkovRules and select which fits.
    //If there are no rules then emit debugFinishSuccess or
    //runWithoutDebugFinishSuccess depending on run mode.
    //Use as _word_after_last_step as output word.

    QString word = _word_after_last_step;
    MarkovRule rule;
    if(!choseAndUseRule(word, rule))
    {
        //there are no rules
        if(_is_debug_mode)
        {
            emit debugFinishSuccess(_input_word,
                                    word,
                                    _steps_made);
            QCoreApplication::processEvents();
        }
        else
        {
            emit runWithoutDebugFinishSuccess(_input_word,
                                              word,
                                              _steps_made);
            QCoreApplication::processEvents();
        }
        return false;
    }

    ++_steps_made;



    if(_is_debug_mode)
    {
        emit debugStepFinished(_steps_made,
                               _word_after_last_step,
                               word,
                               rule);
        QCoreApplication::processEvents();
    }

    _word_after_last_step = word;

    if (_steps_history.contains(StepResult(_word_after_last_step,0)))
    {
        QString last_step = _word_after_last_step;
        if(last_step.size() > 30)
            last_step = last_step.mid(0, 30) + "...";

        int prev_same_stem = getStepNumberOfValue(_word_after_last_step);

        QString description = tr("The result of step #%1 is same as on the step #%2\n('%3')")
                .arg(prev_same_stem).arg(_steps_made).arg(last_step);
        RunError error("Algorithm never terminates",
                       description,
                       102);

        if(_is_debug_mode)
        {
            emit debugFinishFail(_input_word,error,_steps_made);
            QCoreApplication::processEvents();
        }
        else
        {
            emit runWithoutDebugFinishFail(_input_word,error,_steps_made);
            QCoreApplication::processEvents();
        }

        return false;
    }

    _steps_history.insert(StepResult(_word_after_last_step,_steps_made));




    //If rule is final then emit debugFinishSuccess or
    //runWithoutDebugFinishSuccess depending on run mode.
    //Use as _word_after_last_step as output word. return false.

    if(rule.isFinalRule())
    {
        if(_is_debug_mode)
        {
            emit debugFinishSuccess(_input_word,
                                    _word_after_last_step,
                                    _steps_made);
            QCoreApplication::processEvents();
        }
        else
        {
            emit runWithoutDebugFinishSuccess(_input_word,
                                              _word_after_last_step,
                                              _steps_made);
            QCoreApplication::processEvents();
        }
        return false;
    }
    else
    {
        if(_is_debug_mode)
        {
            if(_stop_on_next_step ||
                    _break_points.contains(rule.getLineNumber()))
            {
                _stop_on_next_step = false;
                emit debugBreakPointReached(rule.getLineNumber());
                QCoreApplication::processEvents();
                return false;
            }
        }
    }

    return true;
}

void MarkovRunManager::setAlgorithm(MarkovAlgorithm algorithm)
{
    _algorithm = algorithm;
}

void MarkovRunManager::setCanRunSourceCode(bool can)
{
    emit canRunSourceCode(can);
}

bool hasSymbolsNotInAlphabet(QString input_word,
                             MarkovAlphabet alphabet,
                             QChar& symbol)
{
    for(int i=0; i<input_word.size(); ++i)
    {
        QChar ch = input_word[i];
        if(!alphabet.isInAlphabet(ch))
        {
            symbol = ch;
            return true;
        }
    }
    return false;
}

void MarkovRunManager::runWithoutDebug(QString input_word)
{
    _stop_on_next_step = false;
    _terminate_on_next_step = false;
    _steps_made = 0;
    _steps_history.clear();
    _input_word = input_word;
    _word_after_last_step = input_word;
    _is_debug_mode = false;

    emit runWithoutDebugStarted(input_word);
    QCoreApplication::processEvents();

    QChar test;
    if(hasSymbolsNotInAlphabet(input_word,
                               _algorithm.getAlphabet(),
                               test))
    {
        emit runWithoutDebugFinishFail(input_word,
                                       notInAlphabet(test, _algorithm.getAlphabet()),
                                       0);
        QCoreApplication::processEvents();
    }
    else
    {
        QTime time;
        time.start();

        while(findAndApplyNextRule())
        {
            int difference = time.elapsed();
            if(difference > 100)
            {
                emit runStepsMade(_steps_made);
                QCoreApplication::processEvents();
                time.start();
            }
        }
    }
}
void MarkovRunManager::doStartDebug(QString input_word, bool stop_at_first_step)
{
    _stop_on_next_step = stop_at_first_step;
    _terminate_on_next_step = false;
    _steps_made = 0;
    _steps_history.clear();
    _input_word = input_word;
    _word_after_last_step = input_word;
    _is_debug_mode = true;

    emit debugStarted(input_word);
    QCoreApplication::processEvents();

    QChar test;
    if(hasSymbolsNotInAlphabet(input_word,
                               _algorithm.getAlphabet(),
                               test))
    {
        emit debugFinishFail(input_word,
                             notInAlphabet(test, _algorithm.getAlphabet()),
                             0);
        QCoreApplication::processEvents();
    }
    else
    {

        while(findAndApplyNextRule())
        {
        }
    }
}

void MarkovRunManager::runWithDebug(QString input_word)
{
    doStartDebug(input_word, false);
}

void MarkovRunManager::runWithDebugStepByStep(QString input_word)
{
    doStartDebug(input_word, true);
}

void MarkovRunManager::addBreakPoint(int line_number)
{
    _break_points.insert(line_number);
}

void MarkovRunManager::removeBreakPoint(int line_number)
{
    _break_points.remove(line_number);
}

void MarkovRunManager::debugNextStep()
{
    _stop_on_next_step = true;
    findAndApplyNextRule();
}

void MarkovRunManager::debugContinue()
{
    while(findAndApplyNextRule())
    {

    }
}
void MarkovRunManager::terminateRun()
{
    _terminate_on_next_step = true;
}

void MarkovRunManager::debugStop()
{
    if(_is_debug_mode)
    {
        RunError error("Debug stopped","Stoped by the user",103);
        emit debugFinishFail(_input_word,error,_steps_made);
        QCoreApplication::processEvents();
    }
    else
    {
        RunError error("Debug stopped","Stoped by the user",103);
        emit runWithoutDebugFinishFail(_input_word, error,_steps_made);
        QCoreApplication::processEvents();
    }
}
bool operator<(const StepResult& a, const StepResult& b)
{
    return a._output < b._output;
}
bool operator==(const StepResult& a, const StepResult& b)
{
    return a._output == b._output;
}
