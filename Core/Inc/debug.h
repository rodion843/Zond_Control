/*
 * debug.h
 *
 *  Created on: Jun 2, 2021
 *      Author: user0051
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

typedef enum{
	Debug_None,
	Debug_one,
	Debug_two,
}Debug;
Debug debug;
void DebugTask(void* arg);

#endif /* INC_DEBUG_H_ */
