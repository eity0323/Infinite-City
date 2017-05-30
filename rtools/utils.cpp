#include "utils.h"

#include <fstream>
#include <string>
#include <cerrno>

int GLUtils::loadImage(const char* fileName){
	GsImage image;

	gsuint id;
	if(!image.load(fileName)) std::cout << "COULD NOT LOAD IMAGE!\n"; 
	
	glBindTexture(GL_TEXTURE_2D, 0); 

	glGenTextures(1, &id); 
	glBindTexture(GL_TEXTURE_2D, id); 
	glTexImage2D(GL_TEXTURE_2D, 0, 4, image.w(), image.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

	// set properties: 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
	
	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	
	// generate mip maps: 
	glGenerateMipmap(GL_TEXTURE_2D); 
	
	// free image from CPU memory: 
	image.init(0,0);

	return id;
}
GlProgram* GLUtils::compileShader(const char* vert, const char* frag){
	GlShader vertShader;
	GlShader fragShader;

	GlProgram* prog = new GlProgram();

	vertShader.load_and_compile(GL_VERTEX_SHADER, vert);
	fragShader.load_and_compile(GL_FRAGMENT_SHADER, frag);
	prog->init_and_link(vertShader, fragShader);

	return prog;
}

//================================== GlShader =========================================

GlShader::GlShader ()
 {
   id = 0;
 };

GlShader::~GlShader ()
 {
   if ( id>0 ) glDeleteShader ( id );
 };

bool GlShader::load_and_compile ( GLenum type, const char* filename )
 {
   if ( !load ( type, filename ) ) { std::cout<<"Could not load "<<filename<<"!\n"; return false; }
   if ( !compile() )  { std::cout<<"Could not compile "<<filename<<"!\n"; return false; }
   return true;
}

bool GlShader::load ( GLenum type, const char* filename )
 {
   std::ifstream in ( filename, std::ios::in | std::ios::binary );
   if ( !in.is_open() ) { std::cout<<"Could not load shader ["<<filename<<"] !\n"; gs_exit(1); }

   std::string s;
   in.seekg ( 0, std::ios::end );
   s.resize ( (size_t) in.tellg() );
   in.seekg ( 0, std::ios::beg );
   in.read ( &s[0], s.size() );
   set ( type, &s[0] );
   return true;
}

void GlShader::set ( GLenum type, const char* source )
 {
   if ( id>0 ) glDeleteShader ( id );
   id = glCreateShader ( type );
   glShaderSource ( id, 1, (const GLchar**)&source, NULL );
 }

bool GlShader::compile ()
 {
   GLint compiled;
   if ( !id ) return true;
   glCompileShader ( id );
   glGetShaderiv ( id, GL_COMPILE_STATUS, &compiled );
   if ( !compiled )
    { std::cout << "\nShader Compilation Error:\n";
      GLint size;
      glGetShaderiv ( id, GL_INFO_LOG_LENGTH, &size );
      std::string msg; msg.resize(size);
      glGetShaderInfoLog ( id, size, NULL, &msg[0] );
      std::cout << msg << gsnl;
      return false;
    }
   return true;
 }

//=================================== GlProgram ====================================

GLint GlProgram::uniform_location ( GLuint pid, const char* varname )
 {
   if ( !pid ) std::cout << "Program undefined in uniform location call!\n";
   GLint loc = glGetUniformLocation ( pid, varname );
   if ( loc<0 ) std::cout << "Uniform location for [" << varname << "] not found!\n";
   return loc;
 }

// regular members:

GlProgram::GlProgram ()
 {
   id = 0;
 };

GlProgram::~GlProgram ()
 {
   if ( id ) glDeleteProgram ( id );
 };

bool GlProgram::init_and_link ( const GlShader& sh1, const GlShader& sh2 )
 {
   if ( id ) glDeleteProgram ( id );
   id = glCreateProgram();
   attach ( sh1 );
   attach ( sh2 );
   if ( !link() ) return false;
   return true;
 }

bool GlProgram::link ()
 {
   glLinkProgram ( id );
   GLint linked;
   glGetProgramiv ( id, GL_LINK_STATUS, &linked );
   if ( !linked )
    { std::cout << "\nShader Linker Error:\n";
	  GLint size;
	  glGetProgramiv ( id, GL_INFO_LOG_LENGTH, &size );
      std::string msg; msg.resize(size);
	  glGetProgramInfoLog ( id, size, NULL, &msg[0] );
	  std::cout << msg << gsnl;
	  return false;
    }
   return true;
 }

#undef M