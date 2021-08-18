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
 * File			: cbuf.h 
 * Author		: Firmware Team member
 * Comments		: Header file which contains cbuf related defines and 
 *              function prototypes.
 * Revision history: 
 *				Created on 19-Aug-2021
 */


 
// Guard condition to prevent multiple file inclusion
#ifndef cbuf_h
#define	cbuf_h

// include processor files - #include <>  -------------------------------------
#include <xc.h> 

// include project files - #include "" ----------------------------------------

// '#' defines ----------------------------------------------------------------
// 'Macros' -------------------------------------------------------------------

// Declarations : Classes -----------------------------------------------------
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

// Declarations : Structure ---------------------------------------------------
typedef struct cbuf_t   {

    uint8_t    rd_idx;
    uint8_t    wr_idx;
    uint8_t    count;
    uint8_t    buf_size;
    char    *  buf_ptr;
    
} CBUF_T;


// Declarations : Unions ------------------------------------------------------
// Declarations : Enums -------------------------------------------------------

// Declarations : Global Variables --------------------------------------------



// Declarations : Functions ---------------------------------------------------
uint8_t cbuf_init       (CBUF_T * cbuf_p, char * buf_ptr, uint8_t buf_size);
uint8_t is_cbuf_empty   (CBUF_T * cbuf_p);
uint8_t is_cbuf_full    (CBUF_T * cbuf_p);
uint8_t cbuf_read       (CBUF_T * cbuf_p);
uint8_t cbuf_write      (CBUF_T * cbuf_p, uint8_t data);
  




#endif	/* cbuf_h */

//------------------------ Scratch Area ---------------------------------------


