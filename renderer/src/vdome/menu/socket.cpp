#include "socket.h"
namespace vd {
    
//--------------------------------------------------------------
Socket::Socket(){
    enabled = true;
    host = "localhost";
    port = 9092;
	hasEnded = false;
}

//--------------------------------------------------------------
void Socket::setup(){}
    
//--------------------------------------------------------------
void Socket::update(){}
    
 
//--------------------------------------------------------------
void Socket::sendEnd(){
	hasEnded = true;
}
    
//--------------------------------------------------------------
void Socket::loadXML(ofXml &xml) {
    if (xml.exists("socket[@enabled]")) {
        string str = ofToString(xml.getAttribute("socket[@enabled]"));
        if (str == "on") enabled = true;
        else             enabled = false;
    }
    
    if (xml.exists("socket[@host]"))
        host = ofToString( xml.getAttribute("socket[@host]") );
    if (xml.exists("socket[@port]"))
        port = ofToInt( xml.getAttribute("socket[@port]") );
    
    if (enabled)
        setup();
}

//--------------------------------------------------------------
void Socket::saveXML(ofXml &xml) {
    xml.setTo("socket");

    if (enabled) xml.setAttribute("enabled", "on");
    else         xml.setAttribute("enabled", "off");
    
    xml.setAttribute("host", ofToString(host));
    xml.setAttribute("port", ofToString(port));

    xml.setToParent();
}


}//////////////