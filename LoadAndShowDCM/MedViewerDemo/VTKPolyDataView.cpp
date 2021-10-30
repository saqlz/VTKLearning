#include "stdafx.h"
#include "VTKPolyDataView.h"

//必须初始化对象
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

//渲染必要的信息
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
    //定义立方体的顶点坐标
    static float x[8][3] = { { 0,0,0 },{ 1,0,0 },{ 1,1,0 },{ 0,1,0 },{ 0,0,1 },{ 1,0,1 },{ 1,1,1 },{ 0,1,1 } };

    //定义单元，每4个顶点建立一个四边形单元，共计6个单元
    static vtkIdType pts[6][4] = { { 0,1,2,3 },{ 4,5,6,7 },{ 0,1,5,4 },{ 1,2,6,5 },{ 2,3,7,6 },{ 3,0,4,7 } };

    //创建对象
    vtkSmartPointer<vtkPolyData> cube = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
    
    //存储顶点
    for (int i = 0; i < 8; i++) 
    {
        points->InsertPoint(i, x[i]);
    }
    
    //设定单元
    for (int i = 0; i < 6; i++) 
    {
        polys->InsertNextCell(4, pts[i]); 
    }

    //设定每个顶点的标量值
    for (int i = 0; i < 8; i++) 
    {
        scalars->InsertTuple1(i, i); 
    }

    //创建多边形数据
    cube->SetPoints(points);

    //设定单元类型为多边形
    cube->SetPolys(polys);

    //设定每个顶点的标量值
    cube->GetPointData()->SetScalars(scalars);

    //定义颜色映射表
    vtkSmartPointer<vtkLookupTable> pColorTable = vtkSmartPointer<vtkLookupTable>::New();
    //设置颜色表中的颜色
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

    //数据映射
    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputData(cube);
    cubeMapper->SetScalarRange(0, 7);
    cubeMapper->SetLookupTable(pColorTable);

    vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(cubeMapper);

    //Renderer创建以及设置大小
    vtkSmartPointer<vtkRenderer> pRender =
        vtkSmartPointer<vtkRenderer>::New();
    pRender->SetBackground(1, 1, 1);
    pRender->AddActor(cubeActor);

    //创建窗体
    vtkSmartPointer<vtkRenderWindow> pRenderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    pRenderWindow->AddRenderer(pRender);
    pRenderWindow->SetWindowName("Polydata");
    pRenderWindow->SetSize(640, 640);

    //创建窗体交互
    vtkSmartPointer<vtkRenderWindowInteractor> pRenderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    pRenderWindowInteractor->SetRenderWindow(pRenderWindow);

    //相机设置
    pRender->GetActiveCamera()->SetPosition(1, 1, 1);
    pRender->GetActiveCamera()->SetFocalPoint(0, 1, 0);
    pRender->GetActiveCamera()->Azimuth(30);
    pRender->GetActiveCamera()->Elevation(30);
    pRender->ResetCamera();

    //渲染
    pRenderWindow->Render();
    pRenderWindowInteractor->Start();
}
