/**
生成info文件的一条,用于生成ves需要的正样本的info.dat文件
参考 
http://blog.csdn.net/quarryman/article/details/8450387
需要opencv头文件.

编译 make mkinfo

使用(单个)
mkinfo <img>

打开文件 左上角点一下,右下角点一下 任意键盘按键退出(备注1: 注释这里方便批量手动设置)

配合find指令快速生成多个. 
	  find info -name "*.jpg"  | xargs -i ./mkinfo {}


文件默认保存在info.dat文本文件内  INFO_FILE_NAME 宏定义

*/
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>


#define INFO_FILE_NAME "info.dat"
IplImage* src=0;
IplImage* dst=0;
char file[128];
static int xpos[2];
static int ypos[2];
static int t=0;
void on_mouse( int event, int x, int y, int flags, void* ustc)
{
	char temp[16];
	CvPoint pt;
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
		
	if( event == CV_EVENT_MOUSEMOVE )
	{
		cvCopy(dst,src);		
		sprintf(temp,"(%d,%d)",x,y);
		pt = cvPoint(x,y);
		cvPutText(src,temp, pt, &font, cvScalar(255, 255, 255, 0));
		cvCircle( src, pt, 2,cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );
		cvShowImage( "src", src );
		//printf("x %d y %d \n",x,y);
	} else if( event == CV_EVENT_LBUTTONDOWN ) {
		//cvCopy(dst,src);			
		sprintf(temp,"(%d,%d)",x,y);
		pt = cvPoint(x,y);
		cvPutText(src,temp, pt, &font, cvScalar(255, 255, 255, 0));
		cvCircle( src, pt, 2,cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );
		cvCopy(src,dst);
		cvShowImage( "src", src );
		//printf("time = %d\n",t);
		if(t<2){
			xpos[t]=x;
			ypos[t]=y;
		}
		if(t==1){
			FILE* info =fopen(INFO_FILE_NAME,"a");
			// [路径] [图片中包含的人脸(或其他)数量,我这里就限制1了] ,x,y,宽,高
			fprintf(info,"%s 1 %d %d %d %d\n",file,xpos[0],ypos[0],xpos[1]-xpos[0],ypos[1]-ypos[0]);
			fclose(info);
			// 备注1 完成一副图片后直接退出,批量操作很好用.			
			exit(0);
		}
		
		t++;
	} 
}

int main( int argc, char** argv )
{
	src=cvLoadImage(argv[1],1);
	strcpy(file,argv[1]);
	dst=cvCloneImage(src);

	cvNamedWindow("src",1);
	cvSetMouseCallback( "src", on_mouse, 0 );
	
	cvShowImage("src",src);
	cvWaitKey(0); 
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&dst);

	return 0;
}
