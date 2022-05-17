/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <ofMain.h>
#include "sj_common.h"

/************************************************************
************************************************************/

class ofApp : public ofBaseApp{
private:
	/********************
	********************/
	ofVideoGrabber WebCam;
	
	const int Cam_id;
	const int CamWidth;
	const int CamHeight;
	const int CamFps;
	
	ofFbo fbo;
	ofShader shader;
	
	/****************************************
	****************************************/
	void setup_Gui();
	void print_webCam_listDevices();
	void setup_camera();
	
public:
	/****************************************
	****************************************/
	
	/****************************************
	****************************************/
	ofApp(int _Cam_id, int _CamWidth, int _CamHeight, int _CamFps);
	~ofApp();
	
	void setup_sound();
	void setup();
	void update();
	void draw();
	
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
