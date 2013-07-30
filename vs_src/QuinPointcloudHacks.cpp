//
//  QuinPointcloudHacks.cpp
//

#include "QuinPointcloudHacks.h"
#include "CloudsRGBDVideoPlayer.h"

//#include "CloudsRGBDVideoPlayer.h"
//#ifdef AVF_PLAYER
//#include "ofxAVFVideoPlayer.h"
//#endif

//These methods let us add custom GUI parameters and respond to their events
void QuinPointcloudHacks::selfSetupGui(){

	customGui = new ofxUISuperCanvas("Quin's", gui);
	customGui->copyCanvasStyle(gui);
	customGui->copyCanvasProperties(gui);
	customGui->setName("Quin's");
	customGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	customGui->addSlider("printheadX", 0.0, 1.0, &printheadX);
	customGui->addSlider("printheadY", 0.0, 1.0, &printheadY);
	customGui->addToggle("printEnable", &printEnable);
    customGui->addToggle("printDisableDiscard", &printDisableDiscard);
    customGui->addToggle("shatterEnable", &shatterEnable);
    customGui->addToggle("wavEnable", &wavEnable);
    customGui->addToggle("verticesEnable", &verticesEnable);
    customGui->addToggle("facesEnable", &facesEnable);
    customGui->addToggle("rawEnable", &rawEnable);
	
	ofAddListener(customGui->newGUIEvent, this, &QuinPointcloudHacks::selfGuiEvent);
	
	guis.push_back(customGui);
	guimap[customGui->getName()] = customGui;
}

void QuinPointcloudHacks::selfGuiEvent(ofxUIEventArgs &e){
	if(e.widget->getName() == "Custom Button"){
		cout << "Button pressed!" << endl;
	}
}

//Use system gui for global or logical settings, for exmpl
void QuinPointcloudHacks::selfSetupSystemGui(){
	
}

void QuinPointcloudHacks::guiSystemEvent(ofxUIEventArgs &e){
	
}
//use render gui for display settings, like changing colors
void QuinPointcloudHacks::selfSetupRenderGui(){

}

void QuinPointcloudHacks::guiRenderEvent(ofxUIEventArgs &e){
	
}

// selfSetup is called when the visual system is first instantiated
// This will be called during a "loading" screen, so any big images or
// geometry should be loaded here
void QuinPointcloudHacks::selfSetup(){
    
    printheadX = 0;
    printheadY = 0.32;
    printSpeed = .01;
    printGranularity = .01;
    printEnable = false;

    
    smoothedAudioAmplitude = 0;
	if(ofFile::doesFileExist(getVisualSystemDataPath() + "TestVideo/Jer_TestVideo.mov")){
		getRGBDVideoPlayer().setup(getVisualSystemDataPath() + "TestVideo/Jer_TestVideo.mov",
								   getVisualSystemDataPath() + "TestVideo/Jer_TestVideo.xml" );
		
		getRGBDVideoPlayer().swapAndPlay();
		
		for(int i = 0; i < 640; i += 2){
			for(int j = 0; j < 480; j+=2){
				simplePointcloud.addVertex(ofVec3f(i,j,0));
			}
		}
		
		pointcloudShader.load(getVisualSystemDataPath() + "shaders/rgbdcombined");
		
	}
	
	
//	someImage.loadImage( getVisualSystemDataPath() + "images/someImage.png";
	
}

// selfPresetLoaded is called whenever a new preset is triggered
// it'll be called right before selfBegin() and you may wish to
// refresh anything that a preset may offset, such as stored colors or particles
void QuinPointcloudHacks::selfPresetLoaded(string presetPath){
	
}

// selfBegin is called when the system is ready to be shown
// this is a good time to prepare for transitions
// but try to keep it light weight as to not cause stuttering
void QuinPointcloudHacks::selfBegin(){
	
}

//do things like ofRotate/ofTranslate here
//any type of transformation that doesn't have to do with the camera
void QuinPointcloudHacks::selfSceneTransformation(){
	
}

//normal update call
void QuinPointcloudHacks::selfUpdate(){
    if (printEnable){
        printheadX += printSpeed;
        if (printheadX > 1){
            printheadY += printGranularity;
            printheadX = 0;
        }
    }
}

// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void QuinPointcloudHacks::selfDraw(){
	
    if (printEnable){
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    }
    if (shatterEnable){
        glEnable(GL_DEPTH_TEST);
    }
	ofPushMatrix();
	setupRGBDTransforms();
	pointcloudShader.begin();
    float currAmp = .5;//getRGBDVideoPlayer().getPlayer().getAmplitude();
    if (currAmp > smoothedAudioAmplitude){
        smoothedAudioAmplitude = smoothedAudioAmplitude*.2+currAmp*.8;
    } else {
        smoothedAudioAmplitude = smoothedAudioAmplitude*.99+currAmp*.01;
    }
    //custom variables
    pointcloudShader.setUniform1i("enableFlags", getFlags());
    //3D Print
    pointcloudShader.setUniform1f("printheadWidth", printGranularity);
    pointcloudShader.setUniform2f("printhead", printheadX, printheadY);
    //Shatter
    pointcloudShader.setUniform1f("threshold", 3);
    pointcloudShader.setUniform1f("smoothAudioAmp", smoothedAudioAmplitude);
    pointcloudShader.setUniform1f("audioAmp", MIN(1, MAX(0, ofMap(currAmp, 0, .2, 0, 1))));
    pointcloudShader.setUniform1f("randSeed", ofRandom(1));
	getRGBDVideoPlayer().setupProjectionUniforms(pointcloudShader);
    if (verticesEnable){
        simplePointcloud.drawVertices();
    }
//    simplePointcloud.drawWireframe();
    if (facesEnable){
        simplePointcloud.drawFaces();
    }
    if (rawEnable){
        pointcloudShader.setUniform1i("enableFlags", 0);
        simplePointcloud.drawVertices();
    }
	pointcloudShader.end();
	ofPopMatrix();
//	getRGBDVideoPlayer().getPlayer().getAmplitude();
}

// draw any debug stuff here
void QuinPointcloudHacks::selfDrawDebug(){
	
}
// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
void QuinPointcloudHacks::selfDrawBackground(){

	//turn the background refresh off
	//bClearBackground = false;
	
}
// this is called when your system is no longer drawing.
// Right after this selfUpdate() and selfDraw() won't be called any more
void QuinPointcloudHacks::selfEnd(){
	
	simplePointcloud.clear();
	
}
// this is called when you should clear all the memory and delet anything you made in setup
void QuinPointcloudHacks::selfExit(){
	
}

//events are called when the system is active
//Feel free to make things interactive for you, and for the user!
void QuinPointcloudHacks::selfKeyPressed(ofKeyEventArgs & args){
	
}
void QuinPointcloudHacks::selfKeyReleased(ofKeyEventArgs & args){
	
}

void QuinPointcloudHacks::selfMouseDragged(ofMouseEventArgs& data){
	
}

void QuinPointcloudHacks::selfMouseMoved(ofMouseEventArgs& data){
	
}

void QuinPointcloudHacks::selfMousePressed(ofMouseEventArgs& data){
	
}

void QuinPointcloudHacks::selfMouseReleased(ofMouseEventArgs& data){
	
}