#include <qscilexerglsl.h>
#include <Qsci/qsciscintilla.h>

#include <iostream>
#include <sstream>
#include "glslLexer.h"

QsciLexerGLSL::QsciLexerGLSL(QsciScintilla *parent) : QsciLexerCustom(parent)
{
    m_parent = parent;
    m_API = new QsciAPIs(this);
    QString apiPath("./src/glsl.api");
    const QString a(" ");
    setAPIs(m_API);
    m_API->load(apiPath);
    m_API->prepare();
    m_parent->setAutoCompletionThreshold(2);
    m_parent->setAutoCompletionFillupsEnabled(true);
    m_parent->setAutoCompletionSource(QsciScintilla::AcsAPIs);
}

QsciLexerGLSL::~QsciLexerGLSL()
{
    return;
}

void QsciLexerGLSL::styleText(int start, int end)
{
    m_parent->autoCompleteFromAPIs();
    //return if no QsciScintilla editor
    if(!editor())
        return;

    char * data = new char[end - start +1];

    //get text to be styled
    editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, data);
    QString source(data);
    delete [] data;

    //return if no text to be styled
    if(source.isEmpty())
        return;

    //style keywords
    highlightKeywords(source, start);
}

void QsciLexerGLSL::highlightKeywords(const QString &source, int start)
{
    FlexLexer* lexer = new yyFlexLexer;
    int tok;
    int loc=0;
    std::istringstream istr (source.toStdString());

    std::vector<int> tokens;
    tok = lexer->yylex(&istr);
    while (tok>0)
    {
        tokens.push_back(tok);
        tokens.push_back(loc);
        tokens.push_back(lexer->YYLeng());
        loc = loc + lexer->YYLeng();
        QString s(lexer->YYText());
        tok = lexer->yylex();
    }

    for (int i = 0; i<tokens.size(); i=i+3)
    {
        switch(tokens[i])
        {
        case StyleType::DEFAULT:
            startStyling(start+tokens[i+1]);
            setStyling(tokens[i+2],StyleType::DEFAULT);
        break;
        case StyleType::KEYWORD:
            startStyling(start+tokens[i+1]);
            setStyling(tokens[i+2],StyleType::KEYWORD);
        break;
        case StyleType::DATATYPE:
            startStyling(start+tokens[i+1]);
            setStyling(tokens[i+2],StyleType::DATATYPE);
        break;
        case StyleType::NUMBER:
            startStyling(start+tokens[i+1]);
            setStyling(tokens[i+2],StyleType::NUMBER);
        break;
        case StyleType::OPERATOR:
            startStyling(start+tokens[i+1]);
            setStyling(tokens[i+2],StyleType::OPERATOR);
        break;
        case StyleType::STRING:
            startStyling(start+tokens[i+1]);
            setStyling(tokens[i+2],StyleType::STRING);
        break;
        case StyleType::FUNCTION:
            startStyling(start+tokens[i+1]);
            setStyling(tokens[i+2]-1,StyleType::FUNCTION);
        break;
        case StyleType::COMMENT:
            startStyling(start+tokens[i+1]);
            setStyling(tokens[i+2],StyleType::COMMENT);
        break;
        }
    }
}

QColor QsciLexerGLSL::defaultColor(int style) const
{
    switch(style)
    {
    case StyleType::DEFAULT:
        return QColor(247, 247, 241);
    case StyleType::KEYWORD:
        return QColor(249, 38, 114);
    case StyleType::DATATYPE:
        return QColor(102, 216, 238);
    case StyleType::NUMBER:
        return QColor(174, 129, 255);
    case StyleType::OPERATOR:
        return QColor(249, 38, 114);
    case StyleType::STRING:
        return QColor(230, 219, 116);
    case StyleType::FUNCTION:
        return QColor(166, 226, 46);
    case StyleType::COMMENT:
        return QColor(117, 113, 94);
    }
    return QColor(247,247,241);
}

QColor QsciLexerGLSL::defaultPaper(int style) const
{
    return QColor(39,40,34);
}

QFont QsciLexerGLSL::defaultFont(int style) const
{
    int weight = 50;
    int size = 12;
    bool italic = 0;
    switch(style)
    {
    case StyleType::DEFAULT:
        weight = 50;
        size = 12;
    break;
    case StyleType::DATATYPE:
        italic = true;
    break;
    }

    return QFont("Monospace", size,weight,italic);
}

QString QsciLexerGLSL::description(int style) const
{
    switch(style)
    {
    case StyleType::DEFAULT:
        return "Default";
    case StyleType::KEYWORD:
        return "Keyword";
    }
    return QString(style);
}

const char *QsciLexerGLSL::language() const
{
    return "GLSL";
}
