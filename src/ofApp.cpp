#include "ofApp.h"
#define _USE_LIVE_VIDEO
//--------------------------------------------------------------
//--------------------------------------------------------------

void ofApp::setup()
{

#ifdef _USE_LIVE_VIDEO
    vidGrabber.listDevices();
    // On défini la flux vidéo comme actif
    vidGrabber.setVerbose(true);
    // Défini le nombre d'image par seconde
    vidGrabber.setDesiredFrameRate(60);
    // On défini la taille du flux vidéo
    vidGrabber.initGrabber(320,240);
#endif

    // Passe le flux vidéo en niveaux de gris
    grayImage.allocate(320,240);
    // Passe le flux vidéo en différence
    grayDiff.allocate(320,240);

    // Active le flux vidéo en différence
    bLearnBakground = true;
    threshold = 80;
}

//--------------------------------------------------------------
// Permet de faire des mise a jour dans la page de l'application
void ofApp::update()
{
    // Défini la couleur du fond
    ofBackground(100,100,100);

    //Instenciation de l'objet pour la vidéo (false -> inactif)
    bool bNewFrame = false;

// On active l'objet vidéo pour la mise a jour du flux
#ifdef _USE_LIVE_VIDEO
    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
#else
    vidPlayer.update();
    bNewFrame = vidPlayer.isFrameNew();
#endif

// Si la bNewFrame est activé
    if (bNewFrame)
    {

//On défini la taille du flux vidéo
#ifdef _USE_LIVE_VIDEO
        colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
#else
        colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
#endif
        // On transforme l'image couleur en image noir et blanc
        grayImage = colorImg;
        if (bLearnBakground == true)
        {
            grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
            bLearnBakground = false;
        }

        // Prend la valeur de la différence entre le fond et le flux puis on y applique un seuil
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);

        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayDiff, 20, (320*240)/3, 10, true);	// find holes
    }

}

//--------------------------------------------------------------
void ofApp::draw()
{

    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);
    grayImage.draw(360,20);
    grayDiff.draw(360,280);

    // then draw the contours:

    ofFill();
    ofSetHexColor(0x333333);
    ofRect(360,540,320,240);
    ofSetHexColor(0xffffff);

    // we could draw the whole contour finder
    //contourFinder.draw(360,540);

    // or, instead we can draw each blob individually from the blobs vector,
    // this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++)
    {
        contourFinder.blobs[i].draw(360,540);

        // draw over the centroid if the blob is a hole
        ofSetColor(255);
        if(contourFinder.blobs[i].hole)
        {
            ofDrawBitmapString("hole",
                               contourFinder.blobs[i].boundingRect.getCenter().x + 360,
                               contourFinder.blobs[i].boundingRect.getCenter().y + 540);
        }
    }

    // finally, a report:
    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "bg subtraction and blob detection" << endl
              << "press ' ' to capture bg" << endl
              << "threshold " << threshold << " (press: +/-)" << endl
              << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
    ofDrawBitmapString(reportStr.str(), 20, 240);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
