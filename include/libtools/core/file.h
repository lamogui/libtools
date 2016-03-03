
/*

    This file is part of libtools.

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

/**
  * THIS FILE IS PART OF LIBTOOLS 
  * SECURITY LEVEL : 8 (CRITICAL)  
  * VISIBILITY     : PROTECTED
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : file.h
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : config.h
  * ROLE         : Des fonctions utiles pour manipuler facilement des 
  *                FILE* de la STL
  */


#ifndef LIBTOOLS_FILE_H
#define LIBTOOLS_FILE_H

#include <libtools/public/config.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

LIBTOOLS_PRIVATE uint64_t fsize(FILE *fp);

#ifdef __cplusplus
}
#endif

#endif
