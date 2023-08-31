// SIT210 Task 3.2C
// Name: Om Tomar
// Roll No: 2210994882

// Define pins for luminance sensor, LED, and calibration button
#define LUM_PIN          A3   // Pin for the luminance sensor
#define LED_PIN          3   // Pin for the LED indicator
#define CAL_BTN          4   // Pin for the calibration button

// Delay times
#define CAL_SLEEP        30000  // Delay at the end of calibration (time to position the device)
#define READ_DELAY       1000    // 1-second delay between readings
#define LED_BLINK        500     // LED blink interval

// Declare variables
int  calibrated_sun;            // Stored value for direct sun luminance
int  luminance;                 // Current luminance reading

bool calibrated;                // Calibration status flag
bool sun;                       // Sunlight status flag

// Declare functions
void calibrate();              // Function for calibration

void setup() {
    // Set pin modes
    pinMode(LED_PIN, OUTPUT);   // Set LED_PIN as output
    pinMode(CAL_BTN, INPUT);    // Set CAL_BTN as input
    pinMode(LUM_PIN, INPUT);    // Set LUM_PIN as input
    
    sun = false;                 // Initialize sun status as false
    calibrated = false;          // Initialize calibration status as false
}

void loop() {
    luminance = analogRead(LUM_PIN);  // Read luminance value from the sensor
    
    if (!calibrated && digitalRead(CAL_BTN) == HIGH) {
        calibrate();                 // Initiate calibration if button is pressed
    } else if (calibrated) {
        digitalWrite(LED_PIN, HIGH);   // Turn on LED
        
        if (luminance >= calibrated_sun && !sun) {
            Serial.print("Light_Status: ");   // Print the label
            Serial.println("Sun");            // Print the status
            sun = true;
        }
        else if (luminance < calibrated_sun && sun) {
            Serial.print("Light_Status: ");   // Print the label
            Serial.println("Shade");          // Print the status
            sun = false;
        }
    }
    
    delay(READ_DELAY);  // Delay between readings
}

void calibrate() {
    calibrated = true;     // Set calibration status as true
    calibrated_sun = 100000;  // Initialize calibrated_sun with a high value
    
    for (int i = 0; i < 30; i++) {
        luminance = analogRead(LUM_PIN);  // Read luminance value
        
        if (calibrated_sun > luminance) {
            calibrated_sun = luminance;   // Update calibrated_sun if current reading is lower
        }
        
        digitalWrite(LED_PIN, HIGH);      // Turn on LED
        delay(LED_BLINK);                  // LED blink interval
        digitalWrite(LED_PIN, LOW);       // Turn off LED
        delay(LED_BLINK);                  // LED blink interval
    }
    
    Serial.print("Light_Status: ");   // Print the label
    Serial.println("Calibrated OK");  // Print the status
    delay(CAL_SLEEP);  // Delay after calibration
}
