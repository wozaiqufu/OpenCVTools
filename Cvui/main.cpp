#include "cvui.h"
#include <opencv2/opencv.hpp>
using namespace cv;

#define WINDOW_NAME "CVUI"

int main(void)
{
    Mat frame = Mat(450,900,CV_8UC3);
    int count = 0;
    double counterFloat = 0.0;
    double trackBarValue = 0.0;
    bool checked1 = false;
    bool checked2 = true;
    namedWindow(WINDOW_NAME);
    cvui::init(WINDOW_NAME);
    for(;;)
    {
        //fill the frame with a nice color-RGB
        frame = Scalar(40,100,40);
        //show some text
        cvui::text(frame,10,5,"below are all components!");
        //show text with what some algorithms' output
        cvui::printf(frame,10,20,0.4,0x00ff00,"use printf formatting:%d + %.2f = %.2f",2,3.1,5.1);
        //click button and on_button_clicked verified by printf
        if(cvui::button(frame,10,40,"Button"))
        {
            count++;
        }

        cvui::printf(frame,100,48,0.4,0xff0000,"button click count: %d",count);
        //window component
        cvui::window(frame,10,80,200,180,"Window");
        //counter component:int
        cvui::counter(frame,10,300,&count);
        //counter used as doubles default step:0.5,"%.2f"
        cvui::counter(frame,10,320,&counterFloat,0.1);
        //trackBar component
        cvui::trackbar(frame,350,20,300,&trackBarValue,0.,100.);
        cvui::printf(frame,350,90,0.4,0xff000,"current trackbar value is:%.2f",trackBarValue);
        //checkboxes
        cvui::checkbox(frame,350,130,"checkbox",&checked1);
        cvui::checkbox(frame,350,170,"a checked box",&checked2);
        //update must be called after all components
        //it handles mouse click and others behind the scene with magic!
        cvui::update();
        imshow(WINDOW_NAME,frame);
        if(waitKey(20)==27)//press escp to exit the process
        {
            break;
        }
    }
    return 0;
}
