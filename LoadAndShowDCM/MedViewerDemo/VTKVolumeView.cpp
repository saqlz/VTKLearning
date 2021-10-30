#include "stdafx.h"
#include "VTKVolumeView.h"

//�����ʼ������
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>

//����ƶ���
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>

//��Ⱦ��Ҫ����Ϣ
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
    //1����ȡDICOM����
    std::string inputFilename = "D:\\TestData\\Data";
    vtkSmartPointer<vtkDICOMImageReader> pDicomImageReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    pDicomImageReader->SetDirectoryName(inputFilename.c_str());
    pDicomImageReader->Update();

    //2�������Mapper
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> origMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    origMapper->SetInputData(pDicomImageReader->GetOutput());

    //3���ü������Mapper
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    volumeMapper->SetInputData(pDicomImageReader->GetOutput());
    volumeMapper->SetCropping(1);                                   //����Cropping����
    volumeMapper->SetCroppingRegionPlanes(0, 500, 0, 500, 0, 100);  //Cropping����,��һ������500�����ػ���
    volumeMapper->SetCroppingRegionFlags(0x0002000);                //����ά�ռ��Ϊ27����������

                                                                    /*****************************************************************/
                                                                    //����������������
    vtkSmartPointer<vtkVolumeProperty> volumeProperty =
        vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetInterpolationTypeToLinear();  //�������Բ�ֵ��ʽ
    volumeProperty->ShadeOn();                       //������Ӱ����
    volumeProperty->SetAmbient(0.4);                 //���û����¶�
    volumeProperty->SetDiffuse(0.6);                 //����������ϵ��
    volumeProperty->SetSpecular(0.2);                //���þ��淴��ϵ��

                                                     //��ӻҶȲ�͸��������
    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
        vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint(70, 0.0);
    compositeOpacity->AddPoint(90, 0.4);
    compositeOpacity->AddPoint(180, 0.6);
    volumeProperty->SetScalarOpacity(compositeOpacity);

    //����ݶȲ�ͬ��������
    vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity =
        vtkSmartPointer<vtkPiecewiseFunction>::New();
    gradientOpacity->AddPoint(10, 0.0);
    gradientOpacity->AddPoint(90, 0.5);
    gradientOpacity->AddPoint(100, 1.0);
    volumeProperty->SetGradientOpacity(gradientOpacity);

    //�����ɫ����
    vtkSmartPointer<vtkColorTransferFunction> color =
        vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0, 0, 0, 0);
    color->AddRGBPoint(64, 1.0, 0.52, 0.3);
    color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
    color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
    volumeProperty->SetColor(color);
    /***********************************************************/

    //4���������Ҫ��vtkVolume����
    vtkSmartPointer<vtkVolume> pOriginalVolume =
        vtkSmartPointer<vtkVolume>::New();
    pOriginalVolume->SetMapper(origMapper);
    pOriginalVolume->SetProperty(volumeProperty);

    vtkSmartPointer<vtkVolume> pCroppingVolume =
        vtkSmartPointer<vtkVolume>::New();
    pCroppingVolume->SetMapper(volumeMapper);
    pCroppingVolume->SetProperty(volumeProperty);

    //5��Renderer�����Լ����ô�С
    double origView[4] = { 0, 0, 0.5, 1 };
    double croppingView[4] = { 0.5, 0, 1, 1 };
    vtkSmartPointer<vtkRenderer> pOriginalRender =
        vtkSmartPointer<vtkRenderer>::New();
    pOriginalRender->AddVolume(pOriginalVolume);
    pOriginalRender->SetBackground(1, 1, 0);
    pOriginalRender->SetViewport(origView);

    //6��Renderer�����Լ����ô�С
    vtkSmartPointer<vtkRenderer> pCroppingRender =
        vtkSmartPointer<vtkRenderer>::New();
    pCroppingRender->AddVolume(pCroppingVolume);
    pCroppingRender->SetBackground(0, 1, 0);
    pCroppingRender->SetViewport(croppingView);

    //7����������
    vtkSmartPointer<vtkRenderWindow> pRenderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    pRenderWindow->AddRenderer(pOriginalRender);
    pRenderWindow->AddRenderer(pCroppingRender);
    pRenderWindow->SetWindowName("Cropping Volume");
    pRenderWindow->SetSize(640, 640);

    //8���������彻��
    vtkSmartPointer<vtkRenderWindowInteractor> pRenderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    pRenderWindowInteractor->SetRenderWindow(pRenderWindow);

    //9���������
    pOriginalRender->GetActiveCamera()->SetPosition(0, -1, 0);
    pOriginalRender->GetActiveCamera()->SetFocalPoint(0, 0, 0);
    pOriginalRender->GetActiveCamera()->SetViewUp(0, 0, 1);
    pOriginalRender->GetActiveCamera()->Azimuth(30);
    pOriginalRender->GetActiveCamera()->Elevation(30);
    pOriginalRender->ResetCamera();
    pCroppingRender->SetActiveCamera(pOriginalRender->GetActiveCamera());

    //10����Ⱦ
    pRenderWindow->Render();
    pRenderWindowInteractor->Start();
}
