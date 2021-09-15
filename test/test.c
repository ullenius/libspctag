#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <spctag.h>

int main( int argc, char **argv )
{
	FILE *spc_file;					// Our SPC file
	char* test_str = "abcdefghijklmnopqrstuvwxyzabcdef-azerty";
	char* test_date = "12/31/1999";
	char *test_num = "999";
	char song_title[SPCTAG_SONGTITLE_LENGTH+1];
	char game_title[SPCTAG_GAMETITLE_LENGTH+1];
	char dumper_name[SPCTAG_DUMPERNAME_LENGTH+1];
	char comments[SPCTAG_COMMENTS_LENGTH+1];
	char dump_date[SPCTAG_DUMPDATE_TXT_LENGTH+1];
	char length[SPCTAG_LENGTH_LENGTH+1];
	char fade_length[SPCTAG_FADELENGTH_BIN_LENGTH+1];
	char artist[SPCTAG_ARTIST_LENGTH+1];
	char channels[9];
	char emulator[20];

	// Exit if no filename is specify
	if ( argc < 2 ) {
		fprintf( stderr, "Usage : %s filname\n", argv[0]);
		exit( 1 );
	}

	// Open file or exit
	if ( ( spc_file = fopen( argv[1], "r+b" ) ) == NULL ) {
		fprintf( stderr, "Unable to open file '%s'!\n", argv[1] );
		exit( 2 );
	}

	spctag_init( spc_file );
	
	printf( "Version : %s\n", LIBSPCTAG_VERSION_STR );
	
	if ( spctag_txt_tag )
		printf( "Tags format: Text\n\n" );
	else
		printf( "Tags format: Binary\n\n" );

	printf( "--- Test reading tags ---\n");

	strcpy( song_title, spctag_get_songtitle() );
	strcpy( game_title, spctag_get_gametitle() );
	strcpy( dumper_name, spctag_get_dumpername() );
	strcpy( comments, spctag_get_comments() );
	strcpy( dump_date, spctag_get_dumpdate() );
	strcpy( length, spctag_get_length() );
	strcpy( fade_length, spctag_get_fadelength() );
	strcpy( artist, spctag_get_artist() );
	strcpy( channels, spctag_get_defaultchannels() );
	strcpy( emulator, spctag_get_emulator() );

	printf("Song title : %s\n", song_title );
	printf("Game title : %s\n", game_title );
	printf("Dumper name : %s\n", dumper_name );
	printf("Comments : %s\n", comments );
	printf("Dump date : %s\n", dump_date );
	printf("Length : %s\n", length );
	printf("Fade length : %s\n", fade_length );
	printf("Artist : %s\n", artist );
	printf("Default channels : %s\n", channels );
	printf("Emulator : %s\n", emulator );

	printf( "\n--- Test writing tags ---\n");

	spctag_set_songtitle( test_str );
	spctag_set_gametitle( test_str );
	spctag_set_dumpername( test_str );
	spctag_set_comments( test_str );
	spctag_set_dumpdate( test_date );
	spctag_set_length( test_num );
	spctag_set_fadelength( test_num );
	spctag_set_artist( test_str );
	spctag_set_defaultchannels( "01010101" );
	spctag_set_emulator( "Snes9x" );

	spctag_save( spc_file );

	printf("Song title : %s\n", spctag_get_songtitle() );
	printf("Game title : %s\n", spctag_get_gametitle() );
	printf("Dumper name : %s\n", spctag_get_dumpername() );
	printf("Comments : %s\n", spctag_get_comments() );
	printf("Dump date : %s\n", spctag_get_dumpdate() );
	printf("Length : %s\n", spctag_get_length() );
	printf("Fade length : %s\n", spctag_get_fadelength() );
	printf("Artist : %s\n", spctag_get_artist() );
	printf("Default channels : %s\n", spctag_get_defaultchannels() );
	printf("Emulator : %s\n", spctag_get_emulator() );

	printf( "\n--- Restor original tags ---\n");

	spctag_set_songtitle( song_title );
	spctag_set_gametitle( game_title );
	spctag_set_dumpername( dumper_name );
	spctag_set_comments( comments );
	spctag_set_dumpdate( dump_date );
	spctag_set_length( length );
	spctag_set_fadelength( fade_length );
	spctag_set_artist( artist );
	spctag_set_defaultchannels( channels );
	spctag_set_emulator( emulator );

	spctag_save( spc_file );

	printf("Song title : %s\n", spctag_get_songtitle() );
	printf("Game title : %s\n", spctag_get_gametitle() );
	printf("Dumper name : %s\n", spctag_get_dumpername() );
	printf("Comments : %s\n", spctag_get_comments() );
	printf("Dump date : %s\n", spctag_get_dumpdate() );
	printf("Length : %s\n", spctag_get_length() );
	printf("Fade length : %s\n", spctag_get_fadelength() );
	printf("Artist : %s\n", spctag_get_artist() );
	printf("Default channels : %s\n", spctag_get_defaultchannels() );
	printf("Emulator : %s\n", spctag_get_emulator() );

	// Close file
	fclose( spc_file );

	return 0;
}
