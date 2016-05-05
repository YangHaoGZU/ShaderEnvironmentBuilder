#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "project.h"

#include <ngl/Vec4.h>
#include "NGLScene.h"
#include <QMainWindow>
#include "ParserLib.h"
#include "button.h"
#include "Cebitor.h"
#include "StartupDialog.h"
#include "NewProjectWizard.h"
#include <vector>

class NGLScene;

//------------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief the MainWindow class for our program
/// @author Jonathan Flynn
/// @version 1.0
/// @date 01/03/16
//------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------
// Used to inherit the MainWindow from the generated form file ui_MainWindow.h
//------------------------------------------------------------------------------------------------------------------------
namespace Ui {
  class MainWindow;
}

//------------------------------------------------------------------------------------------------------------------------
/// @class MainWindow
/// @brief our main window used for holding all the Qt widgets
//------------------------------------------------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
Q_OBJECT      // must include this if you use Qt signals/slots
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
public:
    void createButtons();
    void updateShaderValues();
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for the MainWindow
  /// @param [in] _parent the parent window to create the MainWindow in
  //----------------------------------------------------------------------------------------------------------------------
  explicit MainWindow(QWidget *_parent = 0);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor must close down ngl and release OpenGL resources
  //----------------------------------------------------------------------------------------------------------------------
  ~MainWindow();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set terminal text
  //----------------------------------------------------------------------------------------------------------------------
  void setTerminalText(QString _txt);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief clear terminal text
  //----------------------------------------------------------------------------------------------------------------------
  void clearTerminalText();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief show startup dialog
  //----------------------------------------------------------------------------------------------------------------------
  void showStartDialog();
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
public slots:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds line marker to text editor for _shaderName at lineNum
  /// @param [in] _shaderName Name of the shader type
  /// @param [in] _lineNum line to add error symbol at
  //----------------------------------------------------------------------------------------------------------------------
  void addError(QString _shaderName, int _lineNum);
private slots:
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief activated when Compile Shader button is clicked
  //----------------------------------------------------------------------------------------------------------------------
  void on_m_btn_compileShader_clicked();
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
  void shapeTriggered();
  void objOpened();
  void on_actionStartup_Window_triggered();
  void on_actionNew_triggered();
  void on_actionSaveProject_triggered();
  void on_actionSaveProjectAs_triggered();
  void printUniforms();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief function that is activated when Load Texture is clicked
  void on_m_actionLoad_Texture_triggered();
  //----------------------------------------------------------------------------------------------------------------------

private:
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our QScintilla widget1 (vertex)
  //----------------------------------------------------------------------------------------------------------------------
  Cebitor *m_qsci1;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our QScintilla widget2 (fragment)
  //----------------------------------------------------------------------------------------------------------------------
  Cebitor *m_qsci2;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the generated widgets created from the form using ui_MainWindow.h
  //----------------------------------------------------------------------------------------------------------------------
  Ui::MainWindow *m_ui;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our openGL widget using NGLScene
  //----------------------------------------------------------------------------------------------------------------------
  NGLScene *m_gl;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief startup Dialog
  //----------------------------------------------------------------------------------------------------------------------
  StartupDialog *m_startDialog;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief create QsciScintilla widget in the style of sublime defaults
  /// @param [in] _parent the parent widget to fill with the new Qsci Widget
  //----------------------------------------------------------------------------------------------------------------------
  Cebitor *createQsciWidget(QWidget *_parent = 0);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief loads a text file from a path to a tab
  /// @param [in] _path the location of the file
  /// @param [in] _qsci the QsciScintilla tab to be filled with the file
  //----------------------------------------------------------------------------------------------------------------------
  bool loadTextFileToTab(QString _path, Cebitor &_qsci);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt Event called when a key is pressed
  /// @param [in] _event the Qt event to query for size etc
  //----------------------------------------------------------------------------------------------------------------------
  void keyPressEvent(QKeyEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  parserLib *m_parForButton;
  std::vector<Button*> m_buttonList;

  Project *m_project;

};

#endif // MAINWINDOW_H
