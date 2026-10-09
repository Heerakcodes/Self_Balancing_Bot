#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

// ============================================================
// MOTION COMMANDS
// ============================================================

enum MotionCommand
{
    STOP,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


// ============================================================
// MOTION CONTROLLER
// ============================================================

class MotionController
{
private:

    // --------------------------------------------------------
    // ROBOT PARAMETERS
    // --------------------------------------------------------

    float wheelRadius;
    // Wheel radius in meters.
    // Current wheel radius = 98 mm.
    // CHANGE DURING ACTUAL CALIBRATION.

    float wheelBase;
    // Distance between the centers of the two wheels.
    // CHANGE DURING ACTUAL CALIBRATION.


    // --------------------------------------------------------
    // MOTOR PARAMETERS
    // --------------------------------------------------------

    int motorStepsPerRevolution;
    // NEMA 17 motor = 200 full steps/revolution.

    int microsteps;
    // TB6600 microstepping setting.
    // CHANGE TO MATCH ACTUAL TB6600 DIP SWITCH SETTING.


    // --------------------------------------------------------
    // MOTION LIMITS
    // --------------------------------------------------------

    float maxForwardVelocity;
    // Maximum forward/backward velocity in m/s.
    // CHANGE DURING ACTUAL CALIBRATION.

    float maxTurnRate;
    // Maximum turning rate in rad/s.
    // CHANGE DURING ACTUAL CALIBRATION.

    float acceleration;
    // Maximum linear acceleration in m/s².
    // CHANGE DURING ACTUAL CALIBRATION.

    float turnAcceleration;
    // Maximum turning acceleration in rad/s².
    // CHANGE DURING ACTUAL CALIBRATION.


    // --------------------------------------------------------
    // MOTOR LIMIT
    // --------------------------------------------------------

    int maxStepRate;
    // Maximum step rate in steps/second.
    // CHANGE DURING ACTUAL MOTOR TESTING.


    // --------------------------------------------------------
    // COMMAND
    // --------------------------------------------------------

    MotionCommand currentCommand;


    // --------------------------------------------------------
    // TARGET MOTION
    // --------------------------------------------------------

    float targetVelocity;
    float targetTurnRate;


    // --------------------------------------------------------
    // CURRENT MOTION
    // --------------------------------------------------------

    float currentVelocity;
    float currentTurnRate;


    // --------------------------------------------------------
    // WHEEL REFERENCES
    // --------------------------------------------------------

    float leftWheelVelocity;
    float rightWheelVelocity;


    // --------------------------------------------------------
    // MOTOR REFERENCES
    // --------------------------------------------------------

    int leftStepRate;
    int rightStepRate;


    // --------------------------------------------------------
    // TIMING
    // --------------------------------------------------------

    unsigned long previousTime;


    // --------------------------------------------------------
    // INTERNAL FUNCTIONS
    // --------------------------------------------------------

    void updateWheelReferences();

    int velocityToStepRate(float velocity);


public:

    // --------------------------------------------------------
    // CONSTRUCTOR
    // --------------------------------------------------------

    MotionController();


    // --------------------------------------------------------
    // INITIALIZATION
    // --------------------------------------------------------

    void begin();


    // --------------------------------------------------------
    // COMMAND CONTROL
    // --------------------------------------------------------

    void setCommand(MotionCommand command);


    // --------------------------------------------------------
    // DIRECT MOTION CONTROL
    // --------------------------------------------------------

    void setMotion(
        float forwardVelocity,
        float turnRate
    );


    // --------------------------------------------------------
    // UPDATE
    // --------------------------------------------------------

    void update();


    // --------------------------------------------------------
    // MOTION OUTPUTS
    // --------------------------------------------------------

    float getForwardVelocity();

    float getTurnRate();

    float getLeftWheelVelocity();

    float getRightWheelVelocity();


    // --------------------------------------------------------
    // MOTOR OUTPUTS
    // --------------------------------------------------------

    int getLeftStepRate();

    int getRightStepRate();


    // --------------------------------------------------------
    // COMMAND
    // --------------------------------------------------------

    MotionCommand getCommand();
};

#endif