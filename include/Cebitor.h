#ifndef CEBITOR_H
#define CEBITOR_H

#include "QsciLexerGlsl.h"

#include <Qsci/qsciscintilla.h>

//------------------------------------------------------------------------------------------------------------------------
/// @file Cebitor.h
/// @brief CEB text editor, implementation of QScintilla
/// @author Phil Rouse
/// @version 1.0
/// @date 03/03/16
//------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------
/// @class Cebitor
/// @brief CEB text editor class derived from QsciScintilla
//------------------------------------------------------------------------------------------------------------------------
class Cebitor : public QsciScintilla
{
  Q_OBJECT;

public:
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief enum for supported style types
  //----------------------------------------------------------------------------------------------------------------------
  enum MarkerType
  {
    ERROR,
    WARNING,
    FILESTART
  };
  //------------------------------------------------------------------------------------------------------------------------
  /// @brief Cebitor constructor, initialises default values
  //------------------------------------------------------------------------------------------------------------------------
  Cebitor(QWidget *_parent);

public slots:
  //------------------------------------------------------------------------------------------------------------------------
  /// @brief Removes all error and warning line markers
  //------------------------------------------------------------------------------------------------------------------------
  void clearErrors();

protected:
  std::vector<int> m_fileMarkers;
  //------------------------------------------------------------------------------------------------------------------------
  /// @brief adds closing character for braces and quotes
  /// @param [in] _close closing character
  /// @returns true if closing character is added
  //------------------------------------------------------------------------------------------------------------------------
  bool autoClose(const QString _close);
  //------------------------------------------------------------------------------------------------------------------------
  /// @brief removes duplicates of closing braces and quotes
  /// @param [in] _close closing character
  /// @returns true if duplicate character is replaced
  //------------------------------------------------------------------------------------------------------------------------
  bool closing(const QString _close);
  //------------------------------------------------------------------------------------------------------------------------
  /// @brief adds indentation after newline following a "{"
  //------------------------------------------------------------------------------------------------------------------------
  void braceIndent();
protected slots:
  //------------------------------------------------------------------------------------------------------------------------
  /// @brief comment out all selected lines
  //------------------------------------------------------------------------------------------------------------------------
  void comment();
  //------------------------------------------------------------------------------------------------------------------------
  /// @brief called when a character is added
  //------------------------------------------------------------------------------------------------------------------------
  void charAdded(int _c);
};

#endif
