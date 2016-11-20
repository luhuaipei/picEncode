#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <fstream>
#include <string>
#define BIT_DEPTH 8
#define PNG_BYTES_TO_CHECK 1
#define HAVE_ALPHA 1
#define NO_ALPHA 0
#define WIDTH 1920*4  // why width*4
#define HEIGHT 1080
//#define HEIGHT  240
//#define WIDTH ((240*16*199/9/100)*4)
typedef struct _pic_data pic_data;




using namespace std;
struct _pic_data
{
  int width;
  int height;
  int bit_depth;
  int flag;
  char **rgba;
};



pic_data* m_pic_data = new pic_data();

int processBitmap(string bitmapFileName ){
  ifstream bitmap(bitmapFileName.c_str(), ios::binary);
  if(!bitmap) cerr<<"can't open bitmap"<<endl; 

  m_pic_data->width = WIDTH;
  m_pic_data->height = HEIGHT;
  m_pic_data->bit_depth = BIT_DEPTH;
  m_pic_data->flag = 1;
  m_pic_data->rgba = new  char* [HEIGHT];
  char * tmp = new char();
  for(int i =0; i< HEIGHT; i++){
	m_pic_data->rgba[i] = new char [WIDTH];
  }
  
  for(int i = 0; i < HEIGHT ; i++){
    for(int j = 0 ; j < WIDTH ; j+=4){
      for(int k = 0 ;k<4;k++){
	bitmap.read(&(m_pic_data->rgba[i][j+k]),1); 
  //      printf("%d\n", i*j+k) ; 
//        m_pic_data->rgba[i][j+k] = (unsigned char)(*tmp);
      }
    }
  }
  cout<<"After read the bitmap to picdata"<<endl;

}
int generatePNG(pic_data* graph){
 cout<< "width :"<<graph->width<<endl;
cout<< "height :"<<graph->height<<endl;
 cout<<"depth"<<graph->bit_depth<<endl;
  FILE * mypic;
  mypic = fopen("./after.png","wb");
  if(!mypic) 
    {
	cout<<"Error: can't open the file"<<endl;	
        return -1;
    } 
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep * row_pointers;
  png_colorp palette;
  
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
  if(png_ptr == NULL){
	fclose(mypic);
        cout<<"Error:failed png_create_write_struct"<<endl;
	return -1;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if(info_ptr == NULL){
	fclose(mypic);
        cout<<"Error: failed init info_ptr"<<endl;
	return -1;
  }

  if(setjmp(png_jmpbuf(png_ptr)))
  {
        fclose(mypic);
	cout<<"Error: set jmp"<<endl;
        return -1;
  }

  png_init_io(png_ptr,mypic);
  

  if(setjmp(png_jmpbuf(png_ptr)))
  {
        fclose(mypic);
        cout<<"Error: set jmp"<<endl;
	return -1;
  }
  int color_type; 
  if(graph->flag == HAVE_ALPHA) color_type = PNG_COLOR_TYPE_RGB_ALPHA;

  else {
	color_type = PNG_COLOR_TYPE_RGB;
    //    cout<<"Error: wrong type"<<endl;
    //    return -1;
  }
 
  cout<<"color_typei:"<<color_type<<endl;

  cout<< graph->width/4<<" "<<graph->height<<endl;
  png_set_IHDR(png_ptr, info_ptr, graph->width/4, graph -> height,
		graph->bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE,PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  if(setjmp(png_jmpbuf(png_ptr)))
  {
        fclose(mypic);
        cout<<"Error: set jmp"<<endl;
        return -1;
  }

  //wirte bytes
  int temp = 0;
  if(graph->flag == HAVE_ALPHA)  temp = (1 * graph->width);
  else temp = (4* graph->width);

  int pos = 0;
  row_pointers = (png_bytep*)malloc(graph->height*sizeof(png_bytep));
  for(int i = 0; i< graph -> height; i++)
  {
	row_pointers[i] = (png_bytep)malloc(sizeof(unsigned char)*temp);
        for(int j = 0; j < temp ; j += 4){
		row_pointers[i][j]       = graph->rgba[i][j]; //red
		row_pointers[i][j+1]     = graph->rgba[i][j+1]; //green
                row_pointers[i][j+2]     = graph->rgba[i][j+2]; //blue
                if(graph->flag == HAVE_ALPHA)
                {
		    row_pointers[i][j+3] = graph->rgba[i][j+3]; //ALPHP
		}
        }


  }
  
  png_write_image(png_ptr, row_pointers);

  if(setjmp(png_jmpbuf(png_ptr)))
  {
        fclose(mypic);
        cout<<"Error: set jmp"<<endl;
        return -1;
  }


  png_write_end(png_ptr, NULL);
  
  for( int j = 0; j < graph->height; j ++){
	free(row_pointers[j]);
  }
  free(row_pointers);

  fclose(mypic);
  return 0;


}


int main(){
  
 string name = "my_screenshot.bmp";
 processBitmap(name);
 generatePNG(m_pic_data);
}

