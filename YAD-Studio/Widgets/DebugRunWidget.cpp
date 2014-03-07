#include "DebugRunWidget.h"
#include "ui_DebugRunWidget.h"


DebugRunWidget::DebugRunWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugRunWidget)
{
    ui->setupUi(this);

    debugStarted("");
}

DebugRunWidget::~DebugRunWidget()
{
    delete ui;
}
void DebugRunWidget::debugStarted(QString input_word)
{
    ui->inputWidget->setWord(input_word);

    ui->output->setVisible(false);
    ui->outputWidget->setVisible(false);

    ui->error->setVisible(false);
    ui->errorDescription->setVisible(false);

    ui->rule->setVisible(false);
    ui->ruleText->setVisible(false);

    ui->steps->setVisible(false);
    ui->stepsNumber->setVisible(false);

    ui->stopButton->setVisible(false);
    ui->continueButton->setVisible(false);
    ui->nextButton->setVisible(false);
}

void DebugRunWidget::debugSuccess(QString input_word,
             QString output_word,
             int steps_made)
{
    ui->inputWidget->setWord(input_word);

    ui->output->setVisible(true);

    ui->outputWidget->setVisible(true);
    ui->outputWidget->setWord(output_word);

    ui->error->setVisible(false);
    ui->errorDescription->setVisible(false);

    ui->rule->setVisible(false);
    ui->ruleText->setVisible(false);


    ui->steps->setVisible(true);
    ui->stepsNumber->setVisible(true);
    ui->stepsNumber->setText(QString::number(steps_made));

    ui->stopButton->setVisible(false);
    ui->continueButton->setVisible(false);
    ui->nextButton->setVisible(false);
}

void DebugRunWidget::debugFailed(QString input_word,
            RunError error,
            int steps_made)
{
    ui->inputWidget->setWord(input_word);

    ui->error->setVisible(true);
    ui->errorDescription->setVisible(true);

    ui->output->setVisible(true);
    ui->outputWidget->setVisible(false);

    ui->steps->setVisible(true);
    ui->stepsNumber->setVisible(true);
    ui->stepsNumber->setText(QString::number(steps_made));

    ui->errorDescription->setText(error.getFullErrorInfo());

    QPalette palette = ui->error->palette();
    palette.setColor(ui->error->foregroundRole(), Qt::red);
    ui->error->setPalette(palette);

    QPalette palette2 = ui->errorDescription->palette();
    palette2.setColor(ui->errorDescription->foregroundRole(), Qt::red);
    ui->errorDescription->setPalette(palette2);

    ui->stopButton->setVisible(false);
    ui->continueButton->setVisible(false);
    ui->nextButton->setVisible(false);
}

void DebugRunWidget::debugStepFinished(int step_number,
                  QString before_rule_applied,
                  QString after_rule_applied,
                  MarkovRule applied_rule)
{

}

void DebugRunWidget::breakPointReached(int line_number)
{

}
