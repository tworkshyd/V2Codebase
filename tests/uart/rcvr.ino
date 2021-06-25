#include "BoardDefines.h"

void setup() {
  DebugPort.begin(115200);
  DebugPort.print("Start sending characters from Serial Monitor");
}

void loop() {
  if(DebugPort.available()) {
    char data_rcvd = DebugPort.read();
    DebugPort.print(data_rcvd);
  }
}
