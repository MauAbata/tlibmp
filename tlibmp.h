/********************************************************************************
The MIT License
Copyright (c) 2017 Yeonji
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************************/

#ifndef TLIBMP_H
#define TLIBMP_H 1

#define TLIBMP_VER "0.1.0"

#include <stdint.h>

#define TLB_OK 0
#define TLB_ERROR -1

/* BMP INFO Struct */
typedef struct {
    uint16_t bfType;
    uint16_t biType;
} bmp_info_t;

/* BMP Header Struct */
typedef struct {
    unsigned char bfType[2];
    unsigned char bfSize[4];
    unsigned char bfReserved1[2];
    unsigned char bfReserved2[2];
    unsigned char bfOffBits[4];
} bmp_header_t;

/* Magic Number in BMP Header (bfType) */
#define WINDOWS_BMP 0X424D       /*BM*/
#define OS2_BITMAP_ARRAY 0X4241  /*BA*/
#define OS2_COLOR_ICON 0X4349    /*CI*/
#define OS2_COLOR_POINTER 0X4350 /*CP*/
#define OS2_ICON 0X4943          /*IC*/
#define OS2_POINTER 0X5054       /*PT*/

/* DIB Header Struct */
typedef struct {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} bmp_stddib_t;

#define DIB_BITMAPCOREHEADER_SIZE 12
#define DIB_OS22XBITMAPHEADER_SIZE 16
#define DIB_BITMAPINFOHEADER_SIZE 40
#define DIB_BITMAPV2INFOHEADER_SIZE 52
#define DIB_BITMAPV3INFOHEADER_SIZE 56
#define DIB_OS22XBITMAPHEADER2_SIZE 64
#define DIB_BITMAPV4HEADER_SIZE 108
#define DIB_BITMAPV5HEADER_SIZE 124

#define COMPRESS_BI_RGB 0
#define COMPRESS_BI_RLE8 1
#define COMPRESS_BI_RLE4 2
#define COMPRESS_BI_BITFIELDS 3
#define COMPRESS_BI_JPEG 4
#define COMPRESS_BI_PNG 5
#define COMPRESS_BI_ALPHABITFIELDS 6
#define COMPRESS_BI_CMYK 11
#define COMPRESS_BI_CMYKRLE8 12
#define COMPRESS_BI_CMYKRLE4 13

#define CHANNEL_R 0
#define CHANNEL_G 1
#define CHANNEL_B 2
#define CHANNEL_A 3

#define CHANNEL_CNT 4
#define CHANNEL_ALL 4

#define TLB_BORDER_EMPTY 0
#define TLB_BORDER_REPLICATE 1

/* tlibmp bitmap struct */
typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t* data; /* Data format: [RGBA]    */
} tlb_image_t;

typedef struct {
    uint8_t size;
    uint8_t bias;
    double div;
    double* data;
} tlb_core_t;

typedef uint32_t color_t;
typedef uint8_t* tlb_pixel_t;

#define tlb_rgb(R, G, B) (color_t)(((uint8_t)R << 0) + ((uint8_t)G << 8) + ((uint8_t)B << 16))
#define tlb_rgba(R, G, B, A)                                                                       \
    (color_t)(((uint8_t)R << 0) + ((uint8_t)G << 8) + ((uint8_t)B << 16) + ((uint8_t)A << 24))

#ifndef __cdecl
#define __cdecl
#endif

/* BMP operation APIs */

/* load a bmp frome file */
tlb_image_t* tlb_load_bmp(const char* file_name);

/* save bmp to file by default settings */
int tlb_save_bmp(tlb_image_t* image, const char* file_name);

/* print basic info of a bmp file */
int tlb_print_bmp_info(const char* file_name);

/* pixel operation APIs */

/* #define tlb_pixel(image, x, y, ch)  ((image->data + 4*(x * image->width + y))[ch]) */

/* get a array of the pixel */
tlb_pixel_t tlb_pixel(tlb_image_t* image, uint32_t x, uint32_t y);

/* print a pixel with specific color */
int tlb_pixel_set(tlb_image_t* image, uint32_t x, uint32_t y, color_t color);

/* print a pixel with specific color */
int tlb_pixel_ch_set(tlb_image_t* image, uint32_t x, uint32_t y, uint8_t channel, uint8_t val);

/* drowing APIs */
int tlb_draw_line(
    tlb_image_t* image, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, color_t color
);

int tlb_draw_triangle(
    tlb_image_t* image,
    uint32_t x0,
    uint32_t y0,
    uint32_t x1,
    uint32_t y1,
    uint32_t x2,
    uint32_t y2,
    color_t color
);

tlb_core_t* tlb_core_new(uint8_t size);
int __cdecl tlb_core_load(tlb_core_t* core, ...);
int tlb_core_standard(tlb_core_t* core);
#define tlb_core_element(core, x, y) core->data[(core->size * y + x)]

/* Image operation APIs */

/* new image */
tlb_image_t* tlb_img_new(uint32_t width, uint32_t height, color_t bgcolor);

/* delete image */
void tlb_img_free(tlb_image_t* image);

/* make a copy of a image */
tlb_image_t* tlb_img_copy(tlb_image_t* image);
tlb_image_t* tlb_img_chop(tlb_image_t* image, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
int tlb_img_paste(tlb_image_t* src, tlb_image_t* dst, uint32_t x0, uint32_t y0);
tlb_image_t* tlb_img_make_border(tlb_image_t* image, uint32_t brsize, uint8_t type);
tlb_image_t* tlb_img_conv_r(tlb_image_t* image, tlb_core_t* core, uint8_t channel);
tlb_image_t* tlb_img_conv(tlb_image_t* image, tlb_core_t* core, uint8_t channel);

int tlb_img_inverse(tlb_image_t* image);
int tlb_img_gray(tlb_image_t* image);
int tlb_img_binary(tlb_image_t* image, uint8_t threshold);

int tlb_img_color_replace(tlb_image_t* image, color_t find, color_t replace);

tlb_image_t* tlb_img_ch_histogram(tlb_image_t* image, uint8_t channel);
tlb_image_t* tlb_img_histogram(tlb_image_t* image);

tlb_image_t* tlb_img_channel(tlb_image_t* image, uint8_t channel);

tlb_image_t* tlb_img_mosaic(tlb_image_t* image, uint32_t granularity);
tlb_image_t* tlb_block_mosaic(
    tlb_image_t* image,
    uint32_t offset_x,
    uint32_t offset_y,
    uint32_t length_x,
    uint32_t length_y,
    uint32_t granularity
);

#endif
