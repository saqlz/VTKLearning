#include "stdafx.h"
#include "VTKImageView.h"

//必须初始化对象
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#include "vtkSmartPointer.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageCanvasSource2D.h"
#include "vtkImageViewer2.h"

//图像绘制对象
#include "vtkImageActor.h"

//渲染必要的信息
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"

VTKImageView::VTKImageView()
{
}


VTKImageView::~VTKImageView()
{
}

void VTKImageView::Run()
{
    //1、获取数据源
    vtkSmartPointer<vtkImageCanvasSource2D> pCanvasSource =
        vtkSmartPointer<vtkImageCanvasSource2D>::New();
    pCanvasSource->SetScalarTypeToUnsignedChar();
    pCanvasSource->SetNumberOfScalarComponents(1);
    pCanvasSource->SetExtent(0, 100, 0, 100, 0, 0);
    pCanvasSource->SetDrawColor(0, 0, 0, 0);
    pCanvasSource->FillBox(0, 100, 0, 100);
    pCanvasSource->SetDrawColor(255, 0, 0, 0);
    pCanvasSource->FillBox(20, 40, 20, 40);
    pCanvasSource->Update();

    //2、创建演员
    vtkSmartPointer<vtkImageActor> pActor =
        vtkSmartPointer<vtkImageActor>::New();
    pActor->SetInputData(pCanvasSource->GetOutput());

    //3、Renderer创建以及设置大小和背景
    double viewport[4] = { 0, 0, 1, 1 };
    vtkSmartPointer<vtkRenderer> pRenderer =
        vtkSmartPointer<vtkRenderer>::New();
    pRenderer->SetViewport(viewport);
    pRenderer->AddActor(pActor);
    pRenderer->ResetCamera();
    pRenderer->SetBackground(1.0, 1.0, 1.0);

    //设置渲染窗口
    vtkSmartPointer<vtkRenderWindow> pRenderwindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    pRenderwindow->AddRenderer(pRenderer);
    pRenderwindow->SetSize(640, 640);
    pRenderwindow->Render();
    pRenderwindow->SetWindowName("ImageCanvasSource2D");

    //设置窗口交互
    vtkSmartPointer<vtkRenderWindowInteractor> pRenderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style =
        vtkSmartPointer<vtkInteractorStyleImage>::New();
    pRenderWindowInteractor->SetInteractorStyle(style);
    pRenderWindowInteractor->SetRenderWindow(pRenderwindow);
    pRenderWindowInteractor->Initialize();
    pRenderWindowInteractor->Start();

    //留一个问题vtkImageAppendComponents以及vtkImageBlend如何使用？
}

void VTKImageView::RunDicom()
{
    //1、读取DICOM序列
    std::string inputFilename = "D:\\TestData\\Data";
    vtkSmartPointer<vtkDICOMImageReader> pDicomImageReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    pDicomImageReader->SetDirectoryName(inputFilename.c_str());
    pDicomImageReader->Update();

    //2、创建演员
    vtkSmartPointer<vtkImageActor> pActor =
        vtkSmartPointer<vtkImageActor>::New();
    pActor->SetInputData(pDicomImageReader->GetOutput());

    //3、Renderer创建以及设置大小和背景
    vtkSmartPointer<vtkRenderer> pRenderer =
        vtkSmartPointer<vtkRenderer>::New();
    pRenderer->AddActor(pActor);
    pRenderer->ResetCamera();
    pRenderer->SetBackground(1.0, 1.0, 1.0);

    //4、设置渲染窗口
    vtkSmartPointer<vtkRenderWindow> pRenderwindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    pRenderwindow->AddRenderer(pRenderer);
    pRenderwindow->SetSize(640, 640);
    pRenderwindow->Render();
    pRenderwindow->SetWindowName("ImageCanvasSource2D");

    //5、设置窗口交互
    vtkSmartPointer<vtkRenderWindowInteractor> pRenderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style =
        vtkSmartPointer<vtkInteractorStyleImage>::New();
    pRenderWindowInteractor->SetInteractorStyle(style);
    pRenderWindowInteractor->SetRenderWindow(pRenderwindow);
    pRenderWindowInteractor->Initialize();
    pRenderWindowInteractor->Start();
}

void VTKImageView::RunDicomUsingImageView2()
{
    //1、读取DICOM序列
    std::string inputFilename = "D:\\TestData\\Data";
    vtkSmartPointer<vtkDICOMImageReader> pDicomImageReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    pDicomImageReader->SetDirectoryName(inputFilename.c_str());
    pDicomImageReader->Update();

    //2、使用vtkImageViewer2
    vtkSmartPointer<vtkImageViewer2> pImageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    pImageViewer->SetInputConnection(pDicomImageReader->GetOutputPort());

    //3、设置窗口交互
    vtkSmartPointer<vtkRenderWindowInteractor> pRenderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    pImageViewer->SetupInteractor(pRenderWindowInteractor);   //设置窗口交互
    pImageViewer->SetColorLevel(500);
    pImageViewer->SetColorWindow(2000);
    pImageViewer->SetSlice(0);
    pImageViewer->SetSliceOrientationToXY();
    pImageViewer->Render();

    pImageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
    pImageViewer->SetSize(640, 640);
    pImageViewer->GetRenderWindow()->SetWindowName("Display");
    pRenderWindowInteractor->Initialize();
    pRenderWindowInteractor->Start();
}


