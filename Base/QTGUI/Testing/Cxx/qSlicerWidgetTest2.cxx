/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer

=========================================================================auto=*/

// this is a test of the slicer slice logic resampling pipeline

// Qt includes
#include <QApplication>
#include <QTimer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProcessEnvironment>
#include <QWebView>

// SlicerQt includes
#include "qSlicerWidget.h"

// QVTK includes
#include <QVTKWidget.h>

// Slicer includes
#include <vtkMRMLSliceLogic.h>

// MRML includes
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLScalarVolumeDisplayNode.h>
#include <vtkMRMLVolumeArchetypeStorageNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLSliceCompositeNode.h>
#include <vtkMRMLSliceNode.h>
#include <vtkMRMLColorTableNode.h>

// VTK includes
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageMapper.h>
#include <vtkProperty2D.h>
#include <vtkActor2D.h>
#include <vtkVersion.h>

// STD includes

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 900

//
// Build the slicer rendering infrastructure to render data from volume file
// into the given render window.  Populate the scene and return the slice logic.
//
vtkMRMLSliceLogic *setupSliceDisplay(vtkMRMLScene *scene, vtkRenderWindow *rw, const char *archetype)
{
  //
  // allocate needed nodes, add them to the scene, and connect them together
  //
  vtkMRMLScalarVolumeNode *volumeNode = vtkMRMLScalarVolumeNode::New();
  vtkMRMLScalarVolumeDisplayNode *displayNode = vtkMRMLScalarVolumeDisplayNode::New();
  vtkMRMLVolumeArchetypeStorageNode *storageNode = vtkMRMLVolumeArchetypeStorageNode::New();
  vtkMRMLColorTableNode *colorNode = vtkMRMLColorTableNode::New();

  volumeNode->SetScene( scene );
  displayNode->SetScene( scene );
  colorNode->SetScene( scene );
  colorNode->SetTypeToGrey();
  storageNode->SetScene( scene );

  scene->AddNode( volumeNode );
  scene->AddNode( colorNode );
  scene->AddNode( displayNode );
  scene->AddNode( storageNode );

  volumeNode->SetName( "sample volume" );
  volumeNode->SetAndObserveStorageNodeID( storageNode->GetID() );
  volumeNode->SetAndObserveDisplayNodeID( displayNode->GetID() );
  displayNode->SetAndObserveColorNodeID( colorNode->GetID() );

  // read the data
  storageNode->SetFileName( archetype );
  storageNode->ReadData( volumeNode );


  //
  // Create the slice logic to create the slice image
  //
  vtkMRMLSliceLogic *sliceLogic = vtkMRMLSliceLogic::New();
  sliceLogic->SetName( "Image Viewer" );
  sliceLogic->SetMRMLScene(scene);

  vtkMRMLSliceNode *sliceNode = sliceLogic->GetSliceNode();
  sliceNode->SetOrientationToCoronal();

  vtkMRMLSliceCompositeNode *compositeNode = sliceLogic->GetSliceCompositeNode();
  compositeNode->SetBackgroundVolumeID( volumeNode->GetID() );

  //
  // get the output slice and put it into the render window
  //
  // vtkImageData *slice = 0;
#if (VTK_MAJOR_VERSION <= 5)
  vtkImageData *slice = sliceLogic->GetImageData();
#else
  vtkAlgorithmOutput *slicePort = sliceLogic->GetImageDataConnection();
#endif

  vtkImageMapper *mapper = vtkImageMapper::New();
  mapper->SetColorWindow( 255. );
  mapper->SetColorLevel ( 127.5 );
#if (VTK_MAJOR_VERSION <= 5)
  mapper->SetInput( slice );
#else
  mapper->SetInputConnection( slicePort );
#endif
  vtkActor2D *actor = vtkActor2D::New();
  actor->SetMapper( mapper );
  actor->GetProperty()->SetDisplayLocationToBackground();
  vtkRenderer *renderer = vtkRenderer::New();
  renderer->AddActor2D( actor );
  rw->AddRenderer( renderer );

  // clean up
  mapper->Delete();
  actor->Delete();
  renderer->Delete();
  volumeNode->Delete();
  colorNode->Delete();
  displayNode->Delete();
  storageNode->Delete();

  return( sliceLogic );
}

int qSlicerWidgetTest2(int argc, char * argv[] )
{
  //
  // Create a simple gui with a quit button and render window
  //
  QApplication app(argc, argv);
  QWidget parentWidget;
  parentWidget.setWindowTitle("qSlicerWidgetTest2");
  QVBoxLayout vbox;
  parentWidget.setLayout(&vbox);

  QPushButton quitButton;
  quitButton.setParent(&parentWidget);
  quitButton.setText("Quit");
  vbox.addWidget(&quitButton);
  app.connect(&quitButton, SIGNAL(clicked()), &parentWidget, SLOT(close()));

  qSlicerWidget* widget = new qSlicerWidget();
  widget->setParent(&parentWidget);
  vbox.addWidget(widget);

  QVTKWidget* vtkWidget = new QVTKWidget();
  vtkWidget->setParent(&parentWidget);
  vbox.addWidget(vtkWidget);
  vtkWidget->GetRenderWindow()->Render();

  QWebView webView;
  webView.setParent(&parentWidget);
  webView.setUrl(QUrl("http://pyjs.org/examples"));
  vbox.addWidget(&webView);

  vtkMRMLScene* scene = vtkMRMLScene::New();
  widget->setMRMLScene(scene);
  parentWidget.show();
  parentWidget.raise();

  //
  // Get the sample data from a known spot in the build tree
  // (relies on SLICER_HOME being set correctly, which it
  // will be when the launcher is used).
  //
  QProcessEnvironment env;
  QString qarchetype = env.value("SLICER_HOME", "");
  qarchetype.append("share/MRML/Testing/TestData/fixed.nrrd");
  QByteArray archetype = qarchetype.toAscii();

  vtkMRMLSliceLogic *sliceLogic = setupSliceDisplay(
          scene, vtkWidget->GetRenderWindow(), archetype.data() );

  // quit after 5 seconds if the Quit button hasn't been clicked
  QTimer::singleShot(5000, &parentWidget, SLOT(close()));

  // run the app
  app.exec();

  sliceLogic->Delete();
  scene->Delete();

  return EXIT_SUCCESS;
}

