#include "hFramework.h"
#include "hCloudClient.h"

void cfgHandler()
{
	platform.ui.loadHtml({Resource::URL, "/ui.html"});
	platform.ui.video.enable();
}

void ralayTask()
{
	auto& relayPin = hExt.pin4;
	while (1) {
		relayPin.toggle();
		sys.delay(1000);
	}
}

void servoTask()
{
    hServoModule.enablePower();
    
    //move servo right and left
    
    while(1) {
        hServoModule.servo1.setWidth(1700); //set pulse width 1700 us
        sys.delay(1000);
        hServoModule.servo2.setWidth(2200);
        sys.delay(1000);
    }
}

hPIDRegulator myReg;

void setupMotorPID()
{
	float kp = 5;
	float ki = 0.02;
	float kd = 100.0;

	// hPID
	myReg.setScale(1);
	myReg.setKP(kp);
	myReg.setKI(ki);
	myReg.setKD(kd);
	// hRegulator
	myReg.dtMs = 5;
	myReg.stableRange = 30;
	myReg.stableTimes = 3;

	hMotA.attachPositionRegulator(myReg);
	hMotA.resetEncoderCnt();
}

void cloudTask() {
	platform.begin(&RPi);
	//platform.ui.setProjectId("@@@PROJECT_ID@@@");
	platform.ui.setProjectId("2d12e80b1c643841");
	platform.ui.configHandler = cfgHandler;
	
	platform.ui.onKeyEvent = [](KeyEventType type, KeyCode code) {
		if (type == KeyEventType::Pressed) {
			if (code == KeyCode::Key_A) {
				hMotA.rotRel(90); // rotate DC motor with encoder
			}
		}
	};

	while (1) {
		sys.delay(500);
		LED3.toggle();
		auto lb1 = platform.ui.label("l1");
		lb1.setText("uptime %u", sys.getRefTime());
	}
}

void hMain()
{
    sys.taskCreate(cloudTask);
	sys.taskCreate(ralayTask);
	sys.taskCreate(servoTask);
	
	setupMotorPID();
	
    while (1) {
		sys.delay(250);
		LED1.toggle();
	}

}
