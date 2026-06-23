#ifndef MOTORS_H
#define MOTORS_H

#define MOTOR_ENA  33
#define MOTOR_ENB  13
#define MOTOR_IN1  25
#define MOTOR_IN2  26
#define MOTOR_IN3  27
#define MOTOR_IN4  14

#define MOTOR_SPEED       200
#define MOTOR_TURN_SPEED  180
#define CELL_FORWARD_MS  700
#define CELL_BACKWARD_MS  700
#define TURN_90_MS        6100

// Motor B (ENB / IN3 / IN4) is weaker/slower than Motor A (ENA / IN1 / IN2),
// so during straight forward/backward driving it keeps running this many
// extra ms after Motor A stops. Tune this until the robot stops drifting.
#define MOTOR_B_EXTRA_MS  340

void motors_setup();
void motors_forward_one_cell();
void motors_backward_one_cell();
void motors_turn_right_90();
void motors_turn_left_90();
void motors_stop();

#endif
