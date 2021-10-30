//////////////////////////////////////////////////////////////////
///  Copyright (c) Shanghai United Imaging Healthcare Inc., 2012
///  All rights reserved.
/// 
///  \author    Wang Rui (mailto:rui.wang@united-imaging.com)
/// 
///  \file      mcsf_viewer3d_algo_common_bmp_parser.h
/// 
///  \brief     Declaration of BMPParser
/// 
///  \version   1.0
///  
///  \date      05 13, 2016
////////////////////////////////////////////////////////////////

#ifndef WISDOM_TECH_ALGO_COMMON_BMP_PARSER_H_
#define WISDOM_TECH_ALGO_COMMON_BMP_PARSER_H_

#include <string>

/*
BMP���ļ�ͷ��14Byte��λͼ��Ϣͷ��40Byte����ɫ����Byte����λͼ���ݣ���Byte�����
//////////////////////////////////////////////////////////////////////////
�ļ�ͷ
OFFSET               Count TYPE    Description
0000h                    2 char    ID='BM' - BitMap
OS/2 also supports the following IDs :
ID='BA' - Bitmap Array
ID='CI' - Color Icon
ID='CP' - Color Pointer (mouse cursor)
ID='IC' - Icon
ID='PT' - Pointer (mouse cursor)
0002h                    1 dword   Filesize of whole file(�ļ��Ĵ�С)
0006h                    4 byte    reserved
000Ah                    1 dword   Offset of bitmap in file ="BOF"
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
λͼ��Ϣͷ
000Eh                    1 dword   Length of BitMapInfoHeader
The BitMapInfoHeader starts directly after
this header.
12 - OS/2 1.x format
40 - Windows 3.x format
64 - OS/2 2.x format
0012h                    1 dword   Horizontal width of bitmap in pixels
0016h                    1 dword   Vertical width of bitmap in pixels
001Ah                    1 word    Number of planes
001Ch                    1 word    Bits per pixel ( thus the number of colors )="BPP"
001Eh                    1 dword   Compression type, see ALGRTHMS.txt for description of the different types
0 - none
1 - RLE 8-bit/Pixel
2 - RLE 4-bit/Pixel
0022h                    1 dword   Size of picture in bytes
0026h                    1 dword   Horizontal resolution
002Ah                    1 dword   Vertical resolution
002Eh                    1 dword   Number of used colors
0032h                    1 dword   Number of important colors
//////////////////////////////////////////////////////////////////////////
��ɫ��
0036h                    ? rec     Definition of N colors
N=1 shl "BPP"
1 byte    Blue component
1 byte    Green component
1 byte    Red component
1 byte    Filler
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
λͼ����
"BOF"                    ? byte    Image data
EXTENSION:BMP,RLE,LGO
//////////////////////////////////////////////////////////////////////////
*/

class BMPParser
{
public:
     static char* Load(
        const std::string& strFileName,
        int& uiHeight,
        int& uiWidth);

    //����ֻ�ṩ��8λRGBת������ʣ�������ɫ��24λBMP
    static void WriteRGBImageToBMP(
        const char* szFileName,
        const char* pBits,
        int iWidth,
        int iHeight);
};

#endif
