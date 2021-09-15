/*
    constants.h : spctag library constants definition
    v0.2 - 2011-09-18
    
    Copyright (C) 2011 Jérôme SONRIER <jsid@emor3j.fr.eu.org>

    This file is part of libspctag.

    libspctag is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libspctag is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libspctag.  If not, see <http://www.gnu.org/licenses/>.
*/


// SCP file header
#define SPCTAG_FILE_HEADER "SNES-SPC700 Sound File Data"
#define SPCTAG_FILE_HEADER_LENGTH 27

// Tags starts at offset 46
#define SPCTAG_TAGS_START 46

// Tags are in text format if the 19 byte at offset 158 are all only text characteres
#define SPCTAG_TEST_START 158 
#define SPCTAG_TEST_LENGTH 19

// File contains ID666 information if byte at offset 35 = 26
#define SPCTAG_HAS_TAGS 35
#define SPCTAG_TAGS_PRESENTS 26
