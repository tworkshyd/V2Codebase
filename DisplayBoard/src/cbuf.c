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
 * File			: cbuf.c 
 * Author		: Firmware Team member
 * Comments		: Code file which contains cbuf related variables  &  
 *  			function definition.
 * Revision history: 
 *				Created on 19-Aug-2021
 */



// include processor files - #include <>  -------------------------------------
#include <xc.h> 
#include <stdio.h> 

// include project files - #include "" ----------------------------------------
#include "../inc/cbuf.h"


// Local '#' defines ----------------------------------------------------------
// Local 'Macros' -------------------------------------------------------------
									

// Definitions  : Classes -----------------------------------------------------
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

// Definitions  : Structure ---------------------------------------------------
// Definitions  : Unions ------------------------------------------------------
// Definitions  : Enums -------------------------------------------------------

// Definitions  : Global Variables --------------------------------------------



// ISR Definitions ------------------------------------------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Static Function    :
// Summary			  :
// Parameters		  :
// Returns            :
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



// Static Declarations of Variables -------------------------------------------
// Static Declarations of Functions -------------------------------------------

// Static Definitions of Variables --------------------------------------------
// Static Definitions of Functions --------------------------------------------
//=============================================================================
// Static Function    :
// Summary			  :
// Parameters		  :
// Returns            :
//=============================================================================



// Definitions  : Global Functions --------------------------------------------



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t cbuf_init (CBUF_T * cbuf_p, char * buf_ptr, uint8_t buf_size)   {
    
    if (cbuf_p == NULL || buf_ptr == NULL || buf_size == 0) {
        return 0;
    }
    
    cbuf_p->buf_ptr  = buf_ptr;
    cbuf_p->buf_size = buf_size;
    cbuf_p->count    = 0;
    cbuf_p->rd_idx   = 0;
    cbuf_p->wr_idx   = 0;
    
    return 1;
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t is_cbuf_empty (CBUF_T * cbuf_p)   {
    
    
    if (cbuf_p->count == 0)  {
        return 1;    // empty
    }
     
    return 0;       // not empty
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t is_cbuf_full (CBUF_T * cbuf_p)   {
    
    if (cbuf_p->count >= cbuf_p->buf_size)  {
        return 1;    // full
    }
     
    return 0;       // not full
    
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t cbuf_read (CBUF_T * cbuf_p)   {
    
    uint8_t    data;
    
    if (cbuf_p == NULL)
        return 0;
    
    if (cbuf_p->count)  {
        data = cbuf_p->buf_ptr[cbuf_p->rd_idx];
        cbuf_p->count--;
        cbuf_p->rd_idx++;
        if (cbuf_p->rd_idx >= cbuf_p->buf_size) {
            cbuf_p->rd_idx = 0;
        }
        return data;    // success
    }
     
    return 0;           // cbuf is empty
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t cbuf_write (CBUF_T * cbuf_p, uint8_t data)   {
    
    
    if (cbuf_p == NULL)
        return 0;
    
    if (cbuf_p->count < cbuf_p->buf_size)  {
        cbuf_p->buf_ptr[cbuf_p->wr_idx] = data;
        cbuf_p->count++;
        cbuf_p->wr_idx++;
        if (cbuf_p->wr_idx >= cbuf_p->buf_size) {
            cbuf_p->wr_idx = 0;
        }
        return 0;   // cbuf full!!
    }
    
    return 1;       // success
    
}

/* cbuf.c -- ends here..*/



//------------------------ Scratch Area ---------------------------------------

