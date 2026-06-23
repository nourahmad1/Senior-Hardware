#ifndef POSITION_H
#define POSITION_H

#define POS_TOLERANCE_CM  3.0f
#define POS_NUDGE_MS      80
#define POS_MAX_ATTEMPTS  15

bool position_verify(int col, int row);

#endif
