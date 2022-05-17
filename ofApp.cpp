/************************************************************
************************************************************/
#include <ofApp.h>

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _Cam_id, int _CamWidth, int _CamHeight, int _CamFps)
: Cam_id(_Cam_id)
, CamWidth(_CamWidth)
, CamHeight(_CamHeight)
, CamFps(_CamFps)
{
}

/******************************
******************************/
ofApp::~ofApp()
{
	delete Gui_Global;
	
	if(fp_Log)	fclose(fp_Log);
}


/******************************
******************************/
void ofApp::exit(){
	printf("> Good-bye\n");
	fflush(stdout);
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("test:texture2DRect");
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofSetWindowShape(MAIN_WINDOW_W, MAIN_WINDOW_H);
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	ofSeedRandom();
	
	/********************
	********************/
	fp_Log = fopen("../../../data/Log.csv", "w");
	
	setup_Gui();
	
	fbo.allocate(MAIN_WINDOW_W, MAIN_WINDOW_H, GL_RGBA, 4);
	ClearFbo(fbo);
	
	/********************
	print device list : both sound and camera
	********************/
	print_separatoin();
	
	printf("> List of webCam Devices\n");
	print_webCam_listDevices();
	
	/********************
	********************/
	print_separatoin();
	printf("> setup camera\n");
	fflush(stdout);
	
	setup_camera();
	
	shader.load( "sj_shader/sample.vert", "sj_shader/sample.frag" );
	
	/********************
	********************/
	printf("> start process\n");
	fflush(stdout);
}

/******************************
description
	memoryを確保は、app start後にしないと、
	segmentation faultになってしまった。
******************************/
void ofApp::setup_Gui()
{
	/********************
	********************/
	Gui_Global = new GUI_GLOBAL;
	Gui_Global->setup("param", "gui.xml", 10, 10);
}

/******************************
******************************/
void ofApp::print_webCam_listDevices()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	WebCam.setVerbose(true);
	
	vector< ofVideoDevice > Devices = WebCam.listDevices();// 上 2行がないと、List表示されない.
}

/******************************
******************************/
void ofApp::setup_camera()
{
	/********************
	細かく情報出してもらう
	********************/
	ofSetLogLevel(OF_LOG_VERBOSE);
    WebCam.setVerbose(false); // here, already checked the list.
	
	vector< ofVideoDevice > Devices = WebCam.listDevices();// 上 2行がないと、List表示されない.
	
	/********************
	********************/
	if(Cam_id == -1){
		ofExit();
		return;
	}else{
		if(Devices.size() <= Cam_id) { printf("size = %d\n", Devices.size()); ERROR_MSG(); ofExit(); return; }
		
		WebCam.setDeviceID(Cam_id);
		printf("\n> WebCam set device id = %d\n", Cam_id);
		printf("> DeviceID     = %d\n", Devices[Cam_id].id);
		printf("> CamName      = %s\n", Devices[Cam_id].deviceName.c_str());
		printf("> HardwareName = %s\n", Devices[Cam_id].hardwareName.c_str());
		printf("> serialID     = %s\n", Devices[Cam_id].serialID.c_str());
		
		printf( "\n> Cam size asked  = (%d, %d)\n", int(CamWidth), int(CamHeight) );
		/*
			https://openframeworks.cc/documentation/video/ofVideoGrabber/#show_setDesiredFrameRate
				Set's the desired frame rate of the grabber. This should be called before initGrabber(), which will try to initialize at the desired frame rate. 
				Not all frame rates will be supported, but this at least gives you some abilitity to try grab at different rates.
		*/
		WebCam.setDesiredFrameRate(CamFps);
		// WebCam.initGrabber(CAM_WIDTH, CAM_HEIGHT);
		WebCam.setup(CamWidth, CamHeight, true); // bool ofVideoGrabber::initGrabber(int w, int h, bool setUseTexture){
		printf( "> Cam size actual = (%d, %d)\n\n", int(WebCam.getWidth()), int(WebCam.getHeight()) );
		fflush(stdout);
	}
}

/******************************
******************************/
void ofApp::update(){
	/********************
	********************/
	WebCam.update();
	if(WebCam.isFrameNew()) {
		fbo.begin();
			ofPushMatrix();
				ofTranslate(fbo.getWidth(), 0, 0);
				ofScale(-1, 1, 1);
				
				ofBackground(0, 0, 0, 255);
				ofSetColor(255);
				WebCam.draw(0, 0, fbo.getWidth(), fbo.getHeight());
			
			ofPopMatrix();
		fbo.end();
	}
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	shader.begin();
		shader.setUniform1f( "texture_w", fbo.getWidth() );
		shader.setUniform1f( "texture_h", fbo.getHeight() );
		shader.setUniform1f( "xX", Gui_Global->shader_xX );
		
		ofEnableAntiAliasing();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
		
		ofBackground(0, 0, 0, 255);
		ofSetColor(255);
		
		fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
	shader.end();
	
	/********************
	********************/
	if(Gui_Global->b_Disp){
		/* */
		Gui_Global->gui.draw();
		
		/* */
		ofSetColor(255);
		string info;
		info += "FPS = " + ofToString(ofGetFrameRate(), 2) + "\n";
		ofDrawBitmapString(info, 30, 30);
	}
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		case 'd':
			Gui_Global->b_Disp = !Gui_Global->b_Disp;
			break;
			
		case 'm':
			Gui_Global->gui.minimizeAll();
			break;
	}
}

/******************************
******************************/
void ofApp::keyReleased(int key){
}

/******************************
******************************/
void ofApp::mouseMoved(int x, int y ){

}

/******************************
******************************/
void ofApp::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseEntered(int x, int y){

}

/******************************
******************************/
void ofApp::mouseExited(int x, int y){

}

/******************************
******************************/
void ofApp::windowResized(int w, int h){

}

/******************************
******************************/
void ofApp::gotMessage(ofMessage msg){

}

/******************************
******************************/
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
