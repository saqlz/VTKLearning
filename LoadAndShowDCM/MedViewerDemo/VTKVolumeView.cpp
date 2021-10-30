#include "stdafx.h"
#include "VTKVolumeView.h"

//必须初始化对象
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>

//体绘制对象
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>

//渲染必要的信息
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

VTKVolumeView::VTKVolumeView()
{
}


VTKVolumeView::~VTKVolumeView()
{
}

void VTKVolumeView::Run()
{
    //1、读取DICOM序列
    std::string inputFilename = "D:\\TestData\\Data";
    vtkSmartPointer<vtkDICOMImageReader> pDicomImageReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    pDicomImageReader->SetDirectoryName(inputFilename.c_str());
    pDicomImageReader->Update();

    //2、体绘制Mapper
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> origMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    origMapper->SetInputData(pDicomImageReader->GetOutput());

    //3、裁剪体绘制Mapper
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    volumeMapper->SetInputData(pDicomImageReader->GetOutput());
    volumeMapper->SetCropping(1);                                   //开启Cropping功能
    volumeMapper->SetCroppingRegionPlanes(0, 500, 0, 500, 0, 100);  //Cropping区间,留一个问题500是像素还是
    volumeMapper->SetCroppingRegionFlags(0x0002000);                //将三维空间分为27个可视区域

                                                                    /*****************************************************************/
                                                                    //设置体绘制相关属性
    vtkSmartPointer<vtkVolumeProperty> volumeProperty =
        vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetInterpolationTypeToLinear();  //设置线性插值方式
    volumeProperty->ShadeOn();                       //开启阴影属性
    volumeProperty->SetAmbient(0.4);                 //设置环境温度
    volumeProperty->SetDiffuse(0.6);                 //设置漫反射系数
    volumeProperty->SetSpecular(0.2);                //设置镜面反射系数

                                                     //添加灰度不透明度属性
    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
        vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint(70, 0.0);
    compositeOpacity->AddPoint(90, 0.4);
    compositeOpacity->AddPoint(180, 0.6);
    volumeProperty->SetScalarOpacity(compositeOpacity);

    //添加梯度不同明度属性
    vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity =
        vtkSmartPointer<vtkPiecewiseFunction>::New();
    gradientOpacity->AddPoint(10, 0.0);
    gradientOpacity->AddPoint(90, 0.5);
    gradientOpacity->AddPoint(100, 1.0);
    volumeProperty->SetGradientOpacity(gradientOpacity);

    //添加颜色传输
    vtkSmartPointer<vtkColorTransferFunction> color =
        vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0, 0, 0, 0);
    color->AddRGBPoint(64, 1.0, 0.52, 0.3);
    color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
    color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
    volumeProperty->SetColor(color);
    /***********************************************************/

    //4、体绘制需要用vtkVolume对象
    vtkSmartPointer<vtkVolume> pOriginalVolume =
        vtkSmartPointer<vtkVolume>::New();
    pOriginalVolume->SetMapper(origMapper);
    pOriginalVolume->SetProperty(volumeProperty);

    vtkSmartPointer<vtkVolume> pCroppingVolume =
        vtkSmartPointer<vtkVolume>::New();
    pCroppingVolume->SetMapper(volumeMapper);
    pCroppingVolume->SetProperty(volumeProperty);

    //5、Renderer创建以及设置大小
    double origView[4] = { 0, 0, 0.5, 1 };
    double croppingView[4] = { 0.5, 0, 1, 1 };
    vtkSmartPointer<vtkRenderer> pOriginalRender =
        vtkSmartPointer<vtkRenderer>::New();
    pOriginalRender->AddVolume(pOriginalVolume);
    pOriginalRender->SetBackground(1, 1, 0);
    pOriginalRender->SetViewport(origView);

    //6、Renderer创建以及设置大小
    vtkSmartPointer<vtkRenderer> pCroppingRender =
        vtkSmartPointer<vtkRenderer>::New();
    pCroppingRender->AddVolume(pCroppingVolume);
    pCroppingRender->SetBackground(0, 1, 0);
    pCroppingRender->SetViewport(croppingView);

    //7、创建窗体
    vtkSmartPointer<vtkRenderWindow> pRenderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    pRenderWindow->AddRenderer(pOriginalRender);
    pRenderWindow->AddRenderer(pCroppingRender);
    pRenderWindow->SetWindowName("Cropping Volume");
    pRenderWindow->SetSize(640, 640);

    //8、创建窗体交互
    vtkSmartPointer<vtkRenderWindowInteractor> pRenderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    pRenderWindowInteractor->SetRenderWindow(pRenderWindow);

    //9、相机设置
    pOriginalRender->GetActiveCamera()->SetPosition(0, -1, 0);
    pOriginalRender->GetActiveCamera()->SetFocalPoint(0, 0, 0);
    pOriginalRender->GetActiveCamera()->SetViewUp(0, 0, 1);
    pOriginalRender->GetActiveCamera()->Azimuth(30);
    pOriginalRender->GetActiveCamera()->Elevation(30);
    pOriginalRender->ResetCamera();
    pCroppingRender->SetActiveCamera(pOriginalRender->GetActiveCamera());

    //10、渲染
    pRenderWindow->Render();
    pRenderWindowInteractor->Start();
}
