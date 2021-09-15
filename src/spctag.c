/*
    spctag.c : spctag library
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spctag.h"
#include "constants.h"

// Returned variables
char song_title[SPCTAG_SONGTITLE_LENGTH+1];
char game_title[SPCTAG_GAMETITLE_LENGTH+1];
char dumper_name[SPCTAG_DUMPERNAME_LENGTH+1];
char comments[SPCTAG_COMMENTS_LENGTH+1];
char dump_date[SPCTAG_DUMPDATE_TXT_LENGTH+1];
char length[SPCTAG_LENGTH_LENGTH+1];
char fade_length[SPCTAG_FADELENGTH_TXT_LENGTH+1];
char artist[SPCTAG_ARTIST_LENGTH+1];
char channels[9];
char *emulator;

/*
 * get_formated_channels
 * Transform a byte into string
 * param:
 *     channels: channels encoded on a byte
 *     formated_channels: string composed of 0 and 1 that represent channels states
 */
void get_formated_channels( char channels, char* formated_channels )
{
	unsigned char masque = 0x1 << 7;
	int i;

	for ( i=0; i<=7; i++ ) {
		if ( channels & masque )
			formated_channels[i] = '1';
		else
			formated_channels[i] = '0';

		masque >>= 1;
	}

	formated_channels[i] = '\0';
}

/*
 * set_formated_channels
 * Transform a string composed of 0 and 1 into a byte
 * param:
 *     formated_channels: string composed of 0 and 1 that represent channels states
 *     channels: channels encoded on a byte
 */
void set_formated_channels( char* formated_channels, char* channels )
{
	unsigned char masque;
	int i;

	for ( i=0; i<=7; i++ ) {
		masque = 0x1 << 7-i;
		
		if ( formated_channels[i] == '1' )
			*channels |= masque;
		else {
			masque = ~masque;
			*channels &= masque;
		}
	}
}

/*
 * spctag_is_text
 * Read data and tells if it is only text
 * param:
 *     data: data wich must be test
 * return:
 *     0: at least one byte of data is not considered as text
 *     1: all bytes are considered as text
 */
int spctag_is_text( const char *data )
{
	int i;

	for ( i=0; i<SPCTAG_TEST_LENGTH; i++ ) {
		if ( data[i] != 0 && ( data[i] < 44 || data[i] > 58 ) )
			return 0;
	}

	return 1;
}

int spctag_init( FILE* file )
{
	char header[SPCTAG_FILE_HEADER_LENGTH+1];	// SPC file header
	char has_tag;
	char test_tag[SPCTAG_TEST_LENGTH+1];

	// Return if the file is not a SCP file
	fseek( file, 0, SEEK_SET );
	fread( &header, sizeof( header ) - 1, 1, file );
	header[SPCTAG_FILE_HEADER_LENGTH] = '\0';
	if ( strcmp( header, SPCTAG_FILE_HEADER ) != 0 ) {
		fprintf( stderr, "Not an %s\n", SPCTAG_FILE_HEADER );
		return SPCTAG_E_NOT_SPC;
	}

	// Return if the file header do not contain ID666 tags
	fseek( file, SPCTAG_HAS_TAGS, SEEK_SET );
	has_tag = fgetc( file );
	if ( has_tag != SPCTAG_TAGS_PRESENTS ) {
		printf( "No ID666 tag.\n" );
		return SPCTAG_E_NO_TAGS;
	}

	// Test if we have text or binary tags
	fseek( file, SPCTAG_TEST_START, SEEK_SET );
	fread( &test_tag, SPCTAG_TEST_LENGTH, 1, file );
	spctag_txt_tag = spctag_is_text( test_tag );

	// Init tag struct
	fseek( file, SPCTAG_TAGS_START, SEEK_SET );
	if ( spctag_txt_tag ) {
		spctag_tags_txt = (struct TAGS_TXT *)malloc( sizeof( struct TAGS_TXT ) );
		fread( spctag_tags_txt, sizeof( struct TAGS_TXT ), 1, file );
	} else {
		spctag_tags_bin = (struct TAGS_BIN *)malloc( sizeof( struct TAGS_BIN ) );
		fread( spctag_tags_bin, sizeof( struct TAGS_BIN ), 1, file );
	}

	spctag_is_init = 1;

	return SPCTAG_SUCCESS;
}

char* spctag_get_songtitle()
{
	if ( spctag_txt_tag )
		strncpy( song_title, spctag_tags_txt->song_title, SPCTAG_SONGTITLE_LENGTH );
	else
		strncpy( song_title, spctag_tags_bin->song_title, SPCTAG_SONGTITLE_LENGTH );

	song_title[SPCTAG_SONGTITLE_LENGTH] = '\0';

	return song_title;
}

char* spctag_get_gametitle()
{
	if ( spctag_txt_tag )
		strncpy( game_title, spctag_tags_txt->game_title, SPCTAG_GAMETITLE_LENGTH );
	else
		strncpy( game_title, spctag_tags_bin->game_title, SPCTAG_GAMETITLE_LENGTH );

	game_title[SPCTAG_GAMETITLE_LENGTH] = '\0';

	return game_title;
}

char* spctag_get_dumpername()
{
	if ( spctag_txt_tag )
		strncpy( dumper_name, spctag_tags_txt->dumper_name, SPCTAG_DUMPERNAME_LENGTH );
	else
		strncpy( dumper_name, spctag_tags_bin->dumper_name, SPCTAG_DUMPERNAME_LENGTH );
	
	dumper_name[SPCTAG_DUMPERNAME_LENGTH] = '\0';
	
	return dumper_name;
}

char* spctag_get_comments()
{
	if ( spctag_txt_tag )
		strncpy( comments, spctag_tags_txt->comments, SPCTAG_COMMENTS_LENGTH );
	else
		strncpy( comments, spctag_tags_bin->comments, SPCTAG_COMMENTS_LENGTH );

	comments[SPCTAG_COMMENTS_LENGTH] = '\0';

	return comments;
}

char* spctag_get_dumpdate()
{
	if ( spctag_txt_tag )
		strncpy( dump_date, spctag_tags_txt->dump_date, SPCTAG_DUMPDATE_TXT_LENGTH );
	else {
		if ( spctag_tags_bin->dump_day != 0 && spctag_tags_bin->dump_month != 0 && spctag_tags_bin->dump_year != 0 )
			sprintf( dump_date, "%.2d/%.2d/%.4u", spctag_tags_bin->dump_month, spctag_tags_bin->dump_day, *(unsigned*)spctag_tags_bin->dump_year );
		else
			dump_date[0] = '\0';
	}

	dump_date[SPCTAG_DUMPDATE_TXT_LENGTH] = '\0';

	return dump_date;
}

char* spctag_get_length()
{
	if ( spctag_txt_tag )
		strncpy( length, spctag_tags_txt->length, SPCTAG_LENGTH_LENGTH );
	else {
		unsigned bin_length = 0;
		strncpy( (char*)&bin_length, spctag_tags_bin->length, SPCTAG_LENGTH_LENGTH );
		sprintf( length, "%u", bin_length );
	}

	length[SPCTAG_LENGTH_LENGTH] = '\0';

	return length;
}

char* spctag_get_fadelength()
{
	if ( spctag_txt_tag )
		strncpy( fade_length, spctag_tags_txt->fade_length, SPCTAG_FADELENGTH_TXT_LENGTH );
	else {
		unsigned bin_fadelength = 0;
		strncpy( (char*)&bin_fadelength, spctag_tags_bin->fade_length, SPCTAG_FADELENGTH_BIN_LENGTH );
		sprintf( fade_length, "%u", bin_fadelength );
	}

	fade_length[SPCTAG_FADELENGTH_TXT_LENGTH] = '\0';

	return fade_length;
}

char* spctag_get_artist()
{
	if ( spctag_txt_tag )
		strncpy( artist, spctag_tags_txt->artist, SPCTAG_ARTIST_LENGTH );
	else
		strncpy( artist, spctag_tags_bin->artist, SPCTAG_ARTIST_LENGTH );

	artist[SPCTAG_ARTIST_LENGTH] = '\0';

	return artist;
}

char* spctag_get_defaultchannels()
{	
	if ( spctag_txt_tag )
		get_formated_channels( spctag_tags_txt->default_channels, channels );
	else
		get_formated_channels( spctag_tags_bin->default_channels, channels );

	return channels;
}

char* spctag_get_emulator()
{
	if ( spctag_txt_tag ) {
		switch ( spctag_tags_txt->emulator ) {
			case '1' : emulator = "ZSNES"; break;
			case '2' : emulator = "Snes9x"; break;
			default : emulator = "Unknown";
		}
	} else {
		switch ( (int)spctag_tags_bin->emulator ) {
			case 1 : emulator = "ZSNES"; break;
			case 2 : emulator = "Snes9x"; break;
			default : emulator = "Unknown";
		}
	}

	return emulator;
}

int spctag_set_songtitle( char* new_songtitle )
{
	if ( spctag_txt_tag )
		strncpy( spctag_tags_txt->song_title, new_songtitle, SPCTAG_SONGTITLE_LENGTH );
	else
		strncpy( spctag_tags_bin->song_title, new_songtitle, SPCTAG_SONGTITLE_LENGTH );

	return SPCTAG_SUCCESS;
}

int spctag_set_gametitle( char* new_gametitle )
{
	if ( spctag_txt_tag )
		strncpy( spctag_tags_txt->game_title, new_gametitle, SPCTAG_GAMETITLE_LENGTH );
	else
		strncpy( spctag_tags_bin->game_title, new_gametitle, SPCTAG_GAMETITLE_LENGTH );

	return SPCTAG_SUCCESS;
}

int spctag_set_dumpername( char* new_dumpername )
{
	if ( spctag_txt_tag )
		strncpy( spctag_tags_txt->dumper_name, new_dumpername, SPCTAG_DUMPERNAME_LENGTH );
	else
		strncpy( spctag_tags_bin->dumper_name, new_dumpername, SPCTAG_DUMPERNAME_LENGTH );

	return SPCTAG_SUCCESS;
}

int spctag_set_comments( char* new_comments )
{
	if ( spctag_txt_tag )
		strncpy( spctag_tags_txt->comments, new_comments, SPCTAG_COMMENTS_LENGTH );
	else
		strncpy( spctag_tags_bin->comments, new_comments, SPCTAG_COMMENTS_LENGTH );

	return SPCTAG_SUCCESS;
}

int spctag_set_dumpdate( char* new_dumdate )
{
	if ( spctag_txt_tag )
		strncpy( spctag_tags_txt->dump_date, new_dumdate, SPCTAG_DUMPDATE_TXT_LENGTH );
	else {
		char day_string[3];
		char month_string[3];
		char year_string[5];
		char day, month;
		unsigned year;

		strncpy( day_string, new_dumdate+3, 2 );
		strncpy( month_string, new_dumdate, 2 );
		strncpy( year_string, new_dumdate+6, 4 );

		day = atoi( day_string ) & 0xFF;
		month = atoi( month_string ) & 0xFF;
		year = atoi( year_string );

		spctag_tags_bin->dump_day = day;
		spctag_tags_bin->dump_month = month;
		strncpy( spctag_tags_bin->dump_year, (char*)&year, SPCTAG_DUMPYEAR_BIN_LENGTH );
	}

	return SPCTAG_SUCCESS;
}

int spctag_set_length( char* new_length )
{
	if ( spctag_txt_tag )
		strncpy( spctag_tags_txt->length, new_length, SPCTAG_LENGTH_LENGTH );
	else {
		unsigned length = (unsigned)atoi( new_length );

		if ( length > 0xFFFFFF )
			length = 0xFFFFFF;

		strncpy( spctag_tags_bin->length, (char*)&length, SPCTAG_LENGTH_LENGTH );
	}

	return SPCTAG_SUCCESS;
}

int spctag_set_fadelength( char* new_fadelength )
{
	if ( spctag_txt_tag )
		strncpy( spctag_tags_txt->fade_length, new_fadelength, SPCTAG_FADELENGTH_TXT_LENGTH );
	else {
		unsigned fadelength = (unsigned)atoi( new_fadelength );

		if ( fadelength > 0xFFFFFFFF )
			fadelength = 0xFFFFFFFF;

		strncpy( spctag_tags_bin->fade_length, (char*)&fadelength, SPCTAG_FADELENGTH_BIN_LENGTH);
	}

	return SPCTAG_SUCCESS;
}

int spctag_set_artist( char* new_artist )
{
	if ( spctag_txt_tag )
		strncpy( spctag_tags_txt->artist, new_artist, SPCTAG_ARTIST_LENGTH );
	else
		strncpy( spctag_tags_bin->artist, new_artist, SPCTAG_ARTIST_LENGTH );

	return SPCTAG_SUCCESS;
}

int spctag_set_defaultchannels( char* new_defaultchannels )
{
	if ( spctag_txt_tag ) {
		set_formated_channels( new_defaultchannels, &spctag_tags_txt->default_channels );
	} else {
		set_formated_channels( new_defaultchannels, &spctag_tags_bin->default_channels );
	}
}

int spctag_set_emulator( char* new_emulator )
{
	if ( spctag_txt_tag ) {
		if ( strcasecmp( new_emulator, "ZSNES") == 0 )
			spctag_tags_txt->emulator = '1';
		else if ( strcasecmp( new_emulator, "Snes9x") == 0 )
			spctag_tags_txt->emulator = '2';
		else
			spctag_tags_txt->emulator = '0';
	} else {
		if ( strcasecmp( new_emulator, "ZSNES") == 0 )
			spctag_tags_bin->emulator = 1;
		else if ( strcasecmp( new_emulator, "Snes9x") == 0 )
			spctag_tags_bin->emulator = 2;
		else
			spctag_tags_bin->emulator = 0;
	}
}

int spctag_save( FILE* file )
{
	// Move to the start of ID666 tags
	fseek( file, SPCTAG_TAGS_START, SEEK_SET );

	// Write structures to file
	if ( spctag_txt_tag )
		fwrite( spctag_tags_txt, sizeof(*spctag_tags_txt), 1, file );
	else
		fwrite( spctag_tags_bin, sizeof(*spctag_tags_bin), 1, file );
}

void spctag_free()
{
	if ( spctag_txt_tag )
		free( spctag_tags_txt );
	else
		free( spctag_tags_bin );
}
