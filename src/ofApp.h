#pragma once
#include "ofMain.h"
// Inclue la bibliothèque d'openCV
#include "ofxOpenCv.h"
#include "ofxOsc.h"

// Permet d'utiliser la camera en live
#define _USE_LIVE_VIDEO

// Définition de l'hote et du port pour OSC
#define HOST "172.16.69.255"
#define PORT 9876

// Définition de la résolution de la caméra
#define resX 800
#define resY 600

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

		//On défini la caméra live
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
