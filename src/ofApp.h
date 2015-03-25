#pragma once
#include "ofMain.h"
//Inclue la bibliothèque d'openCV
#include "ofxOpenCv.h"
#include "ofxOsc.h"
//Permet d'utiliser la camera en live
#define _USE_LIVE_VIDEO
#define HOST "10.0.1.101"
#define PORT 9876

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//On instancie la caméra live
		#ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#endif

        ofxCvColorImage			colorImg;

        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

		ofxCvBlob   annabelle;
		ofxCvBlob   florence;

        ofxCvContourFinder 	contourFinder;

		int 				threshold;
		bool				bLearnBakground;

		ofxOscSender sender;

};
