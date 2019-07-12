#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "hCloudClient.h"

void printfOnConsoleInWebIDE()
{
	for (;;) {
		platform.printf("asd %d\r\n", sys.getRefTime());
		sys.delay(1000);
	}
}

void cfgHandler()
{
	//uncomment if you want to stream video from your project using smartphone
	//platform.ui.video.enableTransmission();
	platform.ui.loadHtml({Resource::WEBIDE, "/ui.html"});
	auto l1 = platform.ui.label("l1");
	auto lb_bat = platform.ui.label("lb_bat");
	auto l2 = platform.ui.label("l2");
	auto b = platform.ui.button("btn1");
}

void onKeyEvent(KeyEventType type, KeyCode code)
{
	//press "up key" on your keyboard in your device UI
	if (code == KeyCode::Up) {
		if (type == KeyEventType::Pressed) {
			LED3.on();
		} else {
			LED3.off();
		}
	}
}

void onButtonEvent(hId id, ButtonEventType type)
{
	static int cnt = 0;
	if (id == "btn1") {
		UiButton b = platform.ui.button("btn1");
		if (type == ButtonEventType::Pressed) {
			b.setText("pressed %u", cnt++);
		} else {
			b.setText("released %u", cnt++);
		}
		LED1.toggle();
	}
}

void cloudTask() {
	platform.begin(&RPi);
	platform.ui.configHandler = cfgHandler;
	platform.ui.onKeyEvent = onKeyEvent;
	platform.ui.onButtonEvent = onButtonEvent;
	platform.ui.setProjectId("@@@PROJECT_ID@@@");
	
	sys.taskCreate(printfOnConsoleInWebIDE);

	while(1) {
		sys.delay(500);
		platform.ui.label("l1").setText("uptime %u", (unsigned int)sys.getRefTime());
		platform.ui.label("lb_bat").setText("%f [V]", sys.getSupplyVoltage());
		LED3.toggle();

	}
}


void hMain()
{
    sys.taskCreate(cloudTask);
    
	sys.setSysLogDev(&devNull); //turn off sys logs
	sys.setLogDev(&Serial);     //default console setup - USB Serial
	Serial.init(115200);        //default baudrate for USB Serial is 460800

	for (;;) {
		sys.delay(500);
		printf("\r\nuptime %u", (unsigned int)sys.getRefTime()); //print on default console - USB Serial
		LED2.toggle();
	}
}
