#ifndef MARKOVEDITORWIDGET_H
#define MARKOVEDITORWIDGET_H

#include <QPlainTextEdit>
#include "Logic/CompilerError.h"
#include "Logic/MarkovAlgorithm.h"

class LineNumberArea;


class MarkovEditorWidget : public QPlainTextEdit
{
    Q_OBJECT
    
public:
    explicit MarkovEditorWidget(QWidget *parent = 0);
    ~MarkovEditorWidget();
public slots:

    /**
     * @brief when source code from file is loaded this slot will be called.
     * Source code should be put to QTextEditor.
     * @param source_code
     * @param user_change if true then change in source will be treated like user has changet id
     */
    void newSourceCode(QString source_code, bool system_change=true);

    /**
     * @brief the text which is selected should be deleted.
     */
    void deleteSelection();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    /**
     * @brief if any changes where done to source code
     * @param source_code
     */
    void sourceCodeChanged(QString source_code);

    /**
     * @brief if algorithm was updated
     */
    void markovAlgorithmChanged(MarkovAlgorithm algorithm);

    /**
     * @brief  when _can_run changes.
     */
    void canRun(bool);
private:


    void setCanRun(bool new_value);

    /**
     * @brief list of errors which are in source code
     */
    QVector<CompilerError> _errors;
    /**
     * @brief last successfully parsed algorithm
     */
    MarkovAlgorithm _algorithm;

    /**
     * @brief list of active break points
     */
    QVector<int> _breakpoints;

    /**
     * @brief if the are no errors it should be true and false otherwise.
     * When changed canRun should be emitted.
     * MUST BE CHANGED ONLY BY setCanRun(bool)
     */
    bool _can_run;

    QString _last_source_code;

    QWidget *lineNumberArea;
    QPair<int, int> m_countCache;
    QFont _line_number_font;
    QFontMetrics _line_number_metrics;
    int _current_line;
private slots:
    void userChangedSourceCode();
    void updateErrors();

    void updateLineNumberAreaWidth();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(MarkovEditorWidget *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    MarkovEditorWidget *codeEditor;
};

#endif // MARKOVEDITORWIDGET_H
