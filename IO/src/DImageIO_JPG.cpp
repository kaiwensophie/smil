/*
 * Copyright (c) 2011-2014, Matthieu FAESSEL and ARMINES
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Matthieu FAESSEL, or ARMINES nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifdef USE_JPEG

#include "IO/include/private/DImageIO.hpp"
#include "IO/include/private/DImageIO_JPG.hpp"
#include "Core/include/DColor.h"

#include <jpeglib.h>

namespace smil
{
    const static size_t JPEG_BUF_SIZE = 16384;
    
    struct my_source_mgr
    {
        struct jpeg_source_mgr pub;
        std::istream* stream;
        JOCTET*       buffer;
    };

    static void my_init_source(j_decompress_ptr cinfo) 
    {
    }
    static boolean my_fill_input_buffer(j_decompress_ptr cinfo) 
    {
        my_source_mgr* src = (my_source_mgr*)cinfo->src;

        src->stream->read((char*)src->buffer, JPEG_BUF_SIZE);
        size_t bytes = src->stream->gcount();
        if (bytes == 0) 
        {
            /* Insert a fake EOI marker */
            src->buffer[0] = (JOCTET) 0xFF;
            src->buffer[1] = (JOCTET) JPEG_EOI;
            bytes = 2;
        }
        src->pub.next_input_byte = src->buffer;
        src->pub.bytes_in_buffer = bytes;
        return TRUE;
    }
    static void my_skip_input_data(j_decompress_ptr cinfo, long num_bytes) 
    {
        my_source_mgr* src = (my_source_mgr*)cinfo->src;
        if (num_bytes > 0) 
        {
        while (num_bytes > (long)src->pub.bytes_in_buffer) 
        {
            num_bytes -= (long)src->pub.bytes_in_buffer;
            my_fill_input_buffer(cinfo);
        }
        src->pub.next_input_byte += num_bytes;
        src->pub.bytes_in_buffer -= num_bytes;
        }
    }
    static void my_term_source(j_decompress_ptr cinfo) 
    {
        // must seek backward so that future reads will start at correct place.
        my_source_mgr* src = (my_source_mgr*)cinfo->src;
        src->stream->clear();
        src->stream->seekg( src->stream->tellg() - (std::streampos)src->pub.bytes_in_buffer );
    }
    
    static void my_set_source_mgr(j_decompress_ptr cinfo, std::istream& stream) 
    {
        my_source_mgr* src;
        if (cinfo->src == 0) 
        {   // first time
            cinfo->src = (struct jpeg_source_mgr *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(my_source_mgr));

            src = (my_source_mgr*) cinfo->src;
            src->buffer = (JOCTET *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT, JPEG_BUF_SIZE*sizeof(JOCTET));
        
        }

        src->stream = &stream;
        src->pub.init_source = my_init_source;
        src->pub.fill_input_buffer = my_fill_input_buffer;
        src->pub.skip_input_data = my_skip_input_data;
        src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
        src->pub.term_source = my_term_source;
        src->pub.bytes_in_buffer = 0;
        src->pub.next_input_byte = 0;
    }
    
    struct my_destination_mgr
    {
        struct jpeg_destination_mgr pub; /* public fields */
        std::ostream* os; /* target stream */
        JOCTET * buffer;       /* start of buffer */
    };

    void my_init_destination (j_compress_ptr cinfo)
    {
        my_destination_mgr* dest = (my_destination_mgr*) cinfo->dest;
        dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE, JPEG_BUF_SIZE * sizeof(JOCTET));
        dest->pub.next_output_byte = dest->buffer;
        dest->pub.free_in_buffer = JPEG_BUF_SIZE;
    }

    boolean my_empty_output_buffer(j_compress_ptr cinfo)
    {
        my_destination_mgr* dest = (my_destination_mgr*)cinfo->dest;

        dest->os->write((const char*)dest->buffer, JPEG_BUF_SIZE);
        if (dest->os->fail()) 
        {
//             LOG_FATAL("Couldn't write entire jpeg buffer to stream.");
        }
        dest->pub.next_output_byte = dest->buffer;
        dest->pub.free_in_buffer = JPEG_BUF_SIZE;
        return TRUE;
    }

    void my_term_destination (j_compress_ptr cinfo)
    {
        my_destination_mgr* dest = (my_destination_mgr*) cinfo->dest;
        size_t datacount = JPEG_BUF_SIZE - dest->pub.free_in_buffer;

        /* Write any data remaining in the buffer */
        if (datacount > 0)
        {
            dest->os->write((const char*)dest->buffer, datacount);
            if (dest->os->fail())
            {
//                 LOG_FATAL("Couldn't write remaining jpeg data to stream.");
            }
        }
        dest->os->flush();
    }

    void my_set_dest_mgr(j_compress_ptr cinfo, std::ostream& os)
    {
        my_destination_mgr* dest;

        if (cinfo->dest == NULL) 
        {  // First time
            cinfo->dest = (struct jpeg_destination_mgr *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(my_destination_mgr));
        }

        dest = (my_destination_mgr*)cinfo->dest;
        dest->pub.init_destination = my_init_destination;
        dest->pub.empty_output_buffer = my_empty_output_buffer;
        dest->pub.term_destination = my_term_destination;
        dest->os = &os;
    }
    
// IplImage* readJpeg(std::istream& is) {
//     IplImage* image = 0;
//     struct jpeg_decompress_struct cinfo; 
//     struct jpeg_error_mgr jerr;
// 
//     // Setup decompression structure
//     cinfo.err = jpeg_std_error(&jerr); 
//     jpeg_create_decompress(&cinfo); 
//     my_set_source_mgr(&cinfo, is);
//     
//     // read info from header.
//     int r = jpeg_read_header(&cinfo, TRUE);
//     if (r != JPEG_HEADER_OK) {
//       LOG_FATAL("Failed to read JPEG header.");
//     } else if (cinfo.num_components != 3 &&
//                cinfo.num_components != 1) {
//       LOG_FATAL("Unsupported number of color components: " 
//                 << cinfo.num_components);
//     } else {
//       jpeg_start_decompress(&cinfo);
//       // resize storage if necessary
//       image = cvCreateImage(cvSize(cinfo.output_width, cinfo.output_height),
//                             IPL_DEPTH_8U, cinfo.output_components);
//       JSAMPARRAY imageBuffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, 
//                                                           cinfo.output_width*cinfo.output_components, 1);
//       for (int y = 0; y < cinfo.output_height; y++) {
//         jpeg_read_scanlines(&cinfo, imageBuffer, 1);
//         uint8_t* dstRow = (uint8_t*)image->imageData + image->widthStep*y;
//         memcpy(dstRow, imageBuffer[0], cinfo.output_width*cinfo.output_components);
//       }
//       // for rgb images, reverse octets
//       if (cinfo.num_components == 3) {
//         cvCvtColor(image, image, CV_RGB2BGR);
//       }
//      
//       jpeg_finish_decompress(&cinfo);
//     }
//     
//     // clean up.
//     jpeg_destroy_decompress(&cinfo); 
// 
//     return image;
//   }

    
    RES_T readJPGHeader(istream &fp, ImageFileHeader &header, jpeg_decompress_struct &cinfo, bool keepAlive)
    {
        /* initialize the JPEG decompression object. */
        jpeg_create_decompress(&cinfo);
        struct jpeg_error_mgr err_mgr;
        cinfo.err = jpeg_std_error(&err_mgr);
        
        /* specify data source (eg, a file) */
        my_set_source_mgr(&cinfo, fp);
        
        /* read file parameters */
        (void) jpeg_read_header(&cinfo, TRUE);
    
        header.width = cinfo.image_width;
        header.height = cinfo.image_height;
        header.channels = cinfo.num_components;
        
        switch(cinfo.data_precision)
        {
          case 8:
            header.scalarType = DtUINT8; 
            break;
          case 16:
            header.scalarType = DtUINT16; 
            break;
        }
        
        switch(cinfo.out_color_space)
        {
          case JCS_RGB:
            header.colorType = ImageFileHeader::COLOR_TYPE_RGB; 
            break;
          case JCS_GRAYSCALE:
            header.colorType = ImageFileHeader::COLOR_TYPE_GRAY; 
            break;
          default:
            header.colorType = ImageFileHeader::COLOR_TYPE_UNKNOWN; 
        }
        
        if (!keepAlive)
        {
            /* destroy cinfo */
            jpeg_destroy_decompress(&cinfo);
        }
        
        return RES_OK;
    }
        
    RES_T writeJPGHeader(ImageFileHeader &header, jpeg_compress_struct &cinfo, ostream &fp, bool keepAlive)
    {
        struct jpeg_error_mgr err_mgr;
        
        /* initialize the JPEG decompression object. */
        jpeg_create_compress(&cinfo);
        cinfo.err = jpeg_std_error(&err_mgr);
        /* specify data source (eg, a file) */
        my_set_dest_mgr(&cinfo, fp);
    
        cinfo.image_width = header.width;      /* image width and height, in pixels */
        cinfo.image_height = header.height;
        cinfo.input_components = 3;     /* # of color components per pixel */
        cinfo.in_color_space = JCS_RGB; /* colorspace of input image */
        jpeg_set_defaults(&cinfo);
        
        switch(header.scalarType)
        {
          case DtUINT8:
            cinfo.data_precision = 8; 
            break;
          case DtUINT16:
            cinfo.data_precision = 16; 
            break;
        }
                
        if (!keepAlive)
        {
            /* destroy cinfo */
            jpeg_destroy_compress(&cinfo);
        }
        
        return RES_OK;
    }
        
    template <>
    RES_T JPG_FileHandler<RGB>::readData(Image<RGB> &image)
    {
        struct jpeg_decompress_struct &cinfo = this->decomp_cinfo;
        struct jpeg_error_mgr err_mgr;
        cinfo.err = jpeg_std_error(&err_mgr);

        size_t width = header.width, height = header.height;
        
        ASSERT(cinfo.data_precision==8 && cinfo.num_components==3, "Not a 24bit RGB image", RES_ERR);
       
        JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, width * cinfo.num_components, 1);

        Image<RGB>::sliceType lines = image.getLines();
        MultichannelArray<UINT8,3>::lineType *arrays;
        
        (void) jpeg_start_decompress(&cinfo);
        
        for (size_t j=0;j<height;j++)
        {
            arrays = lines[j].arrays;
            jpeg_read_scanlines(&cinfo, buffer, 1);
            for (size_t i=0;i<width;i++)
              for (UINT n=0;n<3;n++)
                arrays[n][i] = buffer[0][3*i+n];
        }
        
        (void) jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
//         
//         image.modified();
//         
        return RES_OK;
    }


    
    template <>
    RES_T JPG_FileHandler<RGB>::writeData(const Image<RGB> &image)
    {
        struct jpeg_compress_struct &cinfo = this->comp_cinfo;
        
        struct jpeg_error_mgr err_mgr;
        cinfo.err = jpeg_std_error(&err_mgr);

        size_t width = header.width, height = header.height;

        UINT8 *buffer = new UINT8[width*3];

        Image<RGB>::sliceType lines = image.getLines();
        MultichannelArray<UINT8,3>::lineType *arrays;
        
        (void) jpeg_start_compress(&cinfo, TRUE);
        
        for (size_t j=0;j<height;j++)
        {
            arrays = lines[j].arrays;
            for (size_t i=0;i<width;i++)
              for (UINT n=0;n<3;n++)
                buffer[3*i+n] = arrays[n][i];
            jpeg_write_scanlines(&cinfo, &buffer, 1);
        }
        
        (void) jpeg_finish_compress(&cinfo);
        jpeg_destroy_compress(&cinfo);
        delete[] buffer;
        
        return RES_OK;
    }
    

} // namespace smil

#endif // USE_JPEG
