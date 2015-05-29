#include "ofApp.h"
#define _USE_LIVE_VIDEO
//--------------------------------------------------------------
//--------------------------------------------------------------

void ofApp::setup()
{



#ifdef _USE_LIVE_VIDEO
    /// \brief Définition du flux vidéo
    /// Le flux vidéo est défini par ofVideoGrabber ici appelé grâce a vidGrabber défini dans le fichier ofApp.h
    /// Pas de paramètre
    vidGrabber.listDevices();

    // On défini la flux vidéo comme actif
    /// \brief Flux actif
    /// On défini le flux vidéo comme actif en appellant setVerbose
    /// \param true correspond a l'activation du flux
    vidGrabber.setVerbose(true);

    // Défini le nombre d'image par seconde
    /// \brief Nombre d'image par seconde
    /// L'opition setDesiredFrameRate défini le le nombre d'image par seconde maximum du flux vidéo
    /// \param 60 La fonction admet un entien en paramètre qui correpond au nombre d'image par seconde
    vidGrabber.setDesiredFrameRate(60);

    // On défini la taille du flux vidéo
    /// \brief Résolution de l'image
    /// La fonction initGrabber défini la résolution du flux vidéo par le programme
    /// \param resX Largeur de l'image en pixel
    /// \param resY Hauteur de l'image en pixel
    vidGrabber.initGrabber(resX,resY);
#endif

    // Passe le flux vidéo en niveaux de gris
    /// \brief Flux en niveau de gris
    /// le flux vidéo en niveau de gris et défini par ofxCvGrayscaleImage ici appelé grâce a grayImage défini dans le fichier ofApp.h
    /// La fonction allocate alloue une résolution au flux vidéo transformé
    /// \param resX Largeur de l'image en pixel
    /// \param resY Hauteur de l'image en pixel
    grayImage.allocate(resX,resY);


    // Passe le flux vidéo en différence
    /// \brief Flux en niveau de gris
    /// le flux vidéo en différence et défini par ofxCvGrayscaleImage ici appelé grâce a grayImage défini dans le fichier ofApp.h
    /// La fonction allocate alloue une résolution au flux vidéo transformé
    /// \param resX Largeur de l'image en pixel
    /// \param resY Hauteur de l'image en pixel
    grayDiff.allocate(resX,resY);

    // Adoussicement de la différence
    /// \brief Adoussire l'image
    /// Blur permet l'adoussicement de l'image en différence
    /// \param 11 La fonction admet un entier
    grayDiff.blur(11);

    // Active le flux vidéo en différence
    // bLearnBackground -> prend l'image de ref
    bLearnBakground = true;
    threshold = 20;

    //Envoie des informations sur le protocole OSC
    /// \brief Envoie des information OSC
    /// Permet de définir l'adresse de destination et le port pour l'envoie du paquet OSC
    /// \param HOST Hote de destination défini dans ofApp.h
    /// \param HOST Port défini dans ofApp.h
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
#endif

// Si la bNewFrame est activé
    if (bNewFrame)
    {

//On défini la taille du flux vidéo
#ifdef _USE_LIVE_VIDEO
        colorImg.setFromPixels(vidGrabber.getPixels(), resX,resY);
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
        grayDiff.blur(11);

        // On crée les contours avec en parametre le type d'image qu'on prend la taille mini
        // pour afficher le blob, la taille max le nombre de blob a afficher.
        contourFinder.findContours(grayDiff, 3000, (resX*resY)/3, 2, true);	// find holes
        if(contourFinder.nBlobs > 0)
        {
            annabelle = contourFinder.blobs[0];
            ofxOscMessage a;
            a.setAddress("/mallarme/annabelle/position");
            a.addFloatArg(annabelle.centroid.x/resX);
            a.addFloatArg(annabelle.centroid.y/resY);
            sender.sendMessage(a);

            florence = contourFinder.blobs[1];
            ofxOscMessage f;
            f.setAddress("/mallarme/florence/position");
            f.addFloatArg(florence.centroid.x/resX);
            f.addFloatArg(florence.centroid.y/resY);
            sender.sendMessage(f);
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
    ofRect(360,540,resX,resY);
    ofSetHexColor(0xffffff);

    // we could draw the whole contour finder
    //contourFinder.draw(360,540);

    // or, instead we can draw each blob individually from the blobs vector,
    // this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++)
    {
        // Dessine les contour detecte pour le blob, placement des rectangles (Valeur du draw)
        contourFinder.blobs[i].draw(360,540);

        // draw over the centroid if the blob is a hole
        ofSetColor(255);

        // On identifie les blobs
        if(contourFinder.blobs[0].hole)
        {
            ofDrawBitmapString("anabelle",
                               contourFinder.blobs[0].centroid.x + 360,
                               contourFinder.blobs[0].centroid.y + 540);
        }

        if(contourFinder.blobs[1].hole)
        {
            ofDrawBitmapString("florence",
                               contourFinder.blobs[1].centroid.x + 360,
                               contourFinder.blobs[1].centroid.y + 540);
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
