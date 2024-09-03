#include "streamDock293.h"

streamDock293::streamDock293(tranSport *transport,struct hid_device_info *devInfo):streamDock(transport,devInfo){

}

unsigned char *streamDock293::getFirmVersion(int lenth){
    return transport->getInputReport(lenth);
}


int streamDock293::setBrightness(int percent){
    return this->transport->setBrightness(percent);
}

int streamDock293::setBackgroundImg(std::string path)
{


   if (this->getFileExtension(path)=="jpg")
   {
        struct jpeg_decompress_struct cinfo;
        int i=0;
        struct jpeg_error_mgr jerr;
        jpeg_create_decompress(&cinfo);
        cinfo.err=jpeg_std_error(&jerr);
        FILE * fd=fopen(path.c_str(),"r");
        if (fd==NULL)
        {
            std::cout<<"文件打开失败"<<"\n";
            return -1;
        }
        jpeg_stdio_src(&cinfo,fd);
        jpeg_read_header(&cinfo,true);
        jpeg_start_decompress(&cinfo);
    
        int size=cinfo.output_width*3*cinfo.output_height;
        unsigned char buffer[size];
        unsigned char buf[513],buf1[cinfo.output_width*3];
        memset(buffer,0,size);

        unsigned char *p;
        while (cinfo.output_scanline<cinfo.output_height)
        {
            memset(buf1,0,cinfo.output_width*3);

            p=buf1;
            jpeg_read_scanlines(&cinfo,&p,1);
            for (int j = 0; j < cinfo.output_width; j++)
            {

                buffer[size-1-i*cinfo.output_width*3-j*3-2]=buf1[j*3+2];
                buffer[size-1-i*cinfo.output_width*3-j*3-1]=buf1[j*3+1];
                buffer[size-1-i*cinfo.output_width*3-j*3]=buf1[j*3];

            }
            i++;
        
        }

        jpeg_finish_decompress(&cinfo);
        this->transport->setBackgroundImg(buffer);

        fclose(fd);
        return 1;
   }else if (this->getFileExtension(path)=="png")
   {

        FILE *fp = fopen(path.c_str(), "rb");
        if (!fp) {
            printf("无法打开输入文件： %s", path.c_str());
            exit(1);
        }

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            printf("创建png结构失败");
            exit(1);
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            printf("创建png信息结构失败");
            exit(1);
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            printf("初始化png结构失败");
            exit(1);
        }

        png_init_io(png_ptr, fp);
        png_read_info(png_ptr, info_ptr);

        int width = png_get_image_width(png_ptr, info_ptr);
        int height = png_get_image_height(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);

        if (bit_depth == 16)
            png_set_strip_16(png_ptr);
        if (color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_palette_to_rgb(png_ptr);
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
            png_set_tRNS_to_alpha(png_ptr);
        if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
            png_set_gray_to_rgb(png_ptr);

        png_read_update_info(png_ptr, info_ptr);

        png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
        for (int y = 0; y < height; y++) {
            row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
        png_read_image(png_ptr, row_pointers);
        int size=width*height*3;
        unsigned char buffer[size];
        memset(buffer,0,size);
        for (int y = 0; y < height; y++) {
            png_byte *row = row_pointers[y];
            for (int x = 0; x < width; x++) {
                png_byte *px = &(row[x * 4]);
                buffer[size-1-y*width*3-x*3]=px[0];
                buffer[size-y*width*3-x*3-1-1]=px[1];
                buffer[size-y*width*3-x*3-3]=px[2];
                
            }
        }

        for (int y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        this->transport->setBackgroundImg(buffer);
    
        return 0;
   }
   
   
   

    
}

unsigned char * streamDock293::read()
{

    
    unsigned char *command=new unsigned char(13);
    if(this->transport->read(command,13)!=-1)
        return command;    
    else
        return NULL;
    
      
}

int streamDock293::setKeyImg(std::string path,int key)
{
    int res;
    if (this->getFileExtension(path)=="jpg")
   {
        struct jpeg_decompress_struct cinfo;
        int i=0;
        struct jpeg_error_mgr jerr;
        jpeg_create_decompress(&cinfo);
        cinfo.err=jpeg_std_error(&jerr);
        FILE * fd=fopen(path.c_str(),"r");
        if (fd==NULL)
        {
            std::cout<<"文件打开失败"<<"\n";
            return -1;
        }
        jpeg_stdio_src(&cinfo,fd);
        jpeg_read_header(&cinfo,true);
        jpeg_start_decompress(&cinfo);
        int width=cinfo.output_width,height=cinfo.output_height;
        int size=cinfo.output_width*3*cinfo.output_height;
        unsigned char buffer[size];
        unsigned char buf1[cinfo.output_width*3];
        memset(buffer,0,size);

        unsigned char *p;
        while (cinfo.output_scanline<cinfo.output_height)
        {
            memset(buf1,0,cinfo.output_width*3);

            p=buf1;
            jpeg_read_scanlines(&cinfo,&p,1);
            for (int j = 0; j < cinfo.output_width; j++)
            {

                buffer[size-1-i*cinfo.output_width*3-j*3-2]=buf1[j*3];
                buffer[size-1-i*cinfo.output_width*3-j*3-1]=buf1[j*3+1];
                buffer[size-1-i*cinfo.output_width*3-j*3]=buf1[j*3+2];

            }
            i++;
        
        }

        jpeg_finish_decompress(&cinfo);
        fclose(fd);
       
       struct jpeg_compress_struct cinfoout;
       jpeg_create_compress(&cinfoout);
       struct jpeg_error_mgr myerr;
       cinfoout.err=jpeg_std_error(&myerr);

       cinfoout.image_width=width;
       cinfoout.image_height=height;
       cinfoout.in_color_space=JCS_RGB;
       cinfoout.input_components=3;
       jpeg_set_defaults(&cinfoout);

       jpeg_set_quality(&cinfoout,75,true);


       FILE * outfile=fopen("Temporary.jpg","w+");

       jpeg_stdio_dest(&cinfoout,outfile);

       jpeg_start_compress(&cinfoout,true);

        JSAMPROW row_pointer[1];
        for(int i=0;i<height;i++){
            row_pointer[0]=(JSAMPROW)(buffer+i*width*3);
            jpeg_write_scanlines(&cinfoout,row_pointer,1);
        }
        jpeg_finish_compress(&cinfoout);
        
        fclose(outfile);
        res = this->transport->setKeyImg("Temporary.jpg",key);
        remove("Temporary.jpg");
   }else if (this->getFileExtension(path)=="png")
   {

        FILE *fp = fopen(path.c_str(), "rb");
        if (!fp) {
            printf("无法打开输入文件： %s", path.c_str());
            exit(1);
        }

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            printf("创建png结构失败");
            exit(1);
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            printf("创建png信息结构失败");
            exit(1);
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            printf("初始化png结构失败");
            exit(1);
        }

        png_init_io(png_ptr, fp);
        png_read_info(png_ptr, info_ptr);

        int width = png_get_image_width(png_ptr, info_ptr);
        int height = png_get_image_height(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);

        if (bit_depth == 16)
            png_set_strip_16(png_ptr);
        if (color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_palette_to_rgb(png_ptr);
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
            png_set_tRNS_to_alpha(png_ptr);
        if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
            png_set_gray_to_rgb(png_ptr);

        png_read_update_info(png_ptr, info_ptr);

        png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
        for (int y = 0; y < height; y++) {
            row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
        png_read_image(png_ptr, row_pointers);
        int size=width*height*3;
        unsigned char buffer[size];
        memset(buffer,0,size);
        for (int y = 0; y < height; y++) {
            png_byte *row = row_pointers[y];
            for (int x = 0; x < width; x++) {
                png_byte *px = &(row[x * 4]);
                buffer[size-1-y*width*3-x*3]=px[0];
                buffer[size-y*width*3-x*3-1-1]=px[1];
                buffer[size-y*width*3-x*3-3]=px[2];
                
            }
        }

        for (int y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        

        struct jpeg_compress_struct cinfoout;
       jpeg_create_compress(&cinfoout);
       struct jpeg_error_mgr myerr;
       cinfoout.err=jpeg_std_error(&myerr);

       cinfoout.image_width=width;
       cinfoout.image_height=height;
       cinfoout.in_color_space=JCS_RGB;
       cinfoout.input_components=3;
       jpeg_set_defaults(&cinfoout);

       jpeg_set_quality(&cinfoout,75,true);


       FILE * outfile=fopen("Temporary.jpg","w+");

       jpeg_stdio_dest(&cinfoout,outfile);

       jpeg_start_compress(&cinfoout,true);

        JSAMPROW row_pointer[1];
        for(int i=0;i<height;i++){
            row_pointer[0]=(JSAMPROW)(buffer+i*width*3);
            jpeg_write_scanlines(&cinfoout,row_pointer,1);
        }
        jpeg_finish_compress(&cinfoout);
        
        fclose(outfile);
        res = this->transport->setKeyImg("Temporary.jpg",key);
    
        res = this->transport->setKeyImg(path,key);
        remove("Temporary.jpg");    
   }    
    
    printf("%d\n",res);
    return res;
}   


