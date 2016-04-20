#include <QtWidgets>
#include "NewProjectWizard.h"
#include <QFileDialog>
#include <QModelIndex>
#include <algorithm>

NewProjectWizard::NewProjectWizard(QWidget *parent): QWizard(parent)
{
  m_fileModel = new QFileSystemModel;
  QString shadersPath = QDir::currentPath() + "/shaders/files";
  m_fileModel->setRootPath(shadersPath);

  addPage(new IntroPage);
  addPage(new ProjectInfoPage);
  addPage(new GlslFilesPage(this));
  addPage(new ConclusionPage);

  setWindowTitle(tr("New Project Wizard"));

}

void NewProjectWizard::accept()
{
  QByteArray projectName = field("projectName").toByteArray();
  int glslVersion = field("glslVersion").toInt();
  int glslProfile = field("glslProfile").toInt();
  QByteArray vertexName = field("vertexFileName").toByteArray();
  QByteArray fragmentName = field("fragmentFileName").toByteArray();

  QString projectDir = field("projectDirectory").toString();

  QModelIndexList vertexFiles = m_vertexSelectModel->selectedIndexes();
  QModelIndexList fragmentFiles = m_fragmentSelectModel->selectedIndexes();

  qDebug() << projectName << "\n" << glslVersion << "\n" << glslProfile << "\n"
           << vertexName << "\n" << fragmentName << "\n" << projectDir << "\n"
           << vertexFiles;

  QDialog::accept();
}


IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));

    label = new QLabel(tr("This wizard will generate a CEB project "
                          "definition. You simply "
                          "need to specify the project name and set a few "
                          "options to produce a json project file "
                          "implementation for your new OpenGL shader."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

ProjectInfoPage::ProjectInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
  setTitle(tr("Project Information"));
  setSubTitle(tr("Specify basic information about the project for which you "
                 "want to generate."));

  m_l_projectName = new QLabel(tr("Project name:"));
  m_le_projectName = new QLineEdit();
  m_l_projectName->setBuddy(m_le_projectName);

  m_l_projectDirectory = new QLabel(tr("Project Directory:"));
  m_b_browseDirectory = new QPushButton(tr("Browse..."));
  m_le_projectDirectory = new QLineEdit();
  m_l_projectDirectory->setBuddy(m_le_projectDirectory);

  QFrame *line = new QFrame();
  line->setObjectName(QString::fromUtf8("line"));
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);

  m_l_glslVersion = new QLabel(tr("GLSL Version:"));
  m_cb_glslVersion = new QComboBox();
  QStringList glslVer = {"1.10", "1.20", "1.30", "1.40", "1.50", "3.30", "4.00",
                         "4.10", "4.20", "4.30", "4.30", "4.40", "4.50"};
  std::reverse(glslVer.begin(), glslVer.end());
  m_cb_glslVersion->addItems(glslVer);
  m_cb_glslVersion->setEditable(true);
  m_cb_glslVersion->setInsertPolicy(QComboBox::InsertAtTop);

  m_l_glslProfile = new QLabel(tr("GLSL Profile:"));
  m_cb_glslProfile = new QComboBox();
  m_cb_glslProfile->addItems({"Core", "Compatibility"});

  registerField("projectName*", m_le_projectName);
  registerField("projectDirectory*", m_le_projectDirectory);
  registerField("glslVersion", m_cb_glslVersion);
  registerField("glslProfile", m_cb_glslProfile);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(m_l_projectName, 0, 0);
  layout->addWidget(m_le_projectName, 0, 1, 1, 2);
  layout->addWidget(m_l_projectDirectory, 1, 0);
  layout->addWidget(m_le_projectDirectory, 1, 1);
  layout->addWidget(m_b_browseDirectory, 1, 2);
  layout->addWidget(line, 2,0,1,3);
  layout->addWidget(m_l_glslVersion, 3,0);
  layout->addWidget(m_cb_glslVersion, 3,1,1,2);
  layout->addWidget(m_l_glslProfile, 4,0);
  layout->addWidget(m_cb_glslProfile, 4,1,1,2);
  setLayout(layout);

  connect(m_b_browseDirectory, SIGNAL (clicked()),this, SLOT(setProjectDirectory()));
}

void ProjectInfoPage::initializePage()
{
  m_le_projectName->setText("Phong");
  m_le_projectDirectory->setText(QDir::homePath());
  m_cb_glslVersion->setCurrentIndex(0);
  m_cb_glslProfile->setCurrentIndex(0);
}

void ProjectInfoPage::setProjectDirectory()
{
  QFileDialog dialog(this);
  dialog.setFileMode(QFileDialog::Directory);
  dialog.setOption(QFileDialog::ShowDirsOnly, true);
  QStringList folderNames;
  if (dialog.exec())
  {
      folderNames = dialog.selectedFiles();
      qDebug() << folderNames;
  }
  m_le_projectDirectory->setText(folderNames.first());
}


GlslFilesPage::GlslFilesPage(QWidget *parent)
  : QWizardPage(parent)
{
  m_wizard = static_cast<NewProjectWizard*>(parent);
  QString shadersPath = QDir::currentPath() + "/shaders/files";
  setTitle(tr("Shaders & GLSL Files"));
  setSubTitle(tr("Specify the name of the Vertex and Fragment shaders as well "
                 "what GLSL files will be linked."));

  m_l_vertexName = new QLabel(tr("Vertex Name:"));
  m_le_vertexName = new QLineEdit();
  m_l_vertexName->setBuddy(m_le_vertexName);

  m_l_fragmentName = new QLabel(tr("Fragment Name:"));
  m_le_fragmentName = new QLineEdit();
  m_l_fragmentName->setBuddy(m_le_fragmentName);

  m_l_vertexSelectFiles = new QLabel(tr("Vertex Files:"));
  m_l_fragmentSelectFiles = new QLabel(tr("Fragment Files:"));

  /// @todo Make the two tree views into a single function since a lot of values
  /// are set the same. Just done this for the time being for speed and testing.
  m_tv_vertexSelectFiles = new QTreeView;
  m_tv_fragmentSelectFiles = new QTreeView;

  std::vector<QTreeView*> tViews = {m_tv_vertexSelectFiles, m_tv_fragmentSelectFiles};

  for (auto tv: tViews)
  {
    tv->setModel(m_wizard->m_fileModel);
    tv->setRootIndex(m_wizard->m_fileModel->index(shadersPath));
    tv->setSelectionMode(QAbstractItemView::MultiSelection);
    tv->setColumnHidden(1,true);
    tv->setColumnHidden(2,true);
    tv->setColumnHidden(3,true);
  }

  m_wizard->m_vertexSelectModel = m_tv_vertexSelectFiles->selectionModel();
  m_wizard->m_fragmentSelectModel = m_tv_fragmentSelectFiles->selectionModel();

  connect(m_wizard->m_vertexSelectModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
          this, SLOT(vertexSelectionChanged(const QItemSelection&, const QItemSelection&)));
  connect(m_wizard->m_fragmentSelectModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
          this, SLOT(fragmentSelectionChanged(const QItemSelection&, const QItemSelection&)));

  registerField("vertexFileName*", m_le_vertexName);
  registerField("fragmentFileName*", m_le_fragmentName);

  m_gb_glslFiles = new QGroupBox(tr("Select GLSL Files"));
  m_gb_glslFiles->setMinimumHeight(300);
  QGridLayout *groupBoxLayout = new QGridLayout;
  groupBoxLayout->addWidget(m_l_vertexSelectFiles, 0, 0);
  groupBoxLayout->addWidget(m_l_fragmentSelectFiles, 0, 1);
  groupBoxLayout->addWidget(m_tv_vertexSelectFiles, 1, 0);
  groupBoxLayout->addWidget(m_tv_fragmentSelectFiles, 1, 1);

  m_gb_glslFiles->setLayout(groupBoxLayout);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(m_l_vertexName, 0, 0);
  layout->addWidget(m_le_vertexName, 0, 1);
  layout->addWidget(m_l_fragmentName, 1, 0);
  layout->addWidget(m_le_fragmentName, 1, 1);
  layout->addWidget(m_gb_glslFiles, 2, 0, 1, 2);
  setLayout(layout);
}

void GlslFilesPage::initializePage()
{
  QString projectName = field("projectName").toString();
  m_le_vertexName->setText(projectName + "Vertex");
  m_le_fragmentName->setText(projectName + "Fragment");
}

void GlslFilesPage::vertexSelectionChanged(const QItemSelection &_selected,
                                           const QItemSelection &_deselected)
{
  deselectDirs(_selected, m_wizard->m_vertexSelectModel);
}

void GlslFilesPage::fragmentSelectionChanged(const QItemSelection &_selected,
                                           const QItemSelection &_deselected)
{
  deselectDirs(_selected, m_wizard->m_fragmentSelectModel);
}

void GlslFilesPage::deselectDirs(const QItemSelection &_selected,
                                 QItemSelectionModel *_selectModel)
{
  QModelIndexList items = _selected.indexes();
  for (auto index: items)
  {
    QFileInfo info = m_wizard->m_fileModel->fileInfo(index);
    if (info.isDir())
    {
      _selectModel->select(index, QItemSelectionModel::Deselect);
    }
  }
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Conclusion"));

    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void ConclusionPage::initializePage()
{
    QString finishText = wizard()->buttonText(QWizard::FinishButton);
    finishText.remove('&');
    label->setText(tr("Click %1 to generate the class skeleton.")
                   .arg(finishText));
}
