// MedViewerDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "VTKImageView.h"
#include "VTKVolumeView.h"
#include "VTKImageResliceView.h"
#include "VTKPolyDataView.h"
#include "VTKPicker.h"

int main()
{
    ////体绘制
    //VTKVolumeView volumeView;
    //volumeView.Run();

    ////2D显示
    //VTKImageView imageView;
    //imageView.Run();
    //imageView.RunDicom();
    //imageView.RunDicomUsingImageView2();

    ////2D切片（最常用）
    //VTKImageResliceView resliceView;
    //resliceView.Run();

    ////图元
    //VTKPolyDataView polydataView;
    //polydataView.Run();

    ////拾取
    //VTKPicker picker;
    //picker.Run();

    return 0;
}

