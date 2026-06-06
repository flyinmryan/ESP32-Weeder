#include "WeederLogic.h"

WeederLogic::WeederLogic(PhaseEnableRobot& robot, PhaseEnableRobot& zaxis, 
                         AngularServo& lid, AngularServo& tilt, 
                         AngularServo& roll, AngularServo& swing)
    : _robot(robot), _zaxis(zaxis), _lid(lid), _tilt(tilt), _roll(roll), _swing(swing) {}

void WeederLogic::gradualMove(AngularServo& servo, int targetAngle) {
    int current = servo.getAngle();
    int step = (targetAngle > current) ? 1 : -1;
    while (current != targetAngle) {
        current += step;
        servo.setAngle(current);
        delay(20);
    }
}

void WeederLogic::sunTracker(int q1, int q2, int q3, int q4) {
    // viz1-4 are photoresistor readings (assuming digital active low as in original python logic)
    int viz1 = digitalRead(q1) == LOW ? 1 : 0;
    int viz2 = digitalRead(q2) == LOW ? 1 : 0;
    int viz3 = digitalRead(q3) == LOW ? 1 : 0;
    int viz4 = digitalRead(q4) == LOW ? 1 : 0;

    if ((viz1 == 0 || viz4 == 0) && (viz2 == 1 && viz3 == 1)) {
        gradualMove(_tilt, _tilt.getAngle() - 10);
    }
    if ((viz2 == 0 || viz3 == 0) && (viz1 == 1 && viz4 == 1)) {
        gradualMove(_tilt, _tilt.getAngle() + 10);
    }
    // ... add more logic from python sunTracker if needed ...
}

bool WeederLogic::orientToSun(int targetX, int targetY, int threshold) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) return false;

    // Simplified centroid finding for bright spots
    long sumX = 0, sumY = 0;
    int count = 0;

    // If frame is JPEG, we'd need to decode it or just use GRAYSCALE for this step
    // For simplicity, assume we can get a grayscale buffer or just sample the JPEG (hacky)
    // Best way is to set pixel format to GRAYSCALE temporarily if possible, 
    // but esp_camera_init usually locks it.
    // If we have PIXFORMAT_GRAYSCALE:
    if (fb->format == PIXFORMAT_GRAYSCALE) {
        for (int y = 0; y < fb->height; y++) {
            for (int x = 0; x < fb->width; x++) {
                uint8_t pixel = fb->buf[y * fb->width + x];
                if (pixel > threshold) {
                    sumX += x;
                    sumY += y;
                    count++;
                }
            }
        }
    }

    esp_camera_fb_return(fb);

    if (count > 0) {
        int centerX = sumX / count;
        int centerY = sumY / count;

        int closeEnough = 15;
        if (abs(centerX - targetX) > closeEnough) {
            int xAdjust = (centerX - targetX) / 15;
            gradualMove(_roll, _roll.getAngle() + xAdjust);
        }
        if (abs(centerY - targetY) > closeEnough) {
            int yAdjust = (centerY - targetY) / 15;
            gradualMove(_tilt, _tilt.getAngle() + yAdjust);
        }
        return (abs(centerX - targetX) <= closeEnough && abs(centerY - targetY) <= closeEnough);
    }

    return false;
}

void WeederLogic::moveToWeed(int x) {
    // x coordinate maps to 0-6 array values. x=3 is middle.
    int rollAdjust = (3 - x) * 1.5;
    int tiltAdjust = (3 - x) * 1.5;
    int swingAdjust = (3 - x) * 29;

    gradualMove(_swing, _swing.getAngle() + swingAdjust);
    gradualMove(_roll, _roll.getAngle() + rollAdjust);
    gradualMove(_tilt, _tilt.getAngle() + tiltAdjust);
}
