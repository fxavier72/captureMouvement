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
    vidGrabber.initGrabber(800,600);
#endif

    // Passe le flux vidéo en niveaux de gris
    grayImage.allocate(800,600);
    // Passe le flux vidéo en différence
    grayDiff.allocate(800,600);

    // Active le flux vidéo en différence
    bLearnBakground = true;
    threshold = 20;
    sender.setup(HOST, PORT);
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
        colorImg.setFromPixels(vidGrabber.getPixels(), 800,600);
#else
        colorImg.setFromPixels(vidPlayer.getPixels(), 800,600);
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

        // On crée les contours
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayDiff, 20, (800*600)/3, 10, true);	// find holes
        if(contourFinder.nBlobs > 0)
        {
            annabelle = contourFinder.blobs[0];
            ofxOscMessage a;
            a.setAddress("/mallarme/annabelle/position");
            a.addFloatArg(blob.centroid.x);
            a.addFloatArg(blob.centroid.y);
            sender.sendMessage(a);
        }
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
    ofRect(360,540,800,600);
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
            ofDrawBitmapString("hole", contourFinder.blobs[i].centroid);

        }
    }

    // finally, a report:
    /*ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "bg subtraction and blob detection" << endl
              << "press ' ' to capture bg" << endl
              << "threshold " << threshold << " (press: +/-)" << endl
              << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
    ofDrawBitmapString(reportStr.str(), 20, 600);*/

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if(key == 'c')
    {
        bLearnBakground = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
