/* 
 * File:   defines.h
 * Author: e2237709
 *
 * Created on May 25, 2019, 2:15 PM
 */

#ifndef DEFINES_H
#define	DEFINES_H

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

extern unsigned char maze [4][16];


extern unsigned int robotAngle;
extern unsigned char robotDirection;
extern unsigned int xCoordinate;
extern unsigned int yCoordinate;
extern unsigned char xGridBox;
extern unsigned char yGridBox;

extern volatile unsigned char mapShowsFirstColumns;
extern unsigned char receiveBuffer_popPtr;
extern unsigned char rb_pressed;
extern unsigned char unseenBoxes;
extern unsigned char backtrack[32];
extern unsigned char backtrackEnabled;
extern unsigned char backtrackIndex;

extern void turnLeft();
extern void turnRight();
extern void moveForward();
#endif	/* DEFINES_H */

