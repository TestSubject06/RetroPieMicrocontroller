class Timer {
  private:
    unsigned long _elapsedMillis;
    unsigned long _targetMillis;
    bool _counting;
    void (*_callback)(void);
  public:
    Timer(){
      _targetMillis = 0;
      _elapsedMillis = 0;
      _counting = false;
    }

    //Resets the timer
    void reset(void){
      _elapsedMillis = 0;
    }

    bool restart(void){
      _elapsedMillis = 0;
      return start();
    }

    //Resets the timer and also adjusts the target millis
    void reset(unsigned long targetMillis){
      _elapsedMillis = 0;
      _targetMillis = targetMillis;
    }
    
    void update(unsigned long elapsedMillis){
      if(_counting){
        _elapsedMillis += elapsedMillis;
        if(_elapsedMillis > _targetMillis){
          reset();
          cancel();
          _callback();
        }
      }
    }
    
    void cancel(void){
      _counting = false;
    }
    
    void setTimer(unsigned long duration, void (*callback)(void)){
      _targetMillis = duration;
      _callback = callback;
    }

    void setTimer(unsigned long duration){
      _targetMillis = duration;
    }
    
    bool isRunning(void){
      return _counting;
    }

    bool start(void){
      if(_callback){
        _counting = true;
      }
      return _counting;
    }
};

unsigned long lastTime;
bool EN;
bool SHUTDOWN;

Timer timers[3];
//Timers:
//0: Pi Boot Timer, holds EN low until 5 seconds of good power have passed.
//1: Brownout Timer, After 5 seconds of no external power, sets SHUTDOWN high.
//2: Final Shutdown Timer, after 20 seconds, sets EN low.

// the setup function runs once when you press reset or power the board
void setup() {
  lastTime = 0;
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(20, INPUT);
  pinMode(11, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.begin(9600);

  EN = false;
  SHUTDOWN = false;
  
  timers[0].setTimer(5000, &powerEnable);
  timers[1].setTimer(5000, &shutdownPi);
  timers[2].setTimer(20000, &powerDisable);
}
 
// the loop function runs over and over again forever
void loop() {
  unsigned long now = millis();
  unsigned long elapsed = now - lastTime;
  lastTime = now;

  //Read the state of the input power supply
  if(!timers[2].isRunning()){
    if(digitalRead(20) == HIGH){
      //If it's high
      if(!timers[0].isRunning() && !EN){
        timers[0].restart();
      }
      if(timers[1].isRunning() && EN){
        timers[1].cancel();
      }
    }else{
      //If it's low
      if(timers[0].isRunning() && !EN){
        timers[0].reset();
      }
      if(!timers[1].isRunning() && EN){
        timers[1].restart();
      }
    }
  }else{
    timers[0].cancel();
    timers[1].cancel();
  }
  
  //Update timers.
  for(int i = 0; i < 3; i++){
    timers[i].update(elapsed);
  }

//  String output = "DigitalRead: ";
//  output = output + (digitalRead(20) == HIGH ? "HIGH" : "LOW");
//  output = output + " EN: ";
//  output = output + (EN ? "TRUE" : "FALSE");
  //Serial.println(output);
}

void powerEnable(void){
  enablePowerToPi();
  EN = true;
}

void powerDisable(void){
  cutPowerToPi();
  unsetShutdown();
  EN = false;
}

void unsetShutdown(void){
  SHUTDOWN = false;
  digitalWrite(13, LOW);
  digitalWrite(6, LOW);
}

void shutdownPi(void){
  SHUTDOWN = true;
  timers[2].start();
  digitalWrite(13, HIGH);
  digitalWrite(6, HIGH);
}

void cutPowerToPi(void){
  //Serial.println("WROTE LOW TO EN");
  digitalWrite(11, LOW);
}

void enablePowerToPi(void){
  //Serial.println("WROTE HIGH TO EN");

  //Spaz the port like an idiot.
  digitalWrite(11, HIGH);
  delay(1);
  digitalWrite(11, LOW);
  delay(1);
  digitalWrite(11, HIGH);
  delay(1);
  digitalWrite(11, LOW);
  delay(1);
  digitalWrite(11, HIGH);
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
