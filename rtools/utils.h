#ifndef GLUTILS_H
#define GLUTILS_H

#include <GL/glew.h>

#include <gsim/gs_color.h>
#include <gsim/gs_array.h>
#include <gsim/gs_vec.h>
#include <gsim/gs_mat.h>
#include <gsim/gs_image.h>

//====================== GlShader =====================

class GlShader
 { public :
    GLuint id;
   public :
    GlShader ();
   ~GlShader ();
    bool load_and_compile ( GLenum type, const char* filename );
   private :
    bool load ( GLenum type, const char* filename );
    void set ( GLenum type, const char* source );
    bool compile ();
 };

//====================== GlProgram =====================

class GlProgram
 { public :
    GLuint id;
   public :
    static GLint uniform_location ( GLuint pid, const char* varname );
   public :
    GlProgram ();
   ~GlProgram ();
    bool init_and_link ( const GlShader& sh1, const GlShader& sh2 );
    GLint uniform_location ( const char* varname ) const { return uniform_location(id,varname); }
   private :
    void attach ( GLuint shid ) { glAttachShader(id,shid); }
    void attach ( const GlShader& sh ) { glAttachShader(id,sh.id); }
    bool link ();
 };

//====================== GLUtils =====================


class GLUtils{
private:
	
public:
	static int loadImage(const char* fileName);
	static GlProgram* compileShader(const char* vert, const char* frag);
};


// ============== Frustrum ===========================
/*
class Frustrum{
public:
	Frustrum();
	void setFrustum(GsMat m);

};
*/
#endif // GLUTILS_H
