#include <cstdio>  
#include <string>  
#include <cstring>  
#include <cstdlib>  
#include "zlib.h"  
#include "zconf.h"  
using namespace std;  
  
bool gzipInflate( const std::string& compressedBytes, std::string& uncompressedBytes ) {  
  if ( compressedBytes.size() == 0 ) {  
    uncompressedBytes = compressedBytes ;  
    return true ;  
  }  
  
  uncompressedBytes.clear() ;  
  
  unsigned full_length = compressedBytes.size() ;  
  unsigned half_length = compressedBytes.size() / 2;  
  
  unsigned uncompLength = full_length ;  
  char* uncomp = (char*) calloc( sizeof(char), uncompLength );  
  
  z_stream strm;  
  strm.next_in = (Bytef *) compressedBytes.c_str();  
  strm.avail_in = compressedBytes.size() ;  
  strm.total_out = 0;  
  strm.zalloc = Z_NULL;  
  strm.zfree = Z_NULL;  
  
  bool done = false ;  
  
  if (inflateInit2(&strm, (16+MAX_WBITS)) != Z_OK) {  
    free( uncomp );  
    return false;  
  }  
  
  while (!done) {  
    // If our output buffer is too small  
    if (strm.total_out >= uncompLength ) {  
      // Increase size of output buffer  
      char* uncomp2 = (char*) calloc( sizeof(char), uncompLength + half_length );  
      memcpy( uncomp2, uncomp, uncompLength );  
      uncompLength += half_length ;  
      free( uncomp );  
      uncomp = uncomp2 ;  
    }  
  
    strm.next_out = (Bytef *) (uncomp + strm.total_out);  
    strm.avail_out = uncompLength - strm.total_out;  
  
    // Inflate another chunk.  
    int err = inflate (&strm, Z_SYNC_FLUSH);  
    if (err == Z_STREAM_END) done = true;  
    else if (err != Z_OK)  {  
      break;  
    }  
  }  
  
  if (inflateEnd (&strm) != Z_OK) {  
    free( uncomp );  
    return false;  
  }  
  
  for ( size_t i=0; i<strm.total_out; ++i ) {  
    uncompressedBytes += uncomp[ i ];  
  }  
  free( uncomp );  
  return true ;  
}  
  
/* Reads a file into memory. */  
bool loadBinaryFile( const std::string& filename, std::string& contents ) {  
  // Open the gzip file in binary mode  
  FILE* f = fopen( filename.c_str(), "rb" );  
  if ( f == NULL )  
    return false ;  
  
  // Clear existing bytes in output vector  
  contents.clear();  
  
  // Read all the bytes in the file  
  int c = fgetc( f );  
  while ( c != EOF ) {  
    contents +=  (char) c ;  
    c = fgetc( f );  
  }  
  fclose (f);  
  
  return true ;  
}  
  
int main() {  
  // Read the gzip file data into memory  
  std::string fileData ;  
  if ( !loadBinaryFile( "inputtext.tar.gz", fileData ) ) {  
    printf( "Error loading input file." );  
    return 0 ;  
  }  
  
  // Uncompress the file data  
  std::string data ;  
  if ( !gzipInflate( fileData, data ) ) {  
    printf( "Error decompressing file." );  
    return 0 ;  
  }  
  
  // Print the data  
  printf( "Data: \"" );  
  for ( size_t i=0; i<data.size(); ++i ) {  
    printf( "%c", data[i] );  
  }  
  printf ( "\"\n" );  
  return 0;  
}  