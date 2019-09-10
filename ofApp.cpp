#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(150, 1);
	for (auto triangle : ico_sphere.getMesh().getUniqueFaces()) {

		auto avg = glm::vec3(triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
		this->location_list.push_back(avg);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	auto base_noise_seed = glm::vec3(ofRandom(10000), ofRandom(10000), ofRandom(10000));
	auto location_noise_seed = glm::vec3(ofRandom(10000), ofRandom(10000), ofRandom(10000));
	auto log = 15;
	auto noise_step = 0.005;
	for (auto& location : this->location_list) {

		glm::vec3 prev_location;
		for (int i = 0; i < log; i++) {

			auto base_location = glm::vec3(ofMap(ofNoise(base_noise_seed.x, (ofGetFrameNum() + i) * noise_step), 0, 1, -150, 150),
				ofMap(ofNoise(base_noise_seed.y, (ofGetFrameNum() + i) * noise_step), 0, 1, -150, 150),
				ofMap(ofNoise(base_noise_seed.z, (ofGetFrameNum() + i) * noise_step), 0, 1, -150, 150));

			auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(location_noise_seed.x, (ofGetFrameNum() + i) * noise_step), 0, 1, -PI, PI), glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(location_noise_seed.y, (ofGetFrameNum() + i) * noise_step), 0, 1, -PI, PI), glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), ofMap(ofNoise(location_noise_seed.z, (ofGetFrameNum() + i) * noise_step), 0, 1, -PI, PI), glm::vec3(0, 0, 1));
			glm::vec3 tmp_location = glm::vec4(location, 0) * rotation_z * rotation_y * rotation_x;
			tmp_location += base_location;

			if (i != 0) {

				ofSetColor(ofMap(i, 0, log, 239, 39));
				ofDrawLine(tmp_location, prev_location);
			}
			prev_location = tmp_location;
		}

		ofFill();
		ofSetColor(39);
		ofDrawSphere(prev_location, 3);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}