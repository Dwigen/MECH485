/* Code Outline and Inital Code for Mech 458 */
//set CLKPR to 0x00
/* Include Required Files */
/* Define Required Registers */

int stateSwitch(state) {
  switch (state) {
    case 0: /* Pause State, Inital State */
      pause();
    case 1: /* Scan Object State */
      scanObject();
    case 2: /* Change Conveyor Speed */
      updateSpeed();
    case 3: /* Rotate Platform State */
      rotatePlatform();
    case 4: /* Update Display State */
      updateDisplay();
    case 5: /* Update Fifo List */
      updateList();
    case 6: /* Unknown Object */
      unknownObject();
  }
  return 0;
}

/* Determine Item Type */
int scanObject() {

  /*take Reflective sensor input*/

  char type = 'blank';

  float sensorVal = 0.0;
  sensorVal = /*ADC input from Reflective Sensor*/;

  if (sensorVal >= x && sensorVal <= y) {
    type = 'white';
  } else if (sensorVal >= x && sensorVal <= y) {
    type = 'black';
  } else if (sensorVal >= x && sensorVal <= y) {
    type = 'aluminum';
  } else if (sensorVal >= x && sensorVal <= y) {
    type = 'steel';
    } else {
    type = 'unknown';
  }

  if (type = 'unknown') {
    unknownObject();
  } else {
    updateList(type);
  }
  return 0;
}

int unknownObject() {
  /* Sort Remaining Scanned Objects */
  int dist = /*Inital Distance from end of Platform*/
  updateSpeed('reverse');
  int currentmillis = /* Current Millisecond register count */;
  while (dist <= 800) {
    millis = /*millisecond register count*/;
    dist = dist + (/*reverse speed (m/s) */ * ((millis - currentmillis)*1000));
  }
  pause();
  /*Return to
}

int pause() {
  updateSpeed(0);
  rotatePlatform(0);
  updateDisplay();
  while(/*Pause Button Input*/ == 0); /*change to interrupt*/
  return 0;
}
