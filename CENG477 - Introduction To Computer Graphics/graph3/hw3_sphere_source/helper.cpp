#include "helper.h"

unsigned int CompileShader(unsigned int type, const std::string& source){
    unsigned int id  = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id , 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        if(type == GL_VERTEX_SHADER){
            std::cout << "Failed to compile " << "VERTEX" << std::endl;
        }
        if(type == GL_FRAGMENT_SHADER){
            std::cout << "Failed to compile " << "FRAGMENT " << std::endl;
        }
        std::cout << message << std::endl;
    }

    return id;
}


void createShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {

    std::string vertexShader;

    if (!readDataFromFile(vertexShaderPath, vertexShader)){
        std::cout << "Cannot find file name: " + vertexShaderPath << std::endl;
        exit(-1);
    }

    const GLchar* gl_vertexShader = (const GLchar*) vertexShader.c_str();

    std::string fragmentShader;

    if (!readDataFromFile(fragmentShaderPath, fragmentShader)){
        std::cout << "Cannot find file name: " + fragmentShaderPath << std::endl;
        exit(-1);
    }

    const GLchar* gl_fragmentShader = (const GLchar*) fragmentShader.c_str();

    idProgramShader = glCreateProgram();
    idVertexShader = CompileShader(GL_VERTEX_SHADER, gl_vertexShader);
    idFragmentShader = CompileShader(GL_FRAGMENT_SHADER, gl_fragmentShader);

    glAttachShader(idProgramShader, idVertexShader);
    glAttachShader(idProgramShader, idFragmentShader);

    glLinkProgram(idProgramShader);

    glValidateProgram(idProgramShader);

    glDeleteShader(idVertexShader);
    glDeleteShader(idFragmentShader);

}

bool readDataFromFile(const string& fileName, string &data) {
    fstream myfile;

    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open()) {
        string curLine;

        while (getline(myfile, curLine)){
            data += curLine;
            if (!myfile.eof())
                data += "\n";
        }

        myfile.close();
    }
    else
        return false;

    return true;
}

void initTexture(char *filename,int *w, int *h, int mode)
{
    int width, height;

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen( filename, "rb" );
    unsigned long location = 0;
    int i = 0, j = 0;

    if ( !infile )
    {
        printf("Error opening jpeg file %s\n!", filename );
        return;
    }
    printf("Texture filename = %s\n",filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error( &jerr );
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress( &cinfo );
    /* this makes the library read from infile */
    jpeg_stdio_src( &cinfo, infile );
    /* reading the image header which contains image information */
    jpeg_read_header( &cinfo, TRUE );
    /* Start decompression jpeg here */
    jpeg_start_decompress( &cinfo );

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
    /* read one scan line at a time */
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    if(mode == 0){ // HEIGHT MAP 
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1,&idJpegHeightMap);
        glBindTexture(GL_TEXTURE_2D, idJpegHeightMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
    }
    else if(mode == 1){ // TEXTURE
        glActiveTexture(GL_TEXTURE0+1);
        glGenTextures(1,&idJpegTexture);
        glBindTexture(GL_TEXTURE_2D, idJpegTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
    }


    *w = width;
    *h = height;

    glGenerateMipmap(GL_TEXTURE_2D);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    free( raw_image );
    fclose( infile );

}
