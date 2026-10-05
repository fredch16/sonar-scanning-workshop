import processing.serial.*;

Serial myPort;

float iAngle = 0;
float iDistance = -1;

float radarX;
float radarY;

final float MAX_DISTANCE_CM = 50;

// Remember the most recent valid detection at each angle.
final int HOLD_MS = 6000;
final int FADE_MS = 3000;
final int PULSE_MS = 700;
float[] detectedDistance = new float[181];
int[] detectedAt = new int[181];
boolean[] hasDetection = new boolean[181];

// Remember sweep positions even when there is no valid echo.
final int SWEEP_FADE_MS = 1200;
int[] sweptAt = new int[181];
boolean[] hasSweep = new boolean[181];

void setup()
{
  fullScreen(P2D);
  smooth(8);

  radarX = width / 2.0;
  radarY = height * 0.97;

  printArray(Serial.list());

  // Change this index if needed
  myPort = new Serial(this, Serial.list()[1], 115200);
  myPort.bufferUntil('\n');

  background(0);
}

void draw()
{
  // Redraw stored detections; their lifetime does not depend on frame rate.
  background(0);

  drawRadar();
  drawSweep();
  drawObject();
}


void serialEvent(Serial port)
{
  String data = port.readStringUntil('\n');

  if (data == null)
    return;

  data = trim(data);

  String[] values = split(data, ',');

  if (values.length != 2)
    return;

  float angle = float(values[0]);
  float distance = float(values[1]);

  if (Float.isNaN(angle) || Float.isNaN(distance) ||
      Float.isInfinite(angle) || Float.isInfinite(distance) ||
      angle < 0 || angle > 180)
    return;

  iAngle = angle;
  iDistance = distance;

  int sweepIndex = round(angle);
  sweptAt[sweepIndex] = millis();
  hasSweep[sweepIndex] = true;

  // Invalid/out-of-range readings add no dot. Old detections age naturally.
  if (distance >= 0 && distance <= MAX_DISTANCE_CM) {
    int index = round(angle);
    detectedDistance[index] = distance;
    detectedAt[index] = millis();
    hasDetection[index] = true;
  }
}


String findSerialPort()
{
  String[] ports = Serial.list();

  // Linux / macOS / Windows common names
  for (int i = 0; i < ports.length; i++) {
    String p = ports[i].toLowerCase();

    if (p.contains("ttyusb") ||
        p.contains("ttyacm") ||
        p.contains("usbserial") ||
        p.contains("wchusb") ||
        p.contains("cu.usb") ||
        p.contains("com")) {
      return ports[i];
    }
  }

  // Fallback if only one port exists
  if (ports.length == 1)
    return ports[0];

  return null;
}


void drawRadar()
{
  pushMatrix();
  translate(radarX, radarY);

  noFill();
  stroke(40, 120, 40);
  strokeWeight(2);

  float radius = height * 0.88;

  // Range arcs
  arc(0, 0, radius * 2.0, radius * 2.0, PI, TWO_PI);
  arc(0, 0, radius * 1.5, radius * 1.5, PI, TWO_PI);
  arc(0, 0, radius * 1.0, radius * 1.0, PI, TWO_PI);
  arc(0, 0, radius * 0.5, radius * 0.5, PI, TWO_PI);

  // Horizontal baseline
  line(-radius, 0, radius, 0);

  // Angle guides
  for (int angle = 30; angle <= 150; angle += 30) {
    line(
      0,
      0,
      radius * cos(radians(angle)),
      -radius * sin(radians(angle))
    );
  }

  popMatrix();
}


void drawSweep()
{
  pushMatrix();
  translate(radarX, radarY);

  float radius = height * 0.88;
  int now = millis();
  strokeWeight(3);

  // Recent green beams fade gradually into a short sonar trail.
  for (int angle = 0; angle <= 180; angle++) {
    if (!hasSweep[angle])
      continue;

    int age = now - sweptAt[angle];
    if (age >= SWEEP_FADE_MS) {
      hasSweep[angle] = false;
      continue;
    }

    float brightness = 1.0 - age / float(SWEEP_FADE_MS);
    stroke(30, 250, 60, 255 * brightness * brightness);
    line(0, 0,
         radius * cos(radians(angle)),
         -radius * sin(radians(angle)));
  }

  popMatrix();
}


void drawObject()
{
  pushMatrix();
  translate(radarX, radarY);

  float radius = height * 0.88;
  int now = millis();

  for (int angle = 0; angle <= 180; angle++) {
    if (!hasDetection[angle])
      continue;

    int age = now - detectedAt[angle];
    if (age >= HOLD_MS + FADE_MS) {
      hasDetection[angle] = false;
      continue;
    }

    float brightness = 1.0;
    if (age > HOLD_MS)
      brightness = 1.0 - (age - HOLD_MS) / float(FADE_MS);

    float pixelDistance = map(detectedDistance[angle], 0, MAX_DISTANCE_CM, 0, radius);
    float x = pixelDistance * cos(radians(angle));
    float y = -pixelDistance * sin(radians(angle));

    // Broad halo, bright 12 px dot, and a white center.
    noStroke();
    fill(255, 60, 60, 35 * brightness);
    ellipse(x, y, 32, 32);
    fill(255, 75, 75, 90 * brightness);
    ellipse(x, y, 22, 22);
    fill(255, 90, 90, 255 * brightness);
    ellipse(x, y, 12, 12);
    fill(255, 255, 255, 255 * brightness);
    ellipse(x, y, 5, 5);

    // A short expanding ring highlights each newly received detection.
    if (age < PULSE_MS) {
      float progress = age / float(PULSE_MS);
      float diameter = 14 + 30 * progress;
      noFill();
      stroke(255, 150, 100, 220 * (1.0 - progress));
      strokeWeight(2);
      ellipse(x, y, diameter, diameter);
    }
  }

  popMatrix();
}
