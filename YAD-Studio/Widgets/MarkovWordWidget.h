#ifndef MARKOVWORDWIDGET_H
#define MARKOVWORDWIDGET_H

#include <QWidget>

namespace Ui {
class MarkovWordWidget;
}

class MarkovWordWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MarkovWordWidget(QWidget *parent = 0);
    ~MarkovWordWidget();
    
private:
    Ui::MarkovWordWidget *ui;

    QString _word;

    enum HighlightType
    {
        //red
        Before,
        //green
        After
    };
    QString find(QString source);

public slots:
    //changes the text inside. Removes all previously set highlights
    void setWord(const QString& word);
    //sets the highlight to given symbols.
    //from [1,word.size()]
    void addHighlight(int from,
                      int length,
                      HighlightType type);
    //removes previously set highlight
    void removeHighlight();
};

#endif // MARKOVWORDWIDGET_H
