/*
    spctag.h : interface of the spctag library
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


#ifndef SPCTAG_H
#define SPCTAG_H 1

/* Library version */
#define LIBSPCTAG_MAJOR		0 /* Major number of library version */
#define LIBSPCTAG_MINOR		2 /* Minor number of library version */

/* Library version (string) */
#define LIBSPCTAG_VERSION_STR	"0.2"

// Field length
#define SPCTAG_SONGTITLE_LENGTH 32
#define SPCTAG_GAMETITLE_LENGTH 32
#define SPCTAG_DUMPERNAME_LENGTH 16
#define SPCTAG_COMMENTS_LENGTH 32
#define SPCTAG_DUMPDATE_TXT_LENGTH 11
#define SPCTAG_DUMPYEAR_BIN_LENGTH 2
#define SPCTAG_LENGTH_LENGTH 3
#define SPCTAG_FADELENGTH_TXT_LENGTH 5
#define SPCTAG_FADELENGTH_BIN_LENGTH 4
#define SPCTAG_ARTIST_LENGTH 32

// Return codes
#define SPCTAG_SUCCESS 0
#define SPCTAG_E_NOT_SPC -1
#define SPCTAG_E_NO_TAGS -2

#define SPCTAG_EXPORT __attribute__ ((visibility ("default")))

/* Represents ASCII tags */
struct TAGS_TXT {
	char song_title[SPCTAG_SONGTITLE_LENGTH];		// Song title
	char game_title[SPCTAG_GAMETITLE_LENGTH];		// Game title
	char dumper_name[SPCTAG_DUMPERNAME_LENGTH];		// Name of dumper
	char comments[SPCTAG_COMMENTS_LENGTH];			// Comments
	char dump_date[SPCTAG_DUMPDATE_TXT_LENGTH];		// Date SPC was dumped (MM/DD/YYYY)
	char length[SPCTAG_LENGTH_LENGTH];			// Number of seconds to play song before fading out
	char fade_length[SPCTAG_FADELENGTH_TXT_LENGTH];		// Length of fade in milliseconds
	char artist[SPCTAG_ARTIST_LENGTH];			// Artist of song
	char default_channels;					// Default channel disables (0 = enable, 1 = disable)
	char emulator;						// Emulator used to dump SPC
	char reserved[45];
} __attribute__((packed)) *spctag_tags_txt;

/* Represents binary tags */
struct TAGS_BIN {
	char song_title[SPCTAG_SONGTITLE_LENGTH];		// Song title
	char game_title[SPCTAG_GAMETITLE_LENGTH];		// Game title
	char dumper_name[SPCTAG_DUMPERNAME_LENGTH];		// Name of dumper
	char comments[SPCTAG_COMMENTS_LENGTH];			// Comments
	char dump_day;						// Day SPC was dumped
	char dump_month;					// Month SPC was dumped
	char dump_year[SPCTAG_DUMPYEAR_BIN_LENGTH];		// Year SPC was dumped
	char unused[7];
	char length[SPCTAG_LENGTH_LENGTH];			// Number of seconds to play song before fading out
	char fade_length[SPCTAG_FADELENGTH_BIN_LENGTH];	// Length of fade in milliseconds
	char artist[SPCTAG_ARTIST_LENGTH];			// Artist of song
	char default_channels;					// Default channel disables (0 = enable, 1 = disable)
	char emulator;						// Emulator used to dump SPC
	char reserved[46];
} __attribute__((packed)) *spctag_tags_bin;

// Globale variables
SPCTAG_EXPORT int spctag_is_init = 0;		// 1 when spctag is initialized and can be used
SPCTAG_EXPORT int spctag_txt_tag = 0;		// 1 if tags are in ASCII format, 0 if tags are in binary format

/*
 * spctag_init
 * Read tags from file.
 * param:
 *     file: SPC file descriptor
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_init( FILE *file );

/*
 * spctag_get_songtitle
 * Return the song title.
 * return:
 *     song title
 */
SPCTAG_EXPORT char* spctag_get_songtitle();

/*
 * spctag_get_gametitle
 * Return the game title.
 * return:
 *     game title
 */
SPCTAG_EXPORT char* spctag_get_gametitle();

/*
 * spctag_get_dumpername
 * Return the dumper name.
 * return:
 *     dumper name
 */
SPCTAG_EXPORT char* spctag_get_dumpername();

/*
 * spctag_get_comments
 * Return comments.
 * return:
 *     comments
 */
SPCTAG_EXPORT char* spctag_get_comments();

/*
 * spctag_get_dumpdate
 * Return the dump date.
 * return:
 * dump date
 */
SPCTAG_EXPORT char* spctag_get_dumpdate();

/*
 * spctag_get_length
 * Return the number of seconds to play song before fading out.
 * return:
 *     song length
 */
SPCTAG_EXPORT char* spctag_get_length();

/*
 * spctag_get_fadelength
 * Return the length of fade in milliseconds.
 * return:
 *     fade length 
 */
SPCTAG_EXPORT char* spctag_get_fadelength();

/*
 * spctag_get_artist
 * Return the artist name.
 * return:
 *     artist name
 */
SPCTAG_EXPORT char* spctag_get_artist();

/*
 * spctag_get_defaultchannels
 * Return channel states.
 * return:
 *     channel states
 */
SPCTAG_EXPORT char* spctag_get_defaultchannels();

/*
 * spctag_get_emulator
 * Return the name of the emulator used to dump.
 * return:
 *     emulator
 */
SPCTAG_EXPORT char* spctag_get_emulator();

/*
 * spctag_set_songtitle
 * Set the song title into tags structure.
 * param:
 *     new_songtitle: new song title
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_set_songtitle( char* new_songtitle );

/*
 * spctag_set_gametitle
 * Set the game title.
 * param:
 *     new_gametitle: new game title
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_set_gametitle( char* new_gametitle );

/*
 * spctag_set_dumpername
 * Set the dumper name.
 * param:
 *     new_dumpername: new dumper name
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_set_dumpername( char* new_dumpername );

/*
 * spctag_set_comments
 * Set comments.
 * param:
 *     new_comments: new comments
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_set_comments( char* new_comments );

/*
 * spctag_set_dumpdate
 * Set the dump date.
 * param:
 *     new_comments: new comments
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_set_dumpdate( char* new_dumdate );

/*
 * spctag_set_length
 * Set the number of seconds to play song before fading out.
 * param:
 *     new_length: new song length
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_set_length( char* new_length );

/*
 * spctag_set_fadelength
 * Set the length of fade in milliseconds.
 * param:
 *     new_fadelength: new fade length
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_set_fadelength( char* new_fadelength );

/*
 * spctag_set_artist
 * Set the artist name.
 * param:
 *     new_artist: new artist name
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_set_artist( char* new_artist );

/*
 * spctag_set_defaultchannels
 * Set channels states.
 * param:
 *     new_defaultchannels: new default channel states
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_set_defaultchannels( char* new_defaultchannels );

/*
 * spctag_set_emulator
 * Set the name of the emulator used to dump.
 * param:
 *     new_emulator: new emulator
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_set_emulator( char* new_emulator );

/*
 * spctag_save
 * Write structures into file.
 * return:
 *     0: succes
 *     <0: error
 */
SPCTAG_EXPORT int spctag_save( FILE* file );

/*
 * spctag_free
 * Free memory used by libspctag.
 */
SPCTAG_EXPORT void spctag_free();

#endif
