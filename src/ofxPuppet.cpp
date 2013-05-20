#include "ofxPuppet.h"

#define WML_INSTANTIATE_BEFORE

void ofxPuppet::setup(ofMesh & mesh){
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);	
	originalMesh = mesh, deformedMesh = mesh;
	deformedMesh.clear();
	unsigned int nVerts = originalMesh.getVertices().size();
	
	for ( unsigned int i = 0; i < nVerts; ++i ) {
		ofVec2f vVertex(0,0);
		vVertex = originalMesh.getVertices()[i];
		deformedMesh.addVertex(vVertex);
	}
	
	for ( unsigned int i = 0; i < originalMesh.getIndices().size(); ++i ) {
		int index = originalMesh.getIndices()[i];
		deformedMesh.addIndex(index);
	}
	
	deformer.InitializeFromMesh( &originalMesh );
	needsUpdating = false; 
}

void ofxPuppet::update(){
	if(!needsUpdating) {
		int nConstraints = controlPoints.size();
		set<unsigned int>::iterator cur(controlPoints.begin()), end(controlPoints.end());
		while ( cur != end ) {
			unsigned int nVertex = *cur++;
			ofVec3f vVertex;
			
			vVertex = deformedMesh.getVertices()[nVertex]; //( nVertex, vVertex);
			deformer.SetDeformedHandle( nVertex, ofVec2f( vVertex.x, vVertex.y ) );
		}
		
		deformer.ForceValidation();
		
		needsUpdating = true;
	}
	deformer.UpdateDeformedMesh( &deformedMesh, true );
	vector < ofVec3f > vert = deformedMesh.getVertices();
}

void ofxPuppet::draw(){	
	deformedMesh.draw();
}

void ofxPuppet::drawWireframe() {
	deformedMesh.drawWireframe();
}

void ofxPuppet::setControlPoint(int i) {
	setControlPoint(i, deformedMesh.getVertex(i));
}

void ofxPuppet::setControlPoint(int i, const ofVec2f& position) {
	if (controlPoints.find(i) == controlPoints.end()) {
		controlPoints.insert(i);
	}
	deformedMesh.getVertices()[i].set(position.x, position.y);
	needsUpdating = false; 
}

void ofxPuppet::removeControlPoint(int i) {
	controlPoints.erase(i);
	deformer.RemoveHandle(i);
	needsUpdating = false; 
}

ofMesh& ofxPuppet::getDeformedMesh() {
	return deformedMesh;
}

