#include <QHBoxLayout>
#include <QToolButton>
#include <QFileDialog>
#include <QFocusEvent>

#include "QtFilePathEditor.h"


QtFilePathEditor::QtFilePathEditor(QWidget *parent)
    : QWidget(parent), 
    m_openType(FPT_FILE_OPEN)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    QToolButton *button = new QToolButton(this);
    button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    button->setText(QLatin1String("..."));
    layout->addWidget(m_lineEdit);
    layout->addWidget(button);
    setFocusProxy(m_lineEdit);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_InputMethodEnabled);
       
    connect(m_lineEdit, SIGNAL(textEdited(const QString &)),
                this, SIGNAL(filePathChanged(const QString &)));
    connect(button, SIGNAL(clicked()),
                this, SLOT(buttonClicked()));
}

void QtFilePathEditor::setFilePath(const QString &filePath)
{
    if (m_lineEdit->text() != filePath)
        m_lineEdit->setText(filePath);
}

QString QtFilePathEditor::filePath() const
{
    return m_lineEdit->text();
}

void QtFilePathEditor::setFilter(const QString &filter)
{
    m_filter = filter;
}

QString QtFilePathEditor::filter() const
{
    return m_filter;
}

void QtFilePathEditor::setDefaultPath(const QString &filePath)
{
    m_defPath = filePath;
}

void QtFilePathEditor::setOpenType(FilePathType t)
{
    m_openType = t;
}

// slots

void QtFilePathEditor::buttonClicked()
{
	// file name only
	bool isFileName = (m_openType != FPT_DIRECTORY);

    QString path = m_lineEdit->text();
	if (path.isEmpty())
		path = m_defPath + "/.";
	else		
	if (isFileName){
		QFileInfo fi(path);
		if (fi.isRelative() && m_defPath.size()){
			path = m_defPath + "/" + fi.fileName();
		}
	}
	
    QString filePath;
    
    switch (m_openType)
    {
		case FPT_FILE_SAVE:
            filePath = QFileDialog::getSaveFileName(this, tr("Choose a file to save"), path, m_filter);
			break;
			
		case FPT_DIRECTORY:
			filePath = QFileDialog::getExistingDirectory(this, tr("Choose a directory"), path);
			break;

        default:
        case FPT_FILE_OPEN:
            filePath = QFileDialog::getOpenFileName(this, tr("Choose a file to open"), path, m_filter);
            break;
    }

    if (filePath.isNull())
        return;
        
    m_lineEdit->setText(filePath);
    Q_EMIT filePathChanged(filePath);
}

// events

void QtFilePathEditor::focusInEvent(QFocusEvent *e)
{
    m_lineEdit->event(e);
    if (e->reason() == Qt::TabFocusReason || e->reason() == Qt::BacktabFocusReason) {
        m_lineEdit->selectAll();
    }
    QWidget::focusInEvent(e);
}

void QtFilePathEditor::focusOutEvent(QFocusEvent *e)
{
    m_lineEdit->event(e);
    QWidget::focusOutEvent(e);
}

void QtFilePathEditor::keyPressEvent(QKeyEvent *e)
{
    m_lineEdit->event(e);
}

void QtFilePathEditor::keyReleaseEvent(QKeyEvent *e)
{
    m_lineEdit->event(e);
}
