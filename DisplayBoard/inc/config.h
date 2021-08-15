/*
 * MIT License
 *
 * Copyright (c) 2020 T Works Foundation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

/*
 * File			: config.h 
 * Author		: Firmware Team member
 * Comments		: Header file which contains configuration related defines. 
 *              
 * Revision history: 
 *				Created on 12-Aug-2021
 */


 
// Guard condition to prevent multiple file inclusion
#ifndef CONFIG_H
#define	CONFIG_H

// include processor files - #include <>  -------------------------------------
#include <xc.h> 

// include project files - #include "" ----------------------------------------

// '#' defines ----------------------------------------------------------------
/*
 * Hardware Board revisions
 */
#define DISPLAY_BOARD_Rev_1_0   (10)
#define DISPLAY_BOARD_Rev_2_1   (21)
#define DISPLAY_BOARD_Rev_2_2   (22) 

// 1. Select Board Revision ***************************************************
#define DISPLAY_BOARD_Rev       DISPLAY_BOARD_Rev_2_2





//##################------------------------------------#######################
//################## Please Do-NOT Edit Below This Line #######################
//##################------------------------------------#######################

#if DISPLAY_BOARD_Rev == DISPLAY_BOARD_Rev_1_0
#define DISPLAY_BOARD_Rev_string    "Rev - 1.0"

#elif DISPLAY_BOARD_Rev == DISPLAY_BOARD_Rev_2_1
#define DISPLAY_BOARD_Rev_string    "Rev - 2.1"

#elif DISPLAY_BOARD_Rev == DISPLAY_BOARD_Rev_2_2
#define DISPLAY_BOARD_Rev_string    "Rev - 2.2"

#else 
#error    "Please Select appropriate 'Display Board' Hardware revision..!"

#endif



// 'Macros' -------------------------------------------------------------------





#endif	/* CONFIG_H */

//------------------------ Scratch Area ---------------------------------------


