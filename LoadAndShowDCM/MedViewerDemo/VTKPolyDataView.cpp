#include "stdafx.h"
#include "VTKPolyDataView.h"

//�����ʼ������
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#include "vtkSmartPointer.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkLookupTable.h"

//��Ⱦ��Ҫ����Ϣ
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

VTKPolyDataView::VTKPolyDataView()
{
}

VTKPolyDataView::~VTKPolyDataView()
{
}

void VTKPolyDataView::Run()
{
    //����������Ķ�������
    static float x[8][3] = { { 0,0,0 },{ 1,0,0 },{ 1,1,0 },{ 0,1,0 },{ 0,0,1 },{ 1,0,1 },{ 1,1,1 },{ 0,1,1 } };

    //���嵥Ԫ��ÿ4�����㽨��һ���ı��ε�Ԫ������6����Ԫ
    static vtkIdType pts[6][4] = { { 0,1,2,3 },{ 4,5,6,7 },{ 0,1,5,4 },{ 1,2,6,5 },{ 2,3,7,6 },{ 3,0,4,7 } };

    //��������
    vtkSmartPointer<vtkPolyData> cube = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
    
    //�洢����
    for (int i = 0; i < 8; i++) 
    {
        points->InsertPoint(i, x[i]);
    }
    
    //�趨��Ԫ
    for (int i = 0; i < 6; i++) 
    {
        polys->InsertNextCell(4, pts[i]); 
    }

    //�趨ÿ������ı���ֵ
    for (int i = 0; i < 8; i++) 
    {
        scalars->InsertTuple1(i, i); 
    }

    //�������������
    cube->SetPoints(points);

    //�趨��Ԫ����Ϊ�����
    cube->SetPolys(polys);

    //�趨ÿ������ı���ֵ
    cube->GetPointData()->SetScalars(scalars);

    //������ɫӳ���
    vtkSmartPointer<vtkLookupTable> pColorTable = vtkSmartPointer<vtkLookupTable>::New();
    //������ɫ���е���ɫ
    pColorTable->SetNumberOfColors(8);
    pColorTable->SetTableValue(0, 1.0, 0.0, 0.0, 1.0);
    pColorTable->SetTableValue(1, 0.0, 1.0, 0.0, 1.0);
    pColorTable->SetTableValue(2, 1.0, 1.0, 0.0, 1.0);
    pColorTable->SetTableValue(3, 0.0, 0.0, 1.0, 1.0);
    pColorTable->SetTableValue(4, 1.0, 0.0, 1.0, 1.0);
    pColorTable->SetTableValue(5, 0.0, 1.0, 1.0, 1.0);
    pColorTable->SetTableValue(6, 1.0, 1.0, 1.0, 1.0);
    pColorTable->SetTableValue(7, 0.0, 0.0, 0.0, 1.0);
    pColorTable->Build();

    //����ӳ��
    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputData(cube);
    cubeMapper->SetScalarRange(0, 7);
    cubeMapper->SetLookupTable(pColorTable);

    vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(cubeMapper);

    //Renderer�����Լ����ô�С
    vtkSmartPointer<vtkRenderer> pRender =
        vtkSmartPointer<vtkRenderer>::New();
    pRender->SetBackground(1, 1, 1);
    pRender->AddActor(cubeActor);

    //��������
    vtkSmartPointer<vtkRenderWindow> pRenderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    pRenderWindow->AddRenderer(pRender);
    pRenderWindow->SetWindowName("Polydata");
    pRenderWindow->SetSize(640, 640);

    //�������彻��
    vtkSmartPointer<vtkRenderWindowInteractor> pRenderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    pRenderWindowInteractor->SetRenderWindow(pRenderWindow);

    //�������
    pRender->GetActiveCamera()->SetPosition(1, 1, 1);
    pRender->GetActiveCamera()->SetFocalPoint(0, 1, 0);
    pRender->GetActiveCamera()->Azimuth(30);
    pRender->GetActiveCamera()->Elevation(30);
    pRender->ResetCamera();

    //��Ⱦ
    pRenderWindow->Render();
    pRenderWindowInteractor->Start();
}
