// requires an Atmega168 chip
#include <math.h>

// audio out to speaker or amp
#define outpin 8

int ptime;
int k, x, dur, freq, t;
int i, j;

// variable for pow pitchShift routine
float ps;
float noteval;

// note values for two octave scale
// divide them by powers of two to generate other octaves
float A     = 14080;
float AS    = 14917.2;
float B     = 15804.3;
float C     = 16744;
float CS    = 17739.7;
float D     = 18794.5;
float DS    = 19912.1;
float E     = 21096.2;
float F     = 22350.6;
float FS    = 23679.6;
float G     = 25087.7;
float GS    = 26579.5;
float A2    = 28160;
float A2S   = 29834.5;
float B2    = 31608.5;
float C2    = 33488.1;
float C2S   = 35479.4;
float D2    = 37589.1;
float D2S   = 39824.3;
float E2    = 42192.3;
float F2    = 44701.2;
float F2S   = 47359.3;
float G2    = 50175.4;
float G2S   = 53159;
float A3    = 56320;

// octaves - corresponds to piano octaves
float oct8 = 4;
float oct7 = 8;
float oct6 = 16;
float oct5 = 32;
float oct4 = 64;
float oct3 = 128;
float oct2 = 256;
float oct1 = 512;
float oct0 = 1024;

// rhythm values
int wh = 1024;
int h = 512;
int dq = 448;
int q = 256;
int qt = 170;
int de = 192;
int e = 128;
int et = 85;
int dsx = 96;
int sx = 64;
int thx = 32;

// major scale just for demo, hack this
float majScale[] = { A, B, CS, D, E, FS, GS, A2, B2, C2S, D2, E2, F2S, G2S, A3 };

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (i = 0; i <= 11; i++) {
    // choose new transpose interval every loop
    ps = (float) i / 12;

    for (x = 0; x <= 15; x++) {
      // transpose scale up 12 tones
      // pow function generates transposition
      // eliminate " * pow(2,ps) " to cut out transpose routine
      noteval = (majScale[x] / oct4) * pow(2, ps);

      dur = 100;

      freqout((int) noteval, dur);

      delay(10);
    }
  }
}

// freq in hz, t in ms
void freqout(int freq, int t) {
  // Calculate 1/2 period in us
  int hperiod;
  long cycles, i;

  // Turn on output pin
  pinMode(outpin, OUTPUT);

  // Subtract 7 us to make up for digitalWrite overhead
  hperiod = (500000 / freq) - 7;

  // Calculate cycles
  cycles = ((long)freq * (long)t) / 1000;

  Serial.print(freq);
  Serial.print("\t");
  Serial.print(hperiod);
  Serial.print("\t");
  Serial.println(cycles);

  // Play note for t ms
  for (i = 0; i <= cycles; i++) {
    digitalWrite(outpin, HIGH);
    delayMicroseconds(hperiod);
    digitalWrite(outpin, LOW);
    // - 1 to make up for digitaWrite overhead
    delayMicroseconds(hperiod - 1);
  }

  // shut off pin to avoid noise from other operations
  pinMode(outpin, INPUT);
}
