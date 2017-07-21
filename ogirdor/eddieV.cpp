//Modified by: Eddie Velasco
//Date: June 20, 2017
//Purpose: Group Project source code
//
// Source code for generating level from 
// background, tiles, and foreground. Also
// will include functionality for offscreen
// death falls, power-ups, health bar display
// and end of level completion
//
// ============================================
// Week 4 Progress
// --------------
// Added Background, Platform w/Textures
// ============================================
// Week 5 Progress
// --------------
// Added Tile System
// Modified to act as Base + Platforms
// ============================================
// Week 6 Progress
// --------------
// Added Time and Health Bar
// ============================================
// Week 7 Progress
// --------------
// Added new Textures
// & more Level Design with .txt
// ============================================
// Week 8 Progress
//
//=============================================

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "ppm.h"
#include "fonts.h"
#include "game.h"
extern void tileCollision(Vec *, int, int);

void renderBackground()
{
//render background for level
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	glBindTexture(GL_TEXTURE_2D, gl.backgroundTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(gl.xc[0], gl.yc[1]); glVertex2i(0,0);
		glTexCoord2f(gl.xc[0], gl.yc[0]); glVertex2i(0, gl.yres);
		glTexCoord2f(gl.xc[1], gl.yc[0]); glVertex2i(gl.xres, gl.yres);
		glTexCoord2f(gl.xc[1], gl.yc[1]); glVertex2i(gl.xres, 0);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void renderPlatform()
{
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	glBindTexture(GL_TEXTURE_2D, gl.platformTexture);
	float x = gl.xres;
	float y = 100;
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex2i(-x,-y);
		glTexCoord2f(0.0, 0.0); glVertex2i(-x, y);
		glTexCoord2f(1.0, 0.0); glVertex2i(x, y);
		glTexCoord2f(1.0, 1.0); glVertex2i(x,-y);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void renderTiles()
{
	//
	//----------------------------
	//Render the level tile system
	//----------------------------
	Flt dd = lev.ftsz[0];
	Flt offy = lev.tile_base;
	int ncols_to_render = gl.xres / lev.tilesize[0] + 2;
	int col = (int)(gl.camera[0] / dd);
	col = col % lev.ncols;
	//Partial tile offset must be determined here.
	//The leftmost tile might be partially off-screen.
	//cdd: camera position in terms of tiles.
	Flt cdd = gl.camera[0] / dd;
	//flo: just the integer portion
	Flt flo = floor(cdd);
	//dec: just the decimal portion
	Flt dec = (cdd - flo);
	//offx: the offset to the left of the screen to start drawing tiles
	Flt offx = -dec * dd;
	//Log("gl.camera[0]: %lf   offx: %lf\n",gl.camera[0],offx);
	for (int j=0; j<ncols_to_render; j++) {
		int row = lev.nrows-1;
		for (int i=0; i<lev.nrows; i++) {
			if (lev.arr[row][col] == 'w') {
				glColor3f(0.8, 0.8, 0.6);
				glPushMatrix();
				//glEnable(GL_BLEND);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				//glColor4f(1.0, 1.0, 0.0, 0.0);
				glBindTexture(GL_TEXTURE_2D, gl.platformTexture);
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				glTranslated(tr.x,tr.y,tr.z);
				int tx = lev.tilesize[0];
				int ty = lev.tilesize[1];
                                tileCollision(&tr, row, col);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2i( 0,  0);
					glTexCoord2f(0.0, 0.0); glVertex2i( 0, ty);
					glTexCoord2f(1.0, 0.0); glVertex2i(tx, ty);
					glTexCoord2f(1.0, 1.0); glVertex2i(tx,  0);
				glEnd();
				//glDisable(GL_BLEND);
				glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
			}	
			if (lev.arr[row][col] == 'l') {
				glColor3f(0.8, 0.8, 0.6);
				glPushMatrix();
				//glEnable(GL_BLEND);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				//glColor4f(1.0, 1.0, 0.0, 0.0);
				glBindTexture(GL_TEXTURE_2D, gl.lavaTexture);
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				glTranslated(tr.x,tr.y,tr.z);
                                tileCollision(&tr, row, col);
				int tx = lev.tilesize[0];
				int ty = lev.tilesize[1];
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2i( 0,  0);
					glTexCoord2f(0.0, 0.0); glVertex2i( 0, ty);
					glTexCoord2f(1.0, 0.0); glVertex2i(tx, ty);
					glTexCoord2f(1.0, 1.0); glVertex2i(tx,  0);
				glEnd();
				//glDisable(GL_BLEND);
				glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			if (lev.arr[row][col] == 'b') {
				glColor3f(0.6, 0.6, 0.6);
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, gl.metalTexture);
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				glTranslated(tr.x,tr.y,tr.z);
				int tx = lev.tilesize[0];
				int ty = lev.tilesize[1];
                                tileCollision(&tr, row, col);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex2i( 0,  0);
					glTexCoord2f(0.0, 0.0); glVertex2i( 0, ty);
					glTexCoord2f(1.0, 0.0); glVertex2i(tx, ty);
					glTexCoord2f(1.0, 1.0); glVertex2i(tx,  0);
				glEnd();
				glPopMatrix();
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			--row;
		}
		++col;
		col = col % lev.ncols;
	}
}

void pauseScreen()
{
	Rect r; 
        float h = 100.0;
        float w = 200.0;
        glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.58,0.58,0.72,0.8);
        glTranslated(gl.xres/2, gl.yres/2, 0);
        glBegin(GL_QUADS);
                glVertex2i(-w, -h);
                glVertex2i(-w,  h);
                glVertex2i(w,   h);
                glVertex2i(w,  -h);
        glEnd();
        glDisable(GL_BLEND);
        glPopMatrix();
        r.bot = gl.yres/2 + 80;
        r.left = gl.xres/2;
        r.center = 1; 
        ggprint8b(&r, 16, 0, "PAUSE SCREEN");
        r.center = 0;
        r.left = gl.xres/2 - 100;
        ggprint8b(&r, 16, 0, "Press P - Play");
        ggprint8b(&r, 16, 0, "Press E - Exit to Menu");
        ggprint8b(&r, 16, 0, "Press Esc - Exit Game");

//Pause Movement
//Pause Time
//Use Enum State
//Link to Menu
//Exit
}
void startTimer()
{
//Include a starting timer that begins the allowment of movement
//start the scoreTime after this function
//Use Enum State
}
void scoreTime()
{
//Add up time and calculate 'if' below or over
//Minutes x 60 and add Frame which is seconds
//Calculate score and give a grade or medal
//--Additional--
//Create a score tally or point system for game
}
void deathScreen()
{
//Use Enum State
//Generate Save Point
//Generate GAME OVER
}
void healthBar(int xres, int yres)
{
//generate a healthbar on top left of the screen
	Rect r;
	unsigned int c = 0x002d88d8;
	r.bot = yres-20;
	r.left = (xres/xres) + 10;
	r.center = 0;
	ggprint8b(&r, 16, c, "HEALTH BAR");
	Shape s;
	Shape box[200];
	for (int i = 0; i < mainChar.health; i++) {
		box[i].width = 10;
		box[i].height = 3;
		box[i].center.x = r.left + 25;
		box[i].center.y = 570 - (i*8);
		box[i].center.z = 0;
		s = box[i];
		glPushMatrix();
		glColor3ub(255, 0, 255);
		glTranslatef(s.center.x, s.center.y, s.center.z);
		float w = s.width;
		float h = s.height;
		glBegin(GL_QUADS);
		glVertex2i(-w, -h);
		glVertex2i(-w, h);
		glVertex2i(w, h);
		glVertex2i(w, -h);
		glEnd();
		glPopMatrix();
	}
}
void renderTimeDisplay()
{
	Rect r;
	unsigned int c = 0x002d88d8;
	r.bot = gl.yres - 30;
	r.left = (gl.xres/2);
	r.center = 0;
	if (gl.state == STATE_GAMEPLAY) {
		timers.recordTime(&timers.timeCurrent);
        	double timeSpan = timers.timeDiff(&timers.gameTime, &timers.timeCurrent);
        	if (timeSpan > gl.gameDelay) {
                	//advance
                	++gl.gameFrame;
                	timers.recordTime(&timers.gameTime);
        	}
        	if (gl.gameFrame >= 60)
        	{
                	++gl.minutes;
                	gl.gameFrame = 0;
        	}
	}
	ggprint16(&r, 32, c, "%d:%02i", gl.minutes, gl.gameFrame);

}

void levelCompletion ()
{
//Generate Level Completion
//Link to Menu
//Stop Tiles from moving
}
