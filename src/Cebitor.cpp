﻿#include "Cebitor.h"

#include <iostream>
#include <QAction>
#include <QSettings>
#include <QStringList>
#include <Qsci/qscicommand.h>
#include <Qsci/qscicommandset.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file Cebitor.cpp
/// @brief implementation of Cebitor class
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
Cebitor::Cebitor(QWidget *_parent) : QsciScintilla(_parent)
{
  this->setMinimumHeight(300);

  // Create and assign the lexer
  QsciLexer* lex = new QsciLexerGLSL(this);
  this->setLexer(lex);

  // Set the margin defaults
  this->setMarginType(1,QsciScintilla::MarginType::NumberMargin);
  this->setMarginWidth(1," 012");
  this->setMarginsForegroundColor(QColor(128, 128, 128));
  // Set the caret defaults
  this->setCaretForegroundColor(QColor(247, 247, 241));
  this->setCaretWidth(2);
  // Set the brace defaults
  this->setBraceMatching(QsciScintilla::BraceMatch::SloppyBraceMatch);
  this->setMatchedBraceBackgroundColor(QColor(62, 61, 50));
  this->setUnmatchedBraceBackgroundColor(QColor(249, 38, 114));

  // Enable scroll width tracking and set the scroll width to a low number
  // Scintilla doesn't track line length, so if we wanted automated scrollbar
  // to appear we would need to implement a line length checking
  this->SendScintilla(QsciScintillaBase::SCI_SETSCROLLWIDTHTRACKING, 1);
  this->SendScintilla(QsciScintillaBase::SCI_SETSCROLLWIDTH, 5);

  // unbind CTRL-/ keyboard shortcut
  this->standardCommands()->boundTo(Qt::Key_Slash | Qt::CTRL)->setKey(0);

  // rebind CTRL-/ to comment function
  QAction *commentAction = new QAction(this);
  commentAction->setShortcut(Qt::Key_Slash | Qt::CTRL);

  connect(commentAction, SIGNAL(triggered()), this, SLOT(comment()));
  this->addAction(commentAction);

  // connect signals and slots
  connect(this, SIGNAL(SCN_CHARADDED(int)), this, SLOT(charAdded(int)));
}

//----------------------------------------------------------------------------------------------------------------------
bool Cebitor::autoClose(const QString _close)
{
  int cursorIndex;
  int cursorLine;
  int length;
  getCursorPosition(&cursorLine, &cursorIndex);
  length = lineLength(cursorLine);

  // insert closing character if cursor is at EOL or the next character is a space
  if(cursorIndex == length-1)
  {
    insert(_close);
    return true;
  }
  else if(text(cursorLine).at(cursorIndex).toLatin1() == ' ')
  {
    insert(_close);
    return true;
  }
  return false;
}

//----------------------------------------------------------------------------------------------------------------------
bool Cebitor::closing(const QString _close)
{
  int cursorIndex;
  int cursorLine;
  //int length;
  getCursorPosition(&cursorLine, &cursorIndex);
  //length = lineLength(cursorLine);

  // remove duplicate if next character is the same as _close
  if(text(cursorLine).at(cursorIndex) == _close.at(0))
  {
    setSelection(cursorLine, cursorIndex, cursorLine, cursorIndex+1);
    removeSelectedText();
    return true;
  }
  return false;
}

//----------------------------------------------------------------------------------------------------------------------
void Cebitor::comment()
{
  int lineFrom;
  int indexFrom;
  int lineTo;
  int indexTo;
  // get selected lines or current line if no text is selected
  if(this->hasSelectedText())
  {
    this->getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
  }
  else
  {
    this->getCursorPosition(&lineFrom, &indexFrom);
    lineTo = lineFrom;
    indexTo = indexFrom;
  }

  // check if selected lines are already commented
  bool alreadyCommented = true;
  for(int i=lineFrom; i<=lineTo; i++)
  {
    QString lineText = this->text(i);
    if(!lineText.startsWith("//"))
    {
      alreadyCommented = false;
    }
  }

  // remove "//" from each line if they are already commented
  if(alreadyCommented)
  {
    for(int i=lineFrom; i<=lineTo; i++)
    {
      this->setSelection(i,0,i,2);
      this->removeSelectedText();
    }
    //offset original selection for reselecting text
    indexTo -= 2;
    if(indexFrom > 1)
    {
      indexFrom -= 2;
    }
  }

  // insert "//" at the start of each line
  else
  {
    for(int i=lineFrom; i<=lineTo; i++)
    {
      this->insertAt(QString("//"),i,0);
    }
    //offset original selection for reselecting text
    indexTo += 2;
    if(indexFrom != 0)
    {
      indexFrom += 2;
    }
  }

  // reselect to match original selection
  this->setSelection(lineFrom,indexFrom,lineTo,indexTo);
}

//----------------------------------------------------------------------------------------------------------------------
void Cebitor::charAdded(int _c)
{
  switch(_c)
  {
    case (int) '(': { autoClose(QString(')')); break; }
    case (int) '{': { autoClose(QString('}')); break; }
    case (int) '[': { autoClose(QString(']')); break; }

    case (int) ')': { closing(QString(')')); break; }
    case (int) '}': { closing(QString('}')); break; }
    case (int) ']': { closing(QString(']')); break; }

    // special case since " opens and closes
    case (int) '"':
    {
      if(!closing(QString('"')))
      {
        autoClose(QString('"'));
      }
      break;
    }
  }
}
