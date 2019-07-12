#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "hCloudClient.h"

void printfOnConsoleInWebIDE()
{
    for (;;)
    {
        platform.printf("asd %d\r\n", sys.getRefTime());
        sys.delay(1000);
    }
}
void cfgHandler()
{
    auto b = platform.ui.button("btn1");
    b.setText("LED2 control");
}

void onKeyEvent(KeyEventType type, KeyCode code)
{
    //press "up key" on your keyboard in your device UI
    if (code == KeyCode::Up) {
        if (type == KeyEventType::Pressed) {
            for(int i = 0; i<100; i++){
                LED3.on();
                sys.delay(2000);
                LED2.on();
                sys.delay(1000);
                LED3.off();
                sys.delay(2000);
                LED2.off();
                sys.delay(1000);
            }
            
        } else {
            LED3.off();
        }
    }
}
void onButtonEvent(hId id, ButtonEventType type)
{
    if (id == "btn1")
    {
        auto b = platform.ui.button("btn1");
        if (type == ButtonEventType::Pressed) {
            LED2.on();
        } else {
            LED2.off();
        }
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
		LED3.toggle();
		platform.ui.label("l1").setText("uptime %u [ms]", (unsigned int)sys.getRefTime());
	}
}

void hMain()
{
    sys.taskCreate(cloudTask);
    
    for (;;)
    {
        sys.delay(250);
        LED1.toggle();
    }
}