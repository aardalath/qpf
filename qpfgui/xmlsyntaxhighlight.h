#ifndef BASIC_XML_SYNTAX_HIGHLIGHTER_H
#define BASIC_XML_SYNTAX_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextEdit>

class XMLBasicSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    XMLBasicSyntaxHighlighter(QObject * parent);
    XMLBasicSyntaxHighlighter(QTextDocument * parent);
    XMLBasicSyntaxHighlighter(QTextEdit * parent);

protected:
    virtual void highlightBlock(const QString & text);

private:
    void highlightByRegex(const QTextCharFormat & format,
                          const QRegExp & regex, const QString & text);

    void setRegexes();
    void setFormats();

private:
    QTextCharFormat     m_xmlKeywordFormat;
    QTextCharFormat     m_xmlElementFormat;
    QTextCharFormat     m_xmlAttributeFormat;
    QTextCharFormat     m_xmlValueFormat;
    QTextCharFormat     m_xmlCommentFormat;

    QList<QRegExp>      m_xmlKeywordRegexes;
    QRegExp             m_xmlElementRegex;
    QRegExp             m_xmlAttributeRegex;
    QRegExp             m_xmlValueRegex;
    QRegExp             m_xmlCommentRegex;
};

#endif // BASIC_XML_SYNTAX_HIGHLIGHTER_H
